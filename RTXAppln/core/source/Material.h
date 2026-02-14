#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material
{
public:
    virtual ~Material () = default;

    virtual bool Scatter (const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const
    {
        return false;
    }
};

class Lambertian : public Material
{
public:
    Lambertian (const Color &albedo) : albedo (albedo) { }

    bool Scatter (const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        auto scatter_direction = rec.normal + RandomUnitVector ();

        // Catch degenerate scatter direction
        if (scatter_direction.NearZero ())
            scatter_direction = rec.normal;

        scattered = Ray (rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};

class Metal : public Material
{
public:
    Metal (const Color &albedo) : albedo (albedo) { }

    bool Scatter (const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        Vector3 reflected = Reflect (r_in.direction (), rec.normal);
        scattered = Ray (rec.p, reflected);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};

class Dielectric : public Material
{
public:
    Dielectric (double refraction_index) : refraction_index (refraction_index) { }

    bool Scatter (const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        attenuation = Color (1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        Vector3 unit_direction = UnitVector (r_in.direction ());

        double cos_theta = std::fmin (Dot (unit_direction * -1, rec.normal), 1.0);
        double sin_theta = std::sqrt (1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        Vector3 direction;

        if (cannot_refract || Reflectance (cos_theta, ri) > RandomDouble ())
            direction = Reflect (unit_direction, rec.normal);
        else
            direction = Refract (unit_direction, rec.normal, ri);

        scattered = Ray (rec.p, direction);

        return true;
    }

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refraction_index;

    static double Reflectance (double cosine, double refraction_index)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow ((1 - cosine), 5);
    }
};

#endif
