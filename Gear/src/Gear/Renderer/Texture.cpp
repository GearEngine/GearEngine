#include "grpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "stb_image.h"

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

	TextureData::TextureData(const std::string & path)
	{
		stbi_set_flip_vertically_on_load(1);
		Data = nullptr;
		{
			GR_PROFILE_SCOPE("stbi_load - OpenGlTexture2D::OpenGLTexture2D(const std::string&)");
			Data = stbi_load(path.c_str(), &Width, &Height, &Channels, 0);
		}
		GR_CORE_ASSERT(Data, "Failed to load image!");
	}
}