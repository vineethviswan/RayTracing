
#pragma once

#include <d3d11.h>
#include <cstdint>

enum class ImageFormat { RGBA };

class Image
{
public:
	Image();
	Image(uint32_t width, uint32_t height, ImageFormat format);
	~Image();

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	void SetData(unsigned char* data);
	void CreateTexture();
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_srv; }

private:
	uint32_t m_Width, m_Height;
	ImageFormat m_Format;
	unsigned char* m_ImageBuffer;

	ID3D11Texture2D* m_Texture;
	ID3D11ShaderResourceView* m_srv;

	double m_AspectRatio;
};
