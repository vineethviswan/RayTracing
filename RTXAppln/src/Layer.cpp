
#include "Layer.h"
#include "Vector3.h"
#include "Ray.h"

void Layer::Init(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	m_ImageData = new unsigned char[width * height * 4];
	m_FinalImage = new Image(width, height, ImageFormat::RGBA);

	m_PixelData = m_Camera.Init(width, height);
}

Layer::Layer() :
	m_ImageData (nullptr),
	m_FinalImage (nullptr),
    m_Width (0), m_Height(0)
{	
}

Layer::~Layer()
{
	if (m_ImageData)
	{
		delete[] m_ImageData;
		m_ImageData = nullptr;
	}

	if(m_FinalImage)
	{
		delete m_FinalImage;
		m_FinalImage = nullptr;
	}	
}

ID3D11ShaderResourceView* Layer::GetShaderResourceView() const
{
	return m_FinalImage->GetShaderResourceView();	
}

bool Layer::IsImageChanged(uint32_t width, uint32_t height) const 
{
	return m_Width != width || m_Height != height;
}

void Layer::RenderImage()
{
	// Generate image buffer
	uint32_t line_width = m_Width * 4;
	uint32_t y = 0;
	uint32_t x = 0;

	auto camera_center = Point3(0, 0, 0);

	for (; y < m_Height; y++)
	{
		for (x = 0; x < line_width; x += 4)
		{
			auto pixel_center = m_PixelData.pixel_00_location + (x * m_PixelData.pixel_delta_u) + (y * m_PixelData.pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			Ray r(camera_center, ray_direction);
			
			Color pixel_color = r.RayColor(r);			
						
			m_ImageData[x + y * line_width] = int(255.999 * pixel_color.GetX());
			m_ImageData[x + y * line_width + 1] = int(255.999 * pixel_color.GetY());
			m_ImageData[x + y * line_width + 2] = int(255.999 * pixel_color.GetZ());
			m_ImageData[x + y * line_width + 3] = 0xFF;
		}
	}
	m_FinalImage->SetData(m_ImageData);
	m_FinalImage->CreateTexture();	
}
