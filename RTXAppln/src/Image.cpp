
#include "Image.h"

extern ID3D11Device* g_pd3dDevice;

Image::Image() : m_Width (0), 
				 m_Height(0), 
				 m_Format (ImageFormat::RGBA),
				 m_ImageBuffer(nullptr),
			     m_Texture(nullptr),
				 m_srv(nullptr)
{
}

Image::~Image()
{
	if (m_ImageBuffer)
	{
		delete[]m_ImageBuffer;
		m_ImageBuffer = nullptr;
	}
}

void Image::SetData(unsigned char* data)
{
	m_ImageBuffer = data;
}

Image::Image(uint32_t width, uint32_t height, ImageFormat format)
{
	m_Format = format;
	m_Width = width;
	m_Height = height;
	m_ImageBuffer = nullptr;
	m_Texture = nullptr;
	m_srv = nullptr;
}

void Image::CreateTexture()
{
	if (!m_ImageBuffer)
		return;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = m_Width;
	desc.Height = m_Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = m_ImageBuffer;
	subResource.SysMemPitch = desc.Width * 4;

	ID3D11Device* device = g_pd3dDevice;

	subResource.SysMemSlicePitch = 0;
	device->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(pTexture, &srvDesc, &m_srv);
	pTexture->Release();

}

