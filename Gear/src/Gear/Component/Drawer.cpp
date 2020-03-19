#include "grpch.h"
#include "Drawer.h"

#include "Gear/Renderer/Renderer2D.h"

#include "Animator.h"
#include "Transform.h"

#include <iostream>

namespace Gear {


	Drawer2D::~Drawer2D()
	{
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
		}
	}
}