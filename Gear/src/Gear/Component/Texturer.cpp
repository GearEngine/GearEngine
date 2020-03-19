#include "grpch.h"
#include "Texturer.h"

namespace Gear {

	void Texturer2D::Update(Timestep ts)
	{
	}

	void Texturer2D::RegisterTexture(RenderType::Type renderType, Ref<Texture2D> texture, Ref<Texture2D> mask, Ref<Texture2D> blendTexture)
	{
		m_RenderType = renderType;
		m_Texture = texture;
		m_Mask = mask;
		m_BlendTexture = blendTexture;
	}

}