#pragma once
#include "Component.h"
#include "Gear/Renderer/Texture.h"
#include "Texturer.h"

namespace Gear {

	class Drawer : public Component
	{
	protected:
		Drawer(int id)
			: Component(id)
		{}
	private:
		virtual void Render() = 0;
	};

	class Drawer2D : public Drawer
	{
	public:
		Drawer2D(int id)
			: Drawer(id)
		{}
		virtual ~Drawer2D();

	private:

		virtual void Update(Timestep ts) override;
		virtual void Render() override;
		void RenderAnimation();
		void RenderTexutre();

		glm::mat4 m_Translate;
		glm::vec4 m_TintColor;

		Ref<class Animation2D> m_Animation;

		RenderType::Type m_RenderType;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_Blending;
		Ref<Texture2D> m_Mask;

		friend class EntitySystem;
	};

	
	namespace Stuff
	{
		enum : unsigned int
		{
			Texture,
			FixedTexture,
			FrameTexture,
			FixedFrameTexture,
			Quard,
			FixedQuard,
			Animation,
			FixedAnimation
		};
	}

	class LateDrawer : public Drawer
	{
	public:
		struct TextureStuff
		{
			TextureStuff(Ref<Texture2D> texture, const glm::mat4& translate, const glm::vec4& tintColor)
				: Texture(texture), Translate(translate), TintColor(tintColor)
			{}
			Ref<Texture2D> Texture;
			glm::mat4 Translate;
			glm::vec4 TintColor;
			bool Activate = true;
		};
		struct QuardStuff
		{
			QuardStuff() = default;
			QuardStuff(const glm::mat4& translate, const glm::vec4& color)
				: Translate(translate), Color(color)
			{}
			glm::mat4 Translate;
			glm::vec4 Color;
			bool Activate = true;
		};

	public:
		LateDrawer(int id)
			: Drawer(id)
		{}
		virtual ~LateDrawer();

		virtual void Update(Timestep ts) override;
		virtual void Render() override;

		void UpLoadDrawStuff(const std::string name, const QuardStuff& stuff);
		void UpLoadDrawStuff(const std::string name, const TextureStuff& stuff);
		void Reset();
		
		void SetStuff(const std::string& name, const glm::mat4& translate, const glm::vec4& color);
		void SetStuff(const std::string& name, Ref<Texture2D> texture, const glm::mat4& translate, const glm::vec4& tintColor);
		void EraseStuff(const std::string& name, EnumType stuffType);
		void ActivateStuff(const std::string& name, EnumType type);
		void InActivateStuff(const std::string& name, EnumType type);

		inline QuardStuff& GetQuardStuff(const std::string& name)
		{
			auto find = m_QuadStuffList.find(name);
			if (find != m_QuadStuffList.end())
			{
				return find->second;
			}
			else
			{
				return QuardStuff();
			}
		}

	public:
		std::unordered_map<std::string, QuardStuff> m_QuadStuffList;
		std::unordered_map<std::string, TextureStuff> m_TextureStuffList;

		friend class EntitySystem;
	};
}
