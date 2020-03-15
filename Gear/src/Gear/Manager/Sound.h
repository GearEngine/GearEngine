#pragma once

#include "fmod.hpp"

enum SoundChannel
{
	BGM,
	Effect,
	Max = 100
};

namespace Gear {

	class Sound
	{
		friend class SoundSystem;
	public:
		Sound(FMOD::Sound* sound, bool loop = false, bool stream = false);
		~Sound();

	private:
		FMOD::Sound* m_Sound = nullptr;
		bool m_Loop;
		bool m_Stream;
	};

	class SoundSystem
	{
	private:
		SoundSystem();

	public:
		static void Destroy();
		static SoundSystem* Get();

	public:
		static Ref<Sound> CreateSound(const std::string& path, bool loop, bool stream);

		void PlaySound_(Ref<Sound> sound, SoundChannel channel);
		void StopChannel(SoundChannel channel);
		void PauseChannel(SoundChannel channel);
		void ResumeChannel(SoundChannel channel);

	private:
		static FMOD::System* s_System;
		static SoundSystem* s_Instance;
		FMOD::Channel* m_Channel[SoundChannel::Max];
	};

	class SoundStorage 
	{
		friend class SoundSystem;
	public:
		static void AddSound_(const std::string& name, Ref<Sound> sound);
		static Ref<Sound> GetSound_(const std::string& name);
		static void DeleteSound_(const std::string& name);

	private:
		static void Clear();
		static std::unordered_map<std::string, Ref<Sound>> m_Sounds;
	};
}

#define ADD_SOUND(path, name, loop, stream) Gear::SoundStorage::AddSound_(name, Gear::SoundSystem::CreateSound(path, loop, stream))
#define GET_SOUND(name) Gear::SoundStorage::GetSound_(name)
#define PLAY_SOUND(name, channel) Gear::SoundSystem::Get()->PlaySound_(name, channel)