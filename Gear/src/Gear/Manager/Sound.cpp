#include "grpch.h"
#include "Sound.h"

namespace Gear {	

	SoundSystem* SoundSystem::s_Instance = nullptr;
	std::unordered_map<std::string, Ref<Sound>> SoundStorage::m_Sounds = std::unordered_map<std::string, Ref<Sound>>();
	
	Sound::Sound(FMOD::Sound* sound, bool loop, bool stream)
		: m_Sound(sound), m_Loop(loop), m_Stream(stream)
	{
	}

	Sound::~Sound()
	{
		if (m_Sound != nullptr)
		{
			m_Sound->release();
			m_Sound = nullptr;
		}
	}


	SoundSystem::SoundSystem()
	{
		FMOD::System_Create(&s_System);
		s_System->init(SOUND_CHANNEL_MAX, FMOD_INIT_NORMAL, nullptr);
	}

	void SoundSystem::Destroy()
	{
		SoundStorage::Clear();
		if (s_Instance)
		{
			delete s_Instance;
		}
		s_Instance = nullptr;
	}

	

	SoundSystem * SoundSystem::Get()
	{
		if (!s_Instance)
		{
			Init();
		}
		return s_Instance;
	}

	void SoundSystem::Init()
	{
		s_Instance = new SoundSystem;
	}

	Ref<class Sound> SoundSystem::CreateSound(const std::string& path, bool loop, bool stream)
	{
		if (!s_Instance)
		{
			s_Instance = new SoundSystem;
		}

		FMOD::Sound* sound;
		if (stream)
		{
			if (loop)
			{
				s_System->createStream(path.c_str(), FMOD_LOOP_NORMAL, 0, &sound);
			}
			else
			{
				s_System->createStream(path.c_str(), FMOD_LOOP_OFF, 0, &sound);
			}
		}
		else
		{
			if (loop)
			{
				s_System->createSound(path.c_str(), FMOD_LOOP_NORMAL, 0, &sound);
			}
			else
			{
				s_System->createSound(path.c_str(), FMOD_LOOP_OFF, 0, &sound);
			}
		}
		return CreateRef<Sound>(sound, loop, stream);
	}

	void SoundSystem::PlaySound_(Ref<Sound> sound, SoundChannel channel)
	{
		s_System->playSound(sound->m_Sound, 0, false, &m_Channel[channel]);
	}

	void SoundSystem::StopChannel(SoundChannel channel)
	{
		m_Channel[channel]->stop();
	}

	void SoundSystem::PauseChannel(SoundChannel channel)
	{
		m_Channel[channel]->setPaused(true);
	}

	void SoundSystem::ResumeChannel(SoundChannel channel)
	{
		m_Channel[channel]->setPaused(false);
	}

	bool SoundSystem::isPlaying(SoundChannel channel)
	{
		bool ret;
		m_Channel[channel]->isPlaying(&ret);
		return ret;
	}

	void SoundSystem::SetVolue(SoundChannel channel, float volume)
	{
		m_Channel[channel]->setVolume(volume);
	}

	void SoundSystem::AllStop()
	{
		for (int i = 0; i < SOUND_CHANNEL_MAX; ++i)
		{
			m_Channel[i]->stop();
		}
	}

	void SoundSystem::Update()
	{
		s_System->update();
	}

	void SoundStorage::AddSound_(const std::string & name, Ref<Sound> sound)
	{
		if (m_Sounds.find(name) == m_Sounds.end())
		{
			m_Sounds.insert({ name, sound });
		}
		else
		{
			GR_CORE_WARN("SoundStorage::AddSound {0} aleady have!", name);
		}
	}

	Ref<Sound> SoundStorage::GetSound_(const std::string & name)
	{
		auto find = m_Sounds.find(name);
		if (find != m_Sounds.end())
		{
			return find->second;
		}
		else 
		{
			GR_CORE_WARN("{0} does't exist!", name);
			return nullptr;
		}
	}

	void SoundStorage::DeleteSound_(const std::string & name)
	{
		auto find = m_Sounds.find(name);
		if(find != m_Sounds.end())
		{
			m_Sounds.erase(name);
		}
		else
		{
			GR_CORE_WARN("{0} does't exist!", name);
		}
	}

	void SoundStorage::Clear()
	{
		if (m_Sounds.size())
		{
			m_Sounds.clear();
		}
	}
}