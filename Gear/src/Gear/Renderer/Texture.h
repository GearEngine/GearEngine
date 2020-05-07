#pragma once

namespace Gear {

	struct TextureData
	{
	private:
		void SetFrameXY();
		void DivideStore();

	public:
		TextureData() = default;
		TextureData(const std::string& path);
		void Free();

		std::string Path;
		std::string Name;

		unsigned char* Data;
		std::vector<std::vector<unsigned char*>> Datas;

		int Width;
		int Height;
		int Channels;
		uint32_t FrameX = 1;
		uint32_t FrameY = 1;
		int FrameWidth;
		int FrameHeight;

		bool isFrameData = false;
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
		static Ref<Texture2D> Create(const TextureData& data);

		virtual unsigned char* GetData() const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;

	protected:
		unsigned char* m_Data;
	};

	class FrameTexture2D : public Texture
	{
	public:
		static Ref<FrameTexture2D> Create(const std::string& path, int frameX, int frameY);
		static Ref<FrameTexture2D> Create(const TextureData& data);

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