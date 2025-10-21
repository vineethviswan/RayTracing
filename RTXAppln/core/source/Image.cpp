
#include "Image.h"
#include "Logger.h"

Image::Image (uint32_t width, uint32_t height) : m_Width (width), m_Height (height)
{
    if (width == 0 || height == 0)
    {
        Logger::Log (Logger::Level::ERR, "Error: Invalid image dimensions");
        return;
    }

    // Allocate CPU buffer
    m_Data.resize (width * height, 0xFF000000); // Initialize to opaque black

    Logger::Log (
            Logger::Level::INFO, "Creating Image Buffer: " + std::to_string (width) + " x " + std::to_string (height));
}

Image::~Image ()
{
    ReleaseGPUResources ();
    Logger::Log (Logger::Level::INFO, "Image Destroyed.");
}

void Image::SetPixel (uint32_t x, uint32_t y, uint32_t rgba)
{
    if (x >= m_Width || y >= m_Height)
    {
        return; // Bounds check
    }

    m_Data[GetIndex (x, y)] = rgba;
    m_Dirty = true;
}

void Image::Clear (uint32_t rgba)
{
    std::fill (m_Data.begin (), m_Data.end (), rgba);
    m_Dirty = true;
}

// GPU synchronization (call before rendering)
void Image::UpdateGPUTexture (ID3D11Device *device, ID3D11DeviceContext *context)
{
    if (!device || !context)
    {
        Logger::Log (Logger::Level::ERR, "Error: Invalid device or context");
        return;
    }

    // Create GPU resources if they don't exist
    if (!m_Texture)
    {
        CreateGPUResources (device);
    }

    // Only update if data has changed
    if (!m_Dirty)
    {
        return;
    }

    // Copy CPU data to GPU texture
    context->UpdateSubresource (m_Texture,
            0, // Subresource index
            nullptr, // Update entire texture
            m_Data.data (), // Source data
            m_Width * sizeof (uint32_t), // Row pitch (bytes per row)
            0 // Depth pitch (not used for 2D)
    );

    m_Dirty = false;
}

void Image::CreateGPUResources (ID3D11Device *device)
{
    Logger::Log (Logger::Level::INFO, "Image::CreateGPUResources");

    ReleaseGPUResources (); // Clean up any existing resources

    // Create 2D texture
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = m_Width;
    textureDesc.Height = m_Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    ID3D11Texture2D *pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = m_Data.data ();
    subResource.SysMemPitch = textureDesc.Width * 4;

    subResource.SysMemSlicePitch = 0;
    HRESULT hr = device->CreateTexture2D (&textureDesc, &subResource, &m_Texture);
    if (FAILED (hr))
    {
        Logger::Log (Logger::Level::ERR, "Failed to create GPU texture");
        return;
    }

    // Create shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;

    hr = device->CreateShaderResourceView (m_Texture, &srvDesc, &m_Srv);
    if (FAILED (hr))
    {
        Logger::Log (Logger::Level::ERR, "Failed to create shader resource view");
        m_Texture->Release ();
        m_Texture = nullptr;
        return;
    }
    Logger::Log (Logger::Level::INFO, "GPU resources created for image");
}

void Image::ReleaseGPUResources ()
{
    if (m_Srv)
    {
        m_Srv->Release ();
        m_Srv = nullptr;
    }

    if (m_Texture)
    {
        m_Texture->Release ();
        m_Texture = nullptr;
    }
}
