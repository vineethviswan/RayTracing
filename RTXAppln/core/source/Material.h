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

#endif
