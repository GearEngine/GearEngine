#pragma once

#include <string>
#include "Gear/Core/Core.h"

namespace Gear {

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

	class TextureStorage
	{
	public:
		static Ref<Texture2D> GetTexture2D(const std::string& name);
		static Ref<FrameTexture2D> GetFrameTexture2D(const std::string& name);
		static void AddTexture2D(const std::string& name, Ref<Texture2D> texture);
		static void AddFrameTexture2D(const std::string& name, Ref<FrameTexture2D> texture);
		//static void DeleteTexture2D(const std::string& name, Ref<FrameTexture2D> texture);
		//static void DeleteFrameTexture2D(const std::string& name, Ref<FrameTexture2D> texture);

		static void Clear();

	private:
		static std::unordered_map<std::string, Ref<Texture2D>> m_Texture2Ds;
		static std::unordered_map<std::string, Ref<FrameTexture2D>> m_FrameTexture2Ds;
	};
}