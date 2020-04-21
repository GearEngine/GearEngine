#include "wmpch.h"
#include "Grenade.h"

#include "GrenadeFSMHandlers.h"
#include "../ItemPixelCollisionHandler.h"
#include "../ItemStatusHandler.h"

namespace InGame {

	Grenade::Grenade(const InitiateData & initData)
	{
		m_ID = Gear::EntitySystem::CreateEntity(false);

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::FSM, Gear::ComponentID::Timer, Gear::ComponentID::Drawer,
			Gear::ComponentID::Status, Gear::ComponentID::Transform, Gear::ComponentID::Physics,
			Gear::ComponentID::Animator
			});

		Gear::EntitySystem::SetAnimator(m_ID, {
			{ Item::State::OnGoing, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("GrenadeBullet"), 0.0f) }
			});

		Gear::EntitySystem::SetFSM(m_ID, {
			{ Item::State::OnGoing, new GrenadeOnGoingHandler }, { Item::State::OnUnderWater, new GrenadeOnUnderWater},
			{ Item::State::OnExplosion, new GrenadeOnExplosion },
		});

		auto mask = Gear::TextureStorage::GetTexture2D(initData.Mapinfo.MapName + "Mask");
		int width = mask->GetWidth();
		int height = mask->GetHeight();

		auto maskTranslate = glm::translate(glm::mat4(1.0f), initData.MapPosition)
			* glm::scale(glm::mat4(1.0f), { width / initData.MapReductionRatio  , height / initData.MapReductionRatio , 1.0f });

		Gear::EntitySystem::SetPhysics(m_ID, true, 20.f, -0.1f);
		Gear::EntitySystem::SetPixelCollision(m_ID, { 255, 255, 255 }, mask, maskTranslate, {
			{ "GrenadePC", Gear::CreateRef<GrenadeTypePCHandler>() },
			{ "GeneralWeaponUnderWater", Gear::CreateRef<GaneralWeaponOnUnderWater>() }
		});

		Gear::EntitySystem::SetStatus(m_ID, {
			{ Item::Info::Angle, 0.0f}, { Item::Info::Power, 50.0f }, {Item::Info::ExplosionText, Explosion::Text::Foom}, {Item::Info::ExplosionSize, Explosion::Size::Size50 },
			{ Item::Info::From, -1}, { Item::Info::Number, ItemInfo::Number::Grenade }
		});

		Gear::EntitySystem::SetStatusHanlder(m_ID, { 
			{ Item::StatusHandleType::DisplayRemainTime, Gear::CreateRef<ItemRemainTimeDisplayHanlder>()}
		});
	}

	void Grenade::init(const glm::vec3 & position, float initAngle, float initPower, int From, float explosionTime)
	{

		Gear::EntitySystem::ActivateEntity(m_ID);
		Gear::EntitySystem::GetFSM(m_ID)->SetCurrentState(Item::State::OnGoing);
		auto timer = Gear::EntitySystem::GetTimer(m_ID);
		timer->SetTimer(explosionTime + 0.1f);
		timer->Start();

		glm::vec3 pos = position;
		pos.z = ZOrder::z_Item;
		Gear::EntitySystem::SetTransform(m_ID, pos, 0.0f, glm::vec2(1.8f, 1.8f));

		auto physics = Gear::EntitySystem::GetPhysics2D(m_ID);
		glm::vec2 ExternalVector(initPower * glm::cos(glm::radians(initAngle)), initPower * glm::sin(glm::radians(initAngle)));
		physics->SetExternalVector(ExternalVector);
		physics->SetPixelCollisionHandler("GrenadePC");

		auto status = Gear::EntitySystem::GetStatus(m_ID);
		status->SetStat(Item::Info::From, From);
		status->PushNeedHandleData(Item::StatusHandleType::DisplayRemainTime, Gear::Status::StatHandleData(0));
	}

}