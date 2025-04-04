
#pragma once
#include <cstdint>
#include <d3d11.h>
#include <memory>
#include "Image.h"
#include "Camera.h"

class Layer
{
public:
	Layer();
	~Layer();
	void RenderImage();
	void Init(uint32_t width, uint32_t height);
	ID3D11ShaderResourceView* GetShaderResourceView() const;
	bool IsImageChanged(uint32_t width, uint32_t height) const;	

private:
	unsigned char* m_ImageData;
	Image *m_FinalImage;
	uint32_t m_Width, m_Height;	

	Camera m_Camera;
	PixelData m_PixelData;
};

