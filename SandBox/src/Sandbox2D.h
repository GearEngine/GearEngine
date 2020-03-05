#pragma once

#include "Gear.h"

class Sandbox2D : public Gear::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Gear::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Gear::Event& e) override;
private:
	Gear::OrthographicCameraController m_CameraController;

	//Temp
	Gear::Ref<Gear::VertexArray> m_SqaureVA;
	Gear::Ref<Gear::Shader> m_FlatColorShader;

	Gear::Ref<Gear::Texture2D> m_CheckerboardTexture;
	Gear::Ref<Gear::Texture2D> m_Cherno;
	float m_ChernoPosition[2] = {0.0f, 0.0f};

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_SquareColor2 = { 0.7f, 0.1f, 0.2f, 1.0f };
	glm::vec4 tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	float m_SquareRotate = 0.0f;
	static bool temp;
};