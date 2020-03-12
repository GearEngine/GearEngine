#pragma once
#include "Component.h"

namespace Gear {

	class Drawer : public Component
	{
	public:
		virtual void Render() = 0;
	};

	class Drawer2D : public Drawer
	{
	public:
		Drawer2D();
		virtual ~Drawer2D();

		virtual void Update(Timestep ts) override;
		virtual void Render() override;

		inline void LinkTransform(class Transform2D* transform) { m_LinkedTransform = transform; }
		inline void LinkAnimation(class Animator2D* animator) { m_LinkedAnimator = animator; }
		inline void LinkTexture2D(Ref<class Texture2D> texture2D) { m_LinkedTexture = texture2D; }
		inline void LinkFrameTexture2D(Ref<class FrameTexture2D> frameTexture2D) { m_LinkedFrameTexture = frameTexture2D; }

		Transform2D* m_LinkedTransform = nullptr;
		Animator2D* m_LinkedAnimator = nullptr;
		Ref<Texture2D> m_LinkedTexture;
		Ref<FrameTexture2D> m_LinkedFrameTexture;

		Ref<Texture2D> m_DefaultTexture;
		glm::mat4 m_DefaultTranslate;
	};


}
