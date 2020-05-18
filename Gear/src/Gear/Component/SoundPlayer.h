#pragma once

#include "Component.h"
#include "Gear/Manager/Sound.h"

namespace Gear {

	class SoundPlayer : public Component
	{
		using SoundChannel = unsigned int;
	public:
		SoundPlayer(int id)
			: Component(id)
		{}
		virtual ~SoundPlayer();

	private:
		virtual void Update(Timestep ts) override;
		void PlaySound(EnumType currentState);
		void RegisterSound(const std::initializer_list<std::pair<const EnumType, std::pair<Ref<Sound>, SoundChannel>>>& sounds);

	private:
		std::unordered_map<EnumType, std::pair<Ref<Sound>, SoundChannel>> m_Sounds;
		EnumType m_CurrentState;

		friend class EntitySystem;
	};

}