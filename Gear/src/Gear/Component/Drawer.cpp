#include "grpch.h"
#include "Drawer.h"

#include "Gear/Renderer/Renderer2D.h"

#include "Animator.h"
#include "Transform.h"

#include <iostream>

namespace Gear {


	Drawer2D::~Drawer2D()
	{
		m_Texture.reset();
		m_Blending.reset();
		m_Mask.reset();
	}

	void Drawer2D::Update(Timestep ts)
	{
	}

	void Drawer2D::Render()
	{
		if (m_Animation) 
		{
			RenderAnimation();
			return;
		}
		if (m_Texture)
		{
			RenderTexutre();
		}
	}

	void Drawer2D::RenderAnimation()
	{
		Renderer2D::DrawAnimation(m_Translate, m_Animation);
	}

	void Drawer2D::RenderTexutre()
	{
		switch (m_RenderType)
		{
		case RenderType::Normal:
			Renderer2D::DrawQuad(m_Translate, m_Texture);
			break;
		case RenderType::Blending:
			GR_CORE_ASSERT(false, "{DrawerCompoenent::Blending Currently not supported!}");
			break;
		case RenderType::Masking:
			Renderer2D::DrawTextureWithMask(m_Translate, m_Texture, m_Mask);
			break;
		case RenderType::Batch:
			GR_CORE_ASSERT(false, "{DrawerCompoenent::Batch Currently not supported!}");
			break;
		case RenderType::Tiling:
			GR_CORE_ASSERT(false, "{DrawerCompoenent::Tiling Currently not supported!}");
			break;
		case RenderType::Fixed:
			Renderer2D::DrawFixedQuad(m_Translate, m_Texture);
			break;
		}
	}

	LateDrawer::~LateDrawer()
	{
		Reset();
	}

	void LateDrawer::Update(Timestep ts)
	{
		//Render();
	}

	void LateDrawer::Render()
	{
		for (auto& stuff : m_TextureStuffList)
		{
			if (stuff.second.Activate)
			{
				Renderer2D::DrawQuad(stuff.second.Translate, stuff.second.Texture, stuff.second.TintColor);
			}

		}
		for (auto& stuff : m_QuadStuffList)
		{
			if (stuff.second.Activate)
			{
				Renderer2D::DrawQuad(stuff.second.Translate, stuff.second.Color);
			}
		}
	}

	void LateDrawer::UpLoadDrawStuff(const std::string name, const QuardStuff & stuff)
	{
		auto find = m_QuadStuffList.find(name);
		if (find == m_QuadStuffList.end())
		{
			m_QuadStuffList.insert({ name, stuff });
		}
		else
		{
			GR_CORE_WARN("LateDrawer : {} is aleady exist!", name);
		}
	}
	void LateDrawer::UpLoadDrawStuff(const std::string name, const TextureStuff & stuff)
	{
		auto find = m_TextureStuffList.find(name);
		if (find == m_TextureStuffList.end())
		{
			m_TextureStuffList.insert({ name, stuff });
		}
		else
		{
			GR_CORE_WARN("LateDrawer : {} is aleady exist!", name);
		}
	}
	void LateDrawer::Reset()
	{
		m_QuadStuffList.clear();
		m_TextureStuffList.clear();
	}

	void LateDrawer::SetStuff(const std::string & name, const glm::mat4 & translate, const glm::vec4 & color)
	{
		auto find = m_QuadStuffList.find(name);
		if (find != m_QuadStuffList.end())
		{
			find->second.Translate = translate;
			find->second.Color = color;
		}
		else
		{
			GR_CORE_WARN("LateDrawer : {} doesn't exist!", name);
		}

	}

	void LateDrawer::SetStuff(const std::string & name, Ref<Texture2D> texture, const glm::mat4 & translate, const glm::vec4 & tintColor)
	{
		auto find = m_TextureStuffList.find(name);
		if(find != m_TextureStuffList.end())
		{ 
			find->second.Translate = translate;
			find->second.Texture = texture;
			find->second.TintColor = tintColor;
		}
		else
		{
			GR_CORE_WARN("LateDrawer : {} doesn't exist!", name);
		}
	}

	void LateDrawer::EraseStuff(const std::string & name, EnumType type)
	{
		switch (type)
		{
		case Stuff::Texture:
		{
			auto find = m_TextureStuffList.find(name);
			if (find != m_TextureStuffList.end())
			{
				m_TextureStuffList.erase(find);
			}
			else
			{
				GR_CORE_WARN("LateDrawer : {} doesn't exist!", name);
			}
		}
		case Stuff::Quard:
		{
			auto find = m_QuadStuffList.find(name);
			if (find != m_QuadStuffList.end())
			{
				m_QuadStuffList.erase(find);
			}
			else
			{
				GR_CORE_WARN("LateDrawer : {} doesn't exist!", name);
			}
		}
		}
	}

	void LateDrawer::ActivateStuff(const std::string & name, EnumType type)
	{
		switch (type)
		{
		case Stuff::Texture:
		{
			auto find = m_TextureStuffList.find(name);
			if (find != m_TextureStuffList.end())
			{
				find->second.Activate = true;
			}
			else
			{
				GR_CORE_WARN("LateDrawer : {} doesn't exist!", name);
			}
		}
		case Stuff::Quard:
		{
			auto find = m_QuadStuffList.find(name);
			if (find != m_QuadStuffList.end())
			{
				find->second.Activate = true;
			}
			else
			{
				GR_CORE_WARN("LateDrawer : {} doesn't exist!", name);
			}
		}
		}
	}

	void LateDrawer::InActivateStuff(const std::string & name, EnumType type)
	{
		switch (type)
		{
		case Stuff::Texture:
		{
			auto find = m_TextureStuffList.find(name);
			if (find != m_TextureStuffList.end())
			{
				find->second.Activate = false;
			}
			else
			{
				GR_CORE_WARN("LateDrawer : {} doesn't exist!", name);
			}
		}
		case Stuff::Quard:
		{
			auto find = m_QuadStuffList.find(name);
			if (find != m_QuadStuffList.end())
			{
				find->second.Activate = false;
			}
			else
			{
				GR_CORE_WARN("LateDrawer : {} doesn't exist!", name);
			}
		}
		}
	}
}