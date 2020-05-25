#include "wmpch.h"
#include "Donkey.h"

#include "DonkeyFSMHandlers.h"
#include "../ItemPixelCollisionHandler.h"

namespace InGame {

	Donkey::Donkey(const InitiateData & initData)
	{
		m_ID = Gear::EntitySystem::CreateEntity(false, "Donkey");

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::FSM, Gear::ComponentID::Timer, Gear::ComponentID::Drawer,
			Gear::ComponentID::Status, Gear::ComponentID::Transform, Gear::ComponentID::Physics,
			Gear::ComponentID::Animator
			});

		Gear::EntitySystem::SetFSM(m_ID, {
			{ Item::State::OnGoing, new DonkeyOnGoingHandler }
		});

		Gear::EntitySystem::SetAnimator(m_ID, {
			{ Item::State::OnGoing, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("DonkeyBullet"), 0.0f) }
		});

		auto mask = initData.Mapinfo.Mask;
		int width = mask->GetWidth();
		int height = mask->GetHeight();

		auto maskTranslate = glm::translate(glm::mat4(1.0f), initData.MapPosition)
			* glm::scale(glm::mat4(1.0f), { width / initData.MapReductionRatio  , height / initData.MapReductionRatio , 1.0f });

		Gear::EntitySystem::SetPhysics(m_ID, false, 0.4f, 3.0f);
		Gear::EntitySystem::SetPixelCollision(m_ID, { 255.0f, 255.0f, 255.0f }, mask, maskTranslate, {
			{ "DonkeyPC", Gear::CreateRef<DonkeyTypePCHandler>() },
		});

		Gear::EntitySystem::SetStatus(m_ID, {
			{ Item::Info::From, -1}, { Item::Info::Number, ItemInfo::Number::Donkey },
		});
	}

	void Donkey::init(const glm::vec3 & position, float initAngle, float initPower, int From, float explosionTime)
	{
		Gear::EntitySystem::ActivateEntity(m_ID);
		Gear::EntitySystem::GetFSM(m_ID)->SetCurrentState(Item::State::OnGoing);

		glm::vec3 pos(position.x, 20.0f, ZOrder::z_Item);
		Gear::EntitySystem::SetTransform(m_ID, pos, 0.0f, glm::vec2(3.96f, 4.35f));

		auto physics = Gear::EntitySystem::GetPhysics2D(m_ID);
		glm::vec2 ExternalVector(0.0f, 0.0f);
		physics->SetExternalVector(ExternalVector);
		physics->SetPixelCollisionHandler("DonkeyPC");

		Gear::EntitySystem::GetStatus(m_ID)->SetStat(Item::Info::From, From);
	}
}