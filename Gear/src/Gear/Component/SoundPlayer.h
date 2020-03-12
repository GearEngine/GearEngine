#pragma once

#include "Component.h"
#include "Gear/Manager/Sound.h"

namespace Gear {

	class SoundPlayer : public Component
	{
	public:
		virtual void Update(Timestep ts) override;

	public:
		std::unordered_map<std::string, Ref<Sound>> m_Sound;
	};

}