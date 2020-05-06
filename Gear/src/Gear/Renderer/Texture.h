#pragma once

namespace Gear {

	struct TextureData
	{
		TextureData() = default;
		TextureData(const std::string& path);

		std::string Path;
		unsigned char* Data;
		int Width;
		int Height;
		int Channels;
		uint32_t FrameX;
		uint32_t FrameY;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture 
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);

		virtual unsigned char* GetData() const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;

	protected:
		unsigned char* m_Data;
	};

	class FrameTexture2D : public Texture
	{
	public:
		static Ref<FrameTexture2D> Create(const std::string& path, int frameX, int frameY);

	public:
		virtual void Bind(int indexX, int indexY, uint32_t slot = 0) const = 0;

		virtual const uint32_t GetMaxFrameX() const = 0;
		virtual const uint32_t GetMaxFrameY() const = 0;

		virtual void SetData(void* data, int indexX, int indexY, uint32_t size) = 0;
		virtual unsigned char* GetData(int indexX, int indexY) const = 0;

	protected:
		std::vector<std::vector<unsigned char*>> m_Datas;
	};

	
}