#pragma once

#include "fmod.hpp"

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
		using SoundChannel = unsigned int;
		#define SOUND_CHANNEL_MAX 100

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
		bool isPlaying(SoundChannel channel);
		void SetVolue(SoundChannel channel, float volume);
		void AllStop();
		void Update();

	private:
		static FMOD::System* s_System;
		static SoundSystem* s_Instance;
		FMOD::Channel* m_Channel[SOUND_CHANNEL_MAX];
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
#define PLAY_SOUND(sound, channel) Gear::SoundSystem::Get()->PlaySound_(sound, channel)
#define PLAY_SOUND_NAME(name, channel) Gear::SoundSystem::Get()->PlaySound_(GET_SOUND(name), channel)
#define STOP_SOUND_CAHNNEL(channel) Gear::SoundSystem::Get()->StopChannel(channel);
#define IS_PLAYING_SOUND(channel) Gear::SoundSystem::Get()->isPlaying(channel)