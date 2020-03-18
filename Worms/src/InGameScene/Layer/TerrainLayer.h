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
	float m_Radius = 20.0f;
	Gear::Ref<Gear::Texture2D> m_Terrian;

	glm::vec2 m_Mouse;
	glm::vec2 m_MouseOnWorld;
	glm::vec2 m_MouseToLocalTexture;
	glm::vec3 m_Pixel;
};

class Mask
{
public:
	static void SetMask(std::string maskName, const glm::vec3& position, const glm::vec2& scale);
	
public:
	static Gear::Ref<Gear::Texture2D> s_Mask;
	static glm::mat4 s_MaskTranslate;
};

class Camera
{
public:
	static Gear::OrthographicCameraController s_CameraController;
	
public:
	static void SetCamera(float windowWidth, float windowHeight, bool rotate = false);
	static void OnUpdate(Gear::Timestep ts);
	static void OnEvent(Gear::Event& e);
};