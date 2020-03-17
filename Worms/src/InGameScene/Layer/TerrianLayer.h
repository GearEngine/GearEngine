#pragma once
#include "InGameLayer.h"

class TerrianLayer : public Gear::Layer
{
public:
	TerrianLayer();
	virtual ~TerrianLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Gear::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Gear::Event& e) override;

private:
	Gear::Ref<Gear::Texture2D> m_Mask;
	Gear::Ref<Gear::Texture2D> m_Terrian;
};

class MaskLayer : public Gear::Layer
{
public:
	MaskLayer();
	virtual ~MaskLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Gear::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Gear::Event& e) override;
	void DestroyMask(float x, float y, float radius);
	void MouseOnWorld();
	void WorldToLocal();

private:
	Gear::OrthographicCameraController m_CameraController;
	Gear::Ref<Gear::Texture2D> m_Mask;
	Gear::Ref<Gear::Texture2D> m_Terrian;

	glm::mat4 m_Translate;
	glm::vec3 m_Position = { 0.0f, -5.0f, 0.0f };
	glm::vec2 m_Scale;

	glm::vec2 m_Mouse;
	glm::vec2 m_MouseOnWorld;
	glm::vec3 m_MouseToLocalTexture;

	float m_Radius = 20.0f;
};