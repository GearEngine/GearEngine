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
		virtual ~Texturer2D()
		{
			m_Texture.reset();
			m_Mask.reset();
			m_BlendTexture.reset();
		}

	private:
		virtual void Update(Timestep ts) override;
		virtual void RegisterTexture(RenderType::Type renderType, Ref<Texture2D> texture, Ref<Texture2D> m_Mask = nullptr , Ref<Texture2D> m_BlendTexture = nullptr);

	public:
		inline Ref<Texture2D> GetTexture() { return m_Texture; }
		inline Ref<Texture2D> GetMask() { return m_Mask; }

	private:
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_Mask;
		Ref<Texture2D> m_BlendTexture;

		RenderType::Type m_RenderType;

		std::vector<Texture2D> m_BatchTextures;

		friend class EntitySystem;
	};


}