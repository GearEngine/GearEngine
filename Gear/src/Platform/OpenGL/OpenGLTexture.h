#pragma once

#include "Gear/Renderer/Texture.h"

#include <glad/glad.h>

namespace Gear {

	class OpenGLTexture2D
		: public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string &path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;
		virtual unsigned char* GetData() const override { return m_Data; }

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
		unsigned char* m_Data = nullptr;
	};

	class OpenGLFrameTexture2D
		: public FrameTexture2D
	{
	public:
		OpenGLFrameTexture2D(const std::string &path, int frameX, int frameY);
		virtual ~OpenGLFrameTexture2D();

		virtual uint32_t GetWidth() const override { return m_FrameWidth; }
		virtual uint32_t GetHeight() const override { return m_FrameHeight; }

		//TODO : implement SetData
		virtual void SetData(void* data, int indexX, int indexY, uint32_t size) override {}
		virtual unsigned char* GetData(int indexX, int indexY) const override { return m_Data[indexX][indexY]; }

		virtual void Bind(uint32_t slot = 0) const override {};
		virtual void Bind(int indexX, int indexY, uint32_t slot = 0) const override;

		virtual inline const uint32_t GetMaxFrameX() const override { return m_FrameX; }
		virtual inline const uint32_t GetMaxFrameY() const override { return m_FrameY; }

	private:
		void DivideTexture(unsigned char* data);

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;

		uint32_t m_FrameWidth, m_FrameHeight;
		uint32_t m_FrameX, m_FrameY;

		GLenum m_InternalFormat, m_DataFormat;
		std::vector<std::vector<uint32_t>> m_RendererIDs;
		std::vector<std::vector<unsigned char*>> m_Data;
	};
}