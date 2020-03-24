#include "wmpch.h"
#include "TurnChecker.h"

namespace InGame {

	TurnChecker::TurnChecker(const InitiateData & initData)
	{
		m_ID = Gear::EntitySystem::CreateEntity(true);

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Texturer, Gear::ComponentID::Transform,
			Gear::ComponentID::Drawer
			});

		auto border = Gear::TextureStorage::GetTexture2D("TurnCheckerBorder");
		float width = border->GetWidth();
		float height = border->GetHeight()* initData.WindowAspectRatio;

		glm::vec3 position(-0.91f, -0.71f, ZOrder::z_Checker);
		glm::vec3 scale(width / 1850.0f, height / 1850.0f, 1.0f);
		Gear::EntitySystem::SetTransform(m_ID, position, 0.0f, scale);

		Gear::EntitySystem::SetTexturer(m_ID, Gear::RenderType::Fixed, border);
	}

	TurnChecker::~TurnChecker()
	{
	}

}