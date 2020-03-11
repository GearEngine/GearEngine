#include "grpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Gear {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		}
		GR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string & path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}
		GR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<FrameTexture2D> FrameTexture2D::Create(const std::string & path, int frameX, int frameY)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFrameTexture2D>(path, frameX, frameY);
		}
		GR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	std::unordered_map<std::string, Ref<Texture2D>> TextureStorage::m_Texture2Ds = std::unordered_map < std::string, Ref<Texture2D>>();
	std::unordered_map<std::string, Ref<FrameTexture2D>> TextureStorage::m_FrameTexture2Ds = std::unordered_map < std::string, Ref<FrameTexture2D>>();
	
	Ref<Texture2D> TextureStorage::GetTexture2D(const std::string & name)
	{
		auto find = m_Texture2Ds.find(name);
		if (find != m_Texture2Ds.end())
		{
			return find->second;
		}
		else
		{
			GR_CORE_WARN("%s doesn't exist!", name);
		}
	}
	Ref<FrameTexture2D> TextureStorage::GetFrameTexture2D(const std::string & name)
	{
		auto find = m_FrameTexture2Ds.find(name);
		if (find != m_FrameTexture2Ds.end())
		{
			return find->second;
		}
		else
		{
			GR_CORE_WARN("%s doesn't exist!", name);
		}
	}
	void TextureStorage::AddTexture2D(const std::string & name, Ref<Texture2D> texture)
	{
		auto find = m_Texture2Ds.find(name);
		if (find == m_Texture2Ds.end())
		{
			m_Texture2Ds.insert({ name, texture });
		}
		else
		{
			GR_CORE_WARN("%s aleady exist!", name);
		}
	}

	void TextureStorage::AddFrameTexture2D(const std::string & name, Ref<FrameTexture2D> texture)
	{
		auto find = m_FrameTexture2Ds.find(name);
		if (find == m_FrameTexture2Ds.end())
		{
			m_FrameTexture2Ds.insert({ name, texture });
		}
		else
		{
			GR_CORE_WARN("%s aleady exist!", name);
		}
	}
	void TextureStorage::Clear()
	{
		m_Texture2Ds.clear();
		m_FrameTexture2Ds.clear();
	}
}