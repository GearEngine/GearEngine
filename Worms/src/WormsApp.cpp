#include "wmpch.h"
#include <Gear/Core/EntryPoint.h>

#include "InGame/InGameScene.h"

class Worms : public Gear::Application
{
public:
	Worms()
	{
		//Load Sound
		
		
		//Create Event Channel
		Gear::EventSystem::CreateChannel(EventChannel::World);
		Gear::EventSystem::CreateChannel(EventChannel::Explosion);
		Gear::EventSystem::CreateChannel(EventChannel::WormMove);
		Gear::EventSystem::CreateChannel(EventChannel::MouseMove);
		Gear::EventSystem::CreateChannel(EventChannel::MouseClick);

		//Create Scene
		Gear::SceneManager::Get()->AddScene(new InGame::InGameScene("InGameScene", m_ImGuilayer, InGame::InitiateData()));

		//Current Scene Setting
		Gear::SceneManager::Get()->changeScene("InGameScene");
	}

	~Worms()
	{
	}
};

Gear::Application* Gear::CreateApplication()
{
	return new Worms();
}

//Create Entity Order
//Create Entity
//Attach Component
//Set Component specific
//Subscpribe EventChannel