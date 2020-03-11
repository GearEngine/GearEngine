#include "grpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Gear {

	/////////////////////////////////////////////////////////////
	// Texture2D Buffer /////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		GR_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string & path)
		:m_Path(path)
	{
		GR_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			GR_PROFILE_SCOPE("stbi_load - OpenGlTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		GR_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		GR_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GR_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void * data, uint32_t size)
	{
		GR_PROFILE_FUNCTION();

		uint32_t bpc = m_DataFormat == GL_RGBA ? 4 : 3;
		GR_CORE_ASSERT(size == m_Width * m_Height * bpc, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		GR_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}


	/////////////////////////////////////////////////////////////
	// Texture2D Buffer /////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	OpenGLFrameTexture2D::OpenGLFrameTexture2D(const std::string & path, int frameX, int frameY)
	{
		GR_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			GR_PROFILE_SCOPE("stbi_load - OpenGlTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		GR_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;
		m_FrameX = frameX;
		m_FrameY = frameY;
		m_FrameWidth = m_Width / m_FrameX;
		m_FrameHeight = m_Height / m_FrameY;

		m_RendererIDs.resize(m_FrameX);
		for (auto& y : m_RendererIDs)
			y.resize(m_FrameY);

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;		
		GR_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		DivideTexture(data);
		stbi_image_free(data);
	}

	OpenGLFrameTexture2D::~OpenGLFrameTexture2D()
	{
		GR_PROFILE_FUNCTION();

		for (int i = 0; i < m_FrameY; ++i)
		{
			for (int j = 0; j < m_FrameX; ++j)
			{
				glDeleteTextures(1, &m_RendererIDs[j][i]);
			}
		}
		m_RendererIDs.clear();
	}

	void OpenGLFrameTexture2D::Bind(int frameX, int frameY, uint32_t slot) const
	{
		GR_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererIDs[frameX][frameY]);
	}

	void OpenGLFrameTexture2D::DivideTexture(unsigned char * data)
	{
		GR_PROFILE_FUNCTION();

		unsigned char* frameData = new unsigned char[m_FrameWidth * m_FrameHeight * 4];

		for (int i = 0; i < m_FrameY; ++i)
		{
			for (int j = 0; j < m_FrameX; ++j)
			{
				glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererIDs[j][i]);
				glTextureStorage2D(m_RendererIDs[j][i], 1, m_InternalFormat, m_FrameWidth, m_FrameHeight);

				glTextureParameteri(m_RendererIDs[j][i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_RendererIDs[j][i], GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glTextureParameteri(m_RendererIDs[j][i], GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(m_RendererIDs[j][i], GL_TEXTURE_WRAP_T, GL_REPEAT);

				int x = 0, y = 0;
				if (m_InternalFormat == GL_RGBA8)
				{
					for (int pixelY = i * m_FrameHeight; pixelY < (i + 1) * m_FrameHeight; ++pixelY)
					{
						for (int pixelX = j * m_FrameWidth * 4; pixelX < (j + 1) * m_FrameWidth * 4; ++pixelX)
						{
							frameData[y * m_FrameWidth * 4 + x++] = data[pixelY * m_Width * 4 + pixelX];
						}
						x = 0;
						y++;
					}
				}	
				else
				{
					for (int pixelY = i * m_FrameHeight; pixelY < (i + 1) * m_FrameHeight; ++pixelY)
					{
						for (int pixelX = j * m_FrameWidth * 3; pixelX < (j + 1) * m_FrameWidth * 3; ++pixelX)
						{
							frameData[y * m_FrameWidth * 3 + x++] = data[pixelY * m_Width * 3 + pixelX];
						}
						x = 0;
						y++;
					}
				}
				glTextureSubImage2D(m_RendererIDs[j][i], 0, 0, 0, m_FrameWidth, m_FrameHeight, m_DataFormat, GL_UNSIGNED_BYTE, frameData);
			}
		}

		delete[] frameData;
	}
}