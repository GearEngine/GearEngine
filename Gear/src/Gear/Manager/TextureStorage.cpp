#include "grpch.h"
#include "TextureStorage.h"

namespace Gear {


	std::unordered_map<std::string, Ref<Texture2D>> TextureStorage::m_Texture2Ds = std::unordered_map < std::string, Ref<Texture2D>>();
	std::unordered_map<std::string, Ref<FrameTexture2D>> TextureStorage::m_FrameTexture2Ds = std::unordered_map < std::string, Ref<FrameTexture2D>>();
	std::unordered_map<std::string, Ref<Animation2D>> TextureStorage::m_Animations = std::unordered_map<std::string, Ref<Animation2D>>();
	
	Ref<Texture2D> TextureStorage::GetTexture2D(const std::string & name)
	{
		auto find = m_Texture2Ds.find(name);
		if (find != m_Texture2Ds.end())
		{
			return find->second;
		}
		else
		{
			GR_CORE_WARN("{0} doesn't exist!", name);
			return nullptr;
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
			GR_CORE_WARN("{0} doesn't exist!", name);
			return nullptr;
		}
	}
	Ref<Animation2D> TextureStorage::GetAnimation2D(const std::string & name)
	{
		auto find = m_Animations.find(name);
		if (find != m_Animations.end())
		{
			return find->second;
		}
		else
		{
			GR_CORE_WARN("{0} doesn't exist!", name);
			return nullptr;
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
			GR_CORE_WARN("{0} aleady exist!", name);
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
			GR_CORE_WARN("{0} aleady exist!", name);
		}
	}
	void TextureStorage::AddAnimation(const std::string & name, Ref<Animation2D> ani)
	{
		auto find = m_Animations.find(name);
		if (find == m_Animations.end())
		{
			m_Animations.insert({ name, ani });
		}
		else
		{
			GR_CORE_WARN("{0} aleady exist!", name);
		}
	}
	void TextureStorage::Clear()
	{
		m_Texture2Ds.clear();
		m_FrameTexture2Ds.clear();
		m_Animations.clear();
	}
}