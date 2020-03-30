#pragma once
#include "Gear/Renderer/Animation.h"

namespace Gear {

	class TextureStorage
	{
	public:
		static Ref<Texture2D> GetTexture2D(const std::string& name);
		static Ref<FrameTexture2D> GetFrameTexture2D(const std::string& name);
		static Ref<Animation2D> GetAnimation2D(const std::string& name);
		static void AddTexture2D(const std::string& name, Ref<Texture2D> texture);
		static void AddFrameTexture2D(const std::string& name, Ref<FrameTexture2D> texture);
		static void AddAnimation(const std::string&name, Ref<Animation2D> ani);
		//static void DeleteTexture2D(const std::string& name, Ref<FrameTexture2D> texture);
		//static void DeleteFrameTexture2D(const std::string& name, Ref<FrameTexture2D> texture);

		static void Clear();

	private:
		static std::unordered_map<std::string, Ref<Texture2D>> m_Texture2Ds;
		static std::unordered_map<std::string, Ref<FrameTexture2D>> m_FrameTexture2Ds;
		static std::unordered_map<std::string, Ref<Animation2D>> m_Animations;
	};

}
