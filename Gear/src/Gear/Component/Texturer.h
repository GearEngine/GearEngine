#pragma once
#include "Component.h"
#include "Gear/Renderer/Texture.h"

namespace Gear {

	namespace RenderType {

		enum Type: unsigned int
		{
			Normal,
			Blending,
			Masking,
			Batch,
			Tiling,
			Fixed
		};

	}


	class Texturer : public Component
	{
	protected:
		Texturer(int id)
			: Component(id)
		{}

	};

	class Texturer2D : public Texturer
	{
	public:
		Texturer2D(int id)
			: Texturer(id)
		{}

	private:
		virtual void Update(Timestep ts) override;
		virtual void RegisterTexture(RenderType::Type renderType, Ref<Texture2D> texture, Ref<Texture2D> m_Mask = nullptr , Ref<Texture2D> m_BlendTexture = nullptr);

	private:
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_Mask;
		Ref<Texture2D> m_BlendTexture;

		RenderType::Type m_RenderType;

		std::vector<Texture2D> m_BatchTextures;

		friend class EntitySystem;
	};


}