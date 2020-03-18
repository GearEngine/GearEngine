#pragma once
#include "InGameLayer.h"

class TerrainLayer : public Gear::Layer
{
public:
	TerrainLayer();
	virtual ~TerrainLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Gear::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Gear::Event& e) override;

	void DestroyMask(float x, float y, float radius);

private:
	Gear::Ref<Gear::Texture2D> m_Mask;
	Gear::Ref<Gear::Texture2D> m_Terrian;

	glm::mat4 m_TerrainTranslate;
	glm::vec3 m_TerrainPosition = { 0.0f, -5.0f, 0.0f };
	glm::vec2 m_TerrainScale;

	glm::vec2 m_Mouse;
	glm::vec2 m_MouseOnWorld;
	glm::vec2 m_MouseToLocalTexture;

	float m_Radius = 20.0f;
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

private:
	Gear::OrthographicCameraController m_CameraController;
	Gear::Ref<Gear::Texture2D> m_Mask;

	glm::mat4 m_MaskTranslate;
	glm::vec3 m_MaskPosition = { 0.0f, -5.0f, -0.5f };
	glm::vec2 m_MaskScale;
};