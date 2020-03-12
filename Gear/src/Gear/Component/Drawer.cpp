#include "grpch.h"
#include "Drawer.h"

#include "Gear/Renderer/Renderer2D.h"

#include "Animator.h"
#include "Transform.h"

namespace Gear {
	Drawer2D::Drawer2D()
	{
		m_DefaultTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_DefaultTranslate = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f})
			* glm::rotate(glm::mat4(1.0f), 0.0f, { 0.0f, 0.0f, 1.0f, })
			* glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f });
	}
	Drawer2D::~Drawer2D()
	{
		m_LinkedAnimator = nullptr;
		m_LinkedTransform = nullptr;
		m_LinkedTexture.reset();
		m_LinkedFrameTexture.reset();
	}

	void Drawer2D::Update(Timestep ts)
	{
	}
	void Drawer2D::Render()
	{
		if (m_LinkedTransform)
		{
			if (m_LinkedAnimator)
			{
				Renderer2D::DrawAnimation(m_LinkedTransform->GetTranslate(), m_LinkedAnimator->GetCurrentAnimation());
			}
			else if (m_LinkedTexture)
			{
				Renderer2D::DrawRotatedQuad(m_LinkedTransform->GetTranslate(), m_LinkedTexture);
			}
			else if (m_LinkedFrameTexture)
			{
				Renderer2D::DrawRotatedQuad(m_LinkedTransform->GetTranslate(), m_LinkedFrameTexture);
			}
			else 
			{
				Renderer2D::DrawRotatedQuad(m_LinkedTransform->GetTranslate(), m_DefaultTexture);
			}
		}
		else
		{
			if (m_LinkedAnimator)
			{
				Renderer2D::DrawAnimation(m_DefaultTranslate, m_LinkedAnimator->GetCurrentAnimation());
			}
			else if (m_LinkedTexture)
			{
				Renderer2D::DrawQuad(m_DefaultTranslate, m_LinkedTexture);
			}
			else if (m_LinkedFrameTexture)
			{
				Renderer2D::DrawQuad(m_DefaultTranslate, m_LinkedFrameTexture);
			}
			else
			{
				Renderer2D::DrawQuad(m_DefaultTranslate, m_DefaultTexture);
			}
		}
	}
}