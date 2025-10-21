#pragma once

#include <cstdint>
#include <d3d11.h>
#include <vector>

class Image
{
public:
    Image (uint32_t width, uint32_t height);
    ~Image ();

    void SetPixel (uint32_t x, uint32_t y, uint32_t rgba);
    void Clear (uint32_t rgba);

    // GPU synchronization (call before rendering)
    void UpdateGPUTexture (ID3D11Device *device, ID3D11DeviceContext *context);

    // Accessors
    uint32_t GetWidth () const { return m_Width; }
    uint32_t GetHeight () const { return m_Height; }
    ID3D11ShaderResourceView *GetSRV () const { return m_Srv; }
    const std::vector<uint32_t> &GetData () const { return m_Data; }
    bool IsDirty () const { return m_Dirty; }

private:
    uint32_t m_Width;
    uint32_t m_Height;
    std::vector<uint32_t> m_Data;
    ID3D11Texture2D *m_Texture = nullptr;
    ID3D11ShaderResourceView *m_Srv = nullptr;

    // Synchronization
    bool m_Dirty = true;

    void CreateGPUResources (ID3D11Device *device);
    void ReleaseGPUResources ();
    size_t GetIndex (uint32_t x, uint32_t y) const { return y * m_Width + x; }
};
