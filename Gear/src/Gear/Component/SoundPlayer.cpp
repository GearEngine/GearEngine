#include "grpch.h"
#include "SoundPlayer.h"

namespace Gear {

	SoundPlayer::~SoundPlayer()
	{
	}

	void SoundPlayer::Update(Timestep ts)
	{
	}

	void SoundPlayer::PlaySound(EnumType currentState)
	{
		auto find = m_Sounds.find(currentState);
		PLAY_SOUND(find->second.first, find->second.second);
	}

	void SoundPlayer::RegisterSound(const std::initializer_list<std::pair<const EnumType, std::pair<Ref<Sound>, SoundChannel>>>& sounds)
	{
		m_Sounds = sounds;
	}
}
