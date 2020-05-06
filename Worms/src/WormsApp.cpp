#include "wmpch.h"
#include <Gear/Core/EntryPoint.h>

#include "Intro/IntroScene.h"
class Worms : public Gear::Application
{
public:
	Worms()
	{
		Gear::SceneManager::Get()->AddScene(new Intro::IntroScene);
		
		//Current Scene Setting
		Gear::SceneManager::Get()->changeScene("IntroScene");
	}

	~Worms()
	{
		Font::ShoutDown();
	}

};

Gear::Application* Gear::CreateApplication()
{
	return new Worms();
}

//Create Specify Order
//Create Entity
//Attach Component
//Set Component specific
//Subscpribe EventChannel
//Register Event Handler