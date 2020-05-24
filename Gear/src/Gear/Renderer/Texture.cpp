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

	Ref<Texture2D> Texture2D::Create(const TextureData & data)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(data);
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

	Ref<FrameTexture2D> FrameTexture2D::Create(const TextureData & data)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFrameTexture2D>(data);
		}
		GR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void TextureData::SetFrameXY()
	{
		int length = Path.length();
		//asdf(1, 3).png
		if (Path[length - 5] == ')')
		{
			std::string frameX;
			std::string frameY;
			
			for (int i = length - 6; i > 0; --i)
			{
				if (Path[i] == '(')
				{
					for (int j = i - 1; j > 0; --j)
					{
						if (Path[j] == '\\')
						{
							std::reverse(Name.begin(), Name.end());
							break;
						}
						Name += Path[j];
					}
					bool findWidth = false;
					for (int j = i + 1; j < length; ++j)
					{
						if ('0' <= Path[j] && Path[j] <= '9')
						{
							if (findWidth)
							{
								frameY += Path[j];
							}
							else
							{
								frameX += Path[j];
							}
						}
						if (Path[j] == ',')
						{
							findWidth = true;
						}
						if (Path[j] == ')')
						{
							FrameX = std::atoi(frameX.c_str());
							FrameY = std::atoi(frameY.c_str());
							return;
						}
					}
				}
			}
		}
		else
		{
			for (int j = length - 5; j > 0; --j)
			{
				if (Path[j] == '\\')
				{
					std::reverse(Name.begin(), Name.end());
					break;
				}
				Name += Path[j];
			}
			return;
		}
	}

	void TextureData::DivideStore()
	{
		Datas.resize(FrameX);
		for (int i = 0; i < Datas.size(); ++i)
		{
			Datas[i].resize(FrameY);
		}

		for (int i = 0; i < FrameY; ++i)
		{
			for (int j = 0; j < FrameX; ++j)
			{
				unsigned char* frameData;
				int x = 0, y = 0;
				if (Channels == 4)
				{
					frameData = new unsigned char[FrameWidth * FrameHeight * 4];
					for (int pixelY = i * FrameHeight; pixelY < (i + 1) * FrameHeight; ++pixelY)
					{
						for (int pixelX = j * FrameWidth * 4; pixelX < (j + 1) * FrameWidth * 4; ++pixelX)
						{
							frameData[y * FrameWidth * 4 + x++] = Data[pixelY * Width * 4 + pixelX];
						}
						x = 0;
						y++;
					}
				}
				else
				{
					frameData = new unsigned char[FrameWidth * FrameHeight * 3];
					for (int pixelY = i * FrameHeight; pixelY < (i + 1) * FrameHeight; ++pixelY)
					{
						for (int pixelX = j * FrameWidth * 3; pixelX < (j + 1) * FrameWidth * 3; ++pixelX)
						{
							frameData[y * FrameWidth * 3 + x++] = Data[pixelY * Width * 3 + pixelX];
						}
						x = 0;
						y++;
					}
				}
				Datas[j][i] = frameData;
			}
		}
	}

	TextureData::TextureData(const std::string & path)
	{
		Path = path;
		SetFrameXY();
		Data = nullptr;
		{
			GR_PROFILE_SCOPE("stbi_load - OpenGlTexture2D::OpenGLTexture2D(const std::string&)");
			stbi_set_flip_vertically_on_load(1);
			Data = stbi_load(path.c_str(), &Width, &Height, &Channels, 0);
			FrameWidth = Width / FrameX;
			FrameHeight = Height / FrameY;
		}
		GR_CORE_ASSERT(Data, "Failed to load image!");
		if (Path[path.length() - 5] == ')')
		{
			isFrameData = true;
			DivideStore();
		}
	}

	void TextureData::Free()
	{
		stbi_image_free(Data);
		for (int i = 0; i < Datas.size(); ++i)
		{
			for (int j = 0; j < Datas[i].size(); ++j)
			{
				delete[] Datas[i][j];
			}
		}
	}
	
}