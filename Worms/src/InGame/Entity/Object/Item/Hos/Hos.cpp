#include "wmpch.h"
#include "Hos.h"

#include "HosFSMHandlers.h"
#include "../ItemPixelCollisionHandler.h"

namespace InGame {

	Hos::Hos(const InitiateData & initData)
	{
		m_ID = Gear::EntitySystem::CreateEntity(false, "Hos");

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::FSM, Gear::ComponentID::Timer, Gear::ComponentID::Drawer,
			Gear::ComponentID::Status, Gear::ComponentID::Transform, Gear::ComponentID::Physics,
			Gear::ComponentID::Animator
			});

		Gear::EntitySystem::SetFSM(m_ID, {
			{ Item::State::OnGoing, new HosOnGoingHandler }, {Item::State::OnExplosion, new HosOnExplosionHandler }
		});

		Gear::EntitySystem::SetAnimator(m_ID, {
			{ Item::State::OnGoing, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("HosBullet"), 0.0f) }
		});

		Gear::EntitySystem::GetAnimator2D(m_ID)->GetCurrentAnimation()->GetTintColor().a = 0.7f;

		auto mask = initData.Mapinfo.Mask;
		int width = mask->GetWidth();
		int height = mask->GetHeight();

		auto maskTranslate = glm::translate(glm::mat4(1.0f), initData.MapPosition)
			* glm::scale(glm::mat4(1.0f), { width / initData.MapReductionRatio  , height / initData.MapReductionRatio , 1.0f });

		Gear::EntitySystem::SetPhysics(m_ID, false, 0.4f, 3.0f);
		Gear::EntitySystem::SetPixelCollision(m_ID, { 255.0f, 255.0f, 255.0f }, mask, maskTranslate, {
			{ "HosPC", Gear::CreateRef<HosPCHandler>() },
		});

		Gear::EntitySystem::SetStatus(m_ID, {
			{ Item::Info::From, -1}, { Item::Info::Number, ItemInfo::Number::Donkey },
		});
	}

	void Hos::init(const glm::vec3 & position, float initAngle, float initPower, int From, float explosionTime)
	{
		Gear::EntitySystem::ActivateEntity(m_ID);
		Gear::EntitySystem::GetFSM(m_ID)->SetCurrentState(Item::State::OnGoing);

		glm::vec3 pos(position.x, 13.0f, ZOrder::z_Item);
		Gear::EntitySystem::SetTransform(m_ID, pos, 0.0f, glm::vec2(3.96f, 3.96f));

		auto physics = Gear::EntitySystem::GetPhysics2D(m_ID);
		glm::vec2 ExternalVector(0.0f, 0.0f);
		physics->SetExternalVector(ExternalVector);
		physics->SetPixelCollisionHandler("HosPC");

		Gear::EntitySystem::GetStatus(m_ID)->SetStat(Item::Info::From, From);
	}


}