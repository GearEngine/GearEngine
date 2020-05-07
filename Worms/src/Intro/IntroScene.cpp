#include "wmpch.h"
#include "IntroScene.h"

namespace Intro {

	bool IntroScene::loadingComplete = false;
	std::vector<Gear::TextureData> textureDatas;

	void ResourceLoading()
	{
		//Init Font
		Font::Init();

		std::string dir = std::experimental::filesystem::current_path().string();
		dir += "/assets/textures";

		for (auto& p : std::experimental::filesystem::recursive_directory_iterator(dir))
		{
			std::string path = p.path().string();
			int length = path.length();
			if (path[length - 4] == '.')
			{
				textureDatas.emplace_back(Gear::TextureData(path));
			}
		}
		IntroScene::loadingComplete = true;
	}


	void IntroScene::Update(Gear::Timestep ts)
	{

		if (loadingComplete && !resourceLoadGpu)
		{
			resourceLoadGpu = true;
			for (int i = 0; i < textureDatas.size(); ++i)
			{
				if (textureDatas[i].isFrameData)
				{
					Gear::TextureStorage::AddFrameTexture2D(textureDatas[i].Name, Gear::FrameTexture2D::Create(textureDatas[i]));
				}
				else
				{
					Gear::TextureStorage::AddTexture2D(textureDatas[i].Name, Gear::Texture2D::Create(textureDatas[i]));
				}
			}
		}

		Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Gear::RenderCommand::Clear();

		Gear::Renderer2D::BeginScene(introCamera);

		if (!inFirst && !isEndScene && resourceLoadGpu)
		{
			if (Gear::Input::IsKeyPressd(GR_KEY_ENTER) || Gear::Input::IsKeyPressd(GR_KEY_ESCAPE) || Gear::Input::IsKeyPressd(GR_KEY_SPACE)
				|| Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_RIGHT) || Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
			{
				isEndScene = true;
			}
		}

		if (isEndScene && !inFirst)
		{
			alpha.a -= 0.03f;
			Gear::Renderer2D::DrawQuad(introTranslate, introTexture, alpha);
			if (alpha.a <= 0.0f)
			{
				Gear::SceneManager::Get()->AddScene(new Intro::IntroScene2);
				Gear::SceneManager::Get()->changeScene("IntroScene2");
			}
		}
		else if (inFirst)
		{
			alpha.a += 0.05f;
			if (alpha.a >= 1.0f)
			{
				alpha.a = 1.0f;
				inFirst = false;
			}
			Gear::Renderer2D::DrawQuad(introTranslate, introTexture, alpha);
		}
		else if (!inFirst && resourceLoadGpu)
		{
			Gear::Renderer2D::DrawQuad(introTranslate, introTexture);
			pastTime += ts;
			if (pastTime >= sceneChangeDelay)
			{
				isEndScene = true;
			}
		}

		Gear::Renderer2D::EndScene();
	}

	////Intro2
		//Gear::TextureStorage::AddTexture2D("a1", Gear::Texture2D::Create("assets/textures/Intro/a1.png"));
		//Gear::TextureStorage::AddTexture2D("a2", Gear::Texture2D::Create("assets/textures/Intro/a2.png"));
		//Gear::TextureStorage::AddTexture2D("d1", Gear::Texture2D::Create("assets/textures/Intro/d1.png"));
		//Gear::TextureStorage::AddTexture2D("d2", Gear::Texture2D::Create("assets/textures/Intro/d2.png"));
		//Gear::TextureStorage::AddTexture2D("e", Gear::Texture2D::Create("assets/textures/Intro/e.png"));
		//Gear::TextureStorage::AddTexture2D("g", Gear::Texture2D::Create("assets/textures/Intro/g.png"));
		//Gear::TextureStorage::AddTexture2D("m", Gear::Texture2D::Create("assets/textures/Intro/m.png"));
		//Gear::TextureStorage::AddTexture2D("n", Gear::Texture2D::Create("assets/textures/Intro/n.png"));
		//Gear::TextureStorage::AddTexture2D("o", Gear::Texture2D::Create("assets/textures/Intro/o.png"));
		//Gear::TextureStorage::AddTexture2D("r", Gear::Texture2D::Create("assets/textures/Intro/r.png"));
		////Gear::TextureStorage::AddTexture2D("IntroBack", Gear::Texture2D::Create("assets/textures/Intro/back.png"));
		////Gear::TextureStorage::AddTexture2D("IntroTitle", Gear::Texture2D::Create("assets/textures/Intro/title.png"));

		////Load Texture
		//Gear::TextureStorage::AddTexture2D("Grad0", Gear::Texture2D::Create("assets/textures/BackGround/Grad0.png"));
		//Gear::TextureStorage::AddTexture2D("default", Gear::Texture2D::Create("assets/textures/Checkerboard.png"));
		//Gear::TextureStorage::AddTexture2D("TerrainMask", Gear::Texture2D::Create("assets/textures/TerrainMask.png"));
		//Gear::TextureStorage::AddTexture2D("Terrain", Gear::Texture2D::Create("assets/textures/Terrain.png"));
		//Gear::TextureStorage::AddTexture2D("City", Gear::Texture2D::Create("assets/textures/Map/City.png"));
		//Gear::TextureStorage::AddTexture2D("CityMask", Gear::Texture2D::Create("assets/textures/Map/CityMask.png"));
		//Gear::TextureStorage::AddTexture2D("BlueMountainBack", Gear::Texture2D::Create("assets/textures/Map/BlueMountainBack.png"));
		//Gear::TextureStorage::AddTexture2D("TimerBorder", Gear::Texture2D::Create("assets/textures/UI/TimerBorder.png"));
		//Gear::TextureStorage::AddTexture2D("WindCheckerBorder", Gear::Texture2D::Create("assets/textures/UI/WindCheckerBorder.png"));
		//Gear::TextureStorage::AddTexture2D("TurnCheckerBorder", Gear::Texture2D::Create("assets/textures/UI/TurnCheckerBorder.png"));
		//Gear::TextureStorage::AddTexture2D("WindMask", Gear::Texture2D::Create("assets/textures/UI/WindMask.png"));
		//Gear::TextureStorage::AddTexture2D("WormNameBorder", Gear::Texture2D::Create("assets/textures/UI/WormNameBorder.png"));
		//Gear::TextureStorage::AddTexture2D("WormHpBorder", Gear::Texture2D::Create("assets/textures/UI/WormHpBorder.png"));
		//Gear::TextureStorage::AddTexture2D("WaitingTimeBorder", Gear::Texture2D::Create("assets/textures/UI/WaitingTimeBorder.png"));
		//Gear::TextureStorage::AddTexture2D("Japan", Gear::Texture2D::Create("assets/textures/UI/Japan.png"));
		//Gear::TextureStorage::AddTexture2D("USA", Gear::Texture2D::Create("assets/textures/UI/USA.png"));
		//Gear::TextureStorage::AddTexture2D("RedHpBar", Gear::Texture2D::Create("assets/textures/UI/RedHp.png"));
		//Gear::TextureStorage::AddTexture2D("BlueHpBar", Gear::Texture2D::Create("assets/textures/UI/BlueHp.png"));
		//Gear::TextureStorage::AddTexture2D("WhiteHpBar", Gear::Texture2D::Create("assets/textures/UI/WhiteHp.png"));
		//Gear::TextureStorage::AddTexture2D("ItemTimer", Gear::Texture2D::Create("assets/textures/UI/ItemTimer.png"));
		//Gear::TextureStorage::AddTexture2D("WorldMassageBorder", Gear::Texture2D::Create("assets/textures/UI/WorldMassageBorder.png"));

		//Gear::TextureStorage::AddTexture2D("ItemSelector2", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/2.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector3", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/3.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector4", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/4.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector5", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/5.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector6", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/6.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector7", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/7.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector8", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/8.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector9", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/9.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector10", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/10.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector11", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/11.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector12", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/12.png"));
		//Gear::TextureStorage::AddTexture2D("ItemSelector13", Gear::Texture2D::Create("assets/textures/UI/ItemSelector/13.png"));

		//Gear::TextureStorage::AddFrameTexture2D("BlueWater", Gear::FrameTexture2D::Create("assets/textures/BackGround/BlueWaterWave.png", 1, 12));
		//Gear::TextureStorage::AddFrameTexture2D("Cursor", Gear::FrameTexture2D::Create("assets/textures/Cursor.png", 5, 1));
		//Gear::TextureStorage::AddFrameTexture2D("OnUseItem", Gear::FrameTexture2D::Create("assets/textures/wairbakd.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("FallenLeaf", Gear::FrameTexture2D::Create("assets/textures/BackGround/FallenLeaf.png", 1, 128));
		//Gear::TextureStorage::AddFrameTexture2D("CloudLarge", Gear::FrameTexture2D::Create("assets/textures/BackGround/CloudLarge.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("CloudMiddle", Gear::FrameTexture2D::Create("assets/textures/BackGround/CloudMiddle.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("CloudSmall", Gear::FrameTexture2D::Create("assets/textures/BackGround/CloudSmall.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("WindLeft", Gear::FrameTexture2D::Create("assets/textures/UI/WindLeft.png", 1, 8));
		//Gear::TextureStorage::AddFrameTexture2D("WindRight", Gear::FrameTexture2D::Create("assets/textures/UI/WindRight.png", 1, 8));
		//Gear::TextureStorage::AddFrameTexture2D("RedFollowArrow", Gear::FrameTexture2D::Create("assets/textures/UI/RedFollowingArrow.png", 1, 16));
		//Gear::TextureStorage::AddFrameTexture2D("BlueFollowArrow", Gear::FrameTexture2D::Create("assets/textures/UI/BlueFollowingArrow.png", 1, 16));
		//Gear::TextureStorage::AddFrameTexture2D("BlueAim", Gear::FrameTexture2D::Create("assets/textures/UI/BlueAim.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RedAim", Gear::FrameTexture2D::Create("assets/textures/UI/RedAim.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("Blob", Gear::FrameTexture2D::Create("assets/textures/Effect/Blob.png", 1, 16));

		//Gear::TextureStorage::AddFrameTexture2D("RedCursor", Gear::FrameTexture2D::Create("assets/textures/UI/Cursor/RedCursor.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("BlueCursor", Gear::FrameTexture2D::Create("assets/textures/UI/Cursor/BlueCursor.png", 1, 32));

		////Grave
		//Gear::TextureStorage::AddFrameTexture2D("Grave1", Gear::FrameTexture2D::Create("assets/textures/Grave/grave1.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("Grave2", Gear::FrameTexture2D::Create("assets/textures/Grave/grave2.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("Grave3", Gear::FrameTexture2D::Create("assets/textures/Grave/grave3.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("Grave4", Gear::FrameTexture2D::Create("assets/textures/Grave/grave4.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("Grave5", Gear::FrameTexture2D::Create("assets/textures/Grave/grave5.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("Grave6", Gear::FrameTexture2D::Create("assets/textures/Grave/grave6.png", 1, 20));

		////Explosion
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionCircle25", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionCircle25.png", 1, 8));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionCircle50", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionCircle50.png", 1, 8));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionCircle75", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionCircle75.png", 1, 4));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionCircle100", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionCircle100.png", 1, 4));

		//Gear::TextureStorage::AddFrameTexture2D("ExplosionEllipse25", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionEllipse25.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionEllipse50", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionEllipse50.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionEllipse75", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionEllipse75.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionEllipse100", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionEllipse100.png", 1, 10));

		//Gear::TextureStorage::AddFrameTexture2D("ExplosionTextFoom", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionTextFoom.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionTextBiff", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionTextBiff.png", 1, 12));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionTextPoot", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionTextPoot.png", 1, 18));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionTextPow", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionTextPow.png", 1, 12));

		//Gear::TextureStorage::AddFrameTexture2D("ExplosionSmoke25", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionSmoke25.png", 1, 28));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionSmoke50", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionSmoke50.png", 1, 28));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionSmoke75", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionSmoke75.png", 1, 28));
		//Gear::TextureStorage::AddFrameTexture2D("ExplosionSmoke100", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/ExplosionSmoke100.png", 1, 28));

		//Gear::TextureStorage::AddFrameTexture2D("Smoke20", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/Smoke20.png", 1, 28));
		//Gear::TextureStorage::AddFrameTexture2D("Smoke30", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/Smoke30.png", 1, 28));
		//Gear::TextureStorage::AddFrameTexture2D("Smoke40", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/Smoke40.png", 1, 28));

		//Gear::TextureStorage::AddFrameTexture2D("Flame", Gear::FrameTexture2D::Create("assets/textures/Effect/Explosion/Flame1.png", 1, 32));

		////Bullet
		//Gear::TextureStorage::AddFrameTexture2D("BazukaBullet", Gear::FrameTexture2D::Create("assets/textures/Weapon/BazukaBullet.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("GrenadeBullet", Gear::FrameTexture2D::Create("assets/textures/Weapon/GrenadeBullet.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("BananaBullet", Gear::FrameTexture2D::Create("assets/textures/Weapon/BananaBullet.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("Exhaust1", Gear::FrameTexture2D::Create("assets/textures/Effect/Exhaust/Exhaust1.png", 1, 28));

		////Worm
		//Gear::TextureStorage::AddFrameTexture2D("Drawn", Gear::FrameTexture2D::Create("assets/textures/Worm/Drawn.png", 1, 4));
		//Gear::TextureStorage::AddFrameTexture2D("LeftWormDying", Gear::FrameTexture2D::Create("assets/textures/Worm/Dying/LeftWormDying.png", 1, 60));
		//Gear::TextureStorage::AddFrameTexture2D("RightWormDying", Gear::FrameTexture2D::Create("assets/textures/Worm/Dying/RightWormDying.png", 1, 60));

		////walk
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/LeftFlatWalk.png", 1, 15));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/LeftUpWalk.png", 1, 15));
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/LeftDownWalk.png", 1, 15));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/RightFlatWalk.png", 1, 15));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/RightUpWalk.png", 1, 15));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/RightDownWalk.png", 1, 15));
		////Breath
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/LeftFlatBreath.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/LeftUpBreath.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/LeftDownBreath.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/RightFlatBreath.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/RightUpBreath.png", 1, 20));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/RightDownBreath.png", 1, 20));
		////Jump
		//Gear::TextureStorage::AddFrameTexture2D("LeftJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftJumping.png", 1, 7));
		//Gear::TextureStorage::AddFrameTexture2D("RightJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightJumping.png", 1, 7));
		//Gear::TextureStorage::AddFrameTexture2D("LeftBackFlip", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftBackFlip.png", 1, 22));
		//Gear::TextureStorage::AddFrameTexture2D("RightBackFlip", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightBackFlip.png", 1, 22));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightFlatReadyJump.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightUpReadyJump.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightDownReadyJump.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftFlatReadyJump.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftUpReadyJump.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftDownReadyJump.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightFlatLand.png", 1, 6));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightUpLand.png", 1, 6));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightDownLand.png", 1, 6));
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftFlatLand.png", 1, 6));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftUpLand.png", 1, 6));
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftDownLand.png", 1, 6));
		////Fallen
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftFlatSliding.png", 1, 3));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftUpSliding.png", 1, 3));
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftDownSliding.png", 1, 3));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightFlatSliding.png", 1, 3));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightUpSliding.png", 1, 3));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightDownSliding.png", 1, 3));

		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftFlatStuck.png", 1, 49));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftUpStuck.png", 1, 49));
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftDownStuck.png", 1, 49));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightFlatStuck.png", 1, 49));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightUpStuck.png", 1, 49));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightDownStuck.png", 1, 49));

		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftFlatAfterSliding.png", 1, 22));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftUpAfterSliding.png", 1, 22));
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftDownAfterSliding.png", 1, 22));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightFlatAfterSliding.png", 1, 22));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightUpAfterSliding.png", 1, 22));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightDownAfterSliding.png", 1, 22));

		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftFlatAfterDamagedSliding.png", 1, 36));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftUpAfterDamagedSliding.png", 1, 36));
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftDownAfterDamagedSliding.png", 1, 36));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightFlatAfterDamagedSliding.png", 1, 36));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightUpAfterDamagedSliding.png", 1, 36));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightDownAfterDamagedSliding.png", 1, 36));

		////Weapon
		////Bazooka
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownBazukaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/LeftDownBazukaReady.png", 1, 7));
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatBazukaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/LeftFlatBazukaReady.png", 1, 7));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpBazukaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/LeftUpBazukaReady.png", 1, 7));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownBazukaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/RightDownBazukaReady.png", 1, 7));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatBazukaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/RightFlatBazukaReady.png", 1, 7));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpBazukaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/RightUpBazukaReady.png", 1, 7));

		//Gear::TextureStorage::AddFrameTexture2D("LeftDownBazukaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/LeftDownBazukaOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatBazukaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/LeftFlatBazukaOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpBazukaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/LeftUpBazukaOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownBazukaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/RightDownBazukaOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatBazukaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/RightFlatBazukaOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpBazukaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Bazuka/RightUpBazukaOn.png", 1, 32));

		////Grenade
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownGrenadeReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/LeftDownGrenadeReady.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatGrenadeReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/LeftFlatGrenadeReady.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpGrenadeReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/LeftUpGrenadeReady.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownGrenadeReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/RightDownGrenadeReady.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatGrenadeReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/RightFlatGrenadeReady.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpGrenadeReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/RightUpGrenadeReady.png", 1, 10));

		//Gear::TextureStorage::AddFrameTexture2D("LeftDownGrenadeOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/LeftDownGrenadeOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatGrenadeOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/LeftFlatGrenadeOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpGrenadeOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/LeftUpGrenadeOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownGrenadeOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/RightDownGrenadeOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatGrenadeOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/RightFlatGrenadeOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpGrenadeOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Grenade/RightUpGrenadeOn.png", 1, 32));

		////Banana
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownBananaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/LeftDownBananaReady.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatBananaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/LeftFlatBananaReady.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpBananaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/LeftUpBananaReady.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownBananaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/RightDownBananaReady.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatBananaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/RightFlatBananaReady.png", 1, 10));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpBananaReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/RightUpBananaReady.png", 1, 10));

		//Gear::TextureStorage::AddFrameTexture2D("LeftDownBananaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/LeftDownBananaOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatBananaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/LeftFlatBananaOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpBananaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/LeftUpBananaOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownBananaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/RightDownBananaOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatBananaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/RightFlatBananaOn.png", 1, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpBananaOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/Banana/RightUpBananaOn.png", 1, 32));

		////SkipGo
		//Gear::TextureStorage::AddFrameTexture2D("LeftDownSkipGoReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/LeftDownSkipGoReady.png", 1, 5));
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatSkipGoReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/LeftFlatSkipGoReady.png", 1, 5));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpSkipGoReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/LeftUpSkipGoReady.png", 1, 5));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownSkipGoReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/RightDownSkipGoReady.png", 1, 5));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatSkipGoReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/RightFlatSkipGoReady.png", 1, 5));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpSkipGoReady", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/RightUpSkipGoReady.png", 1, 5));

		//Gear::TextureStorage::AddFrameTexture2D("LeftDownSkipGoOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/LeftDownSkipGoOn.png", 1, 9));
		//Gear::TextureStorage::AddFrameTexture2D("LeftFlatSkipGoOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/LeftFlatSkipGoOn.png", 1, 9));
		//Gear::TextureStorage::AddFrameTexture2D("LeftUpSkipGoOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/LeftUpSkipGoOn.png", 1, 9));
		//Gear::TextureStorage::AddFrameTexture2D("RightDownSkipGoOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/RightDownSkipGoOn.png", 1, 9));
		//Gear::TextureStorage::AddFrameTexture2D("RightFlatSkipGoOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/RightFlatSkipGoOn.png", 1, 9));
		//Gear::TextureStorage::AddFrameTexture2D("RightUpSkipGoOn", Gear::FrameTexture2D::Create("assets/textures/Worm/OnWeapon/SkipGo/RightUpSkipGoOn.png", 1, 9));

		////Icon
		//Gear::TextureStorage::AddTexture2D("ItemHighlighter", Gear::Texture2D::Create("assets/textures/Weapon/Icon/ItemHighlighter.png"));

		//Gear::TextureStorage::AddTexture2D("Low Gravity", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Low Gravity.png"));
		//Gear::TextureStorage::AddTexture2D("Bazooka", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Bazooka.png"));
		//Gear::TextureStorage::AddTexture2D("Grenade", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Grenade.png"));
		//Gear::TextureStorage::AddTexture2D("Shotgun", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Shotgun.png"));
		//Gear::TextureStorage::AddTexture2D("Fire Punch", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Fire Punch.png"));
		//Gear::TextureStorage::AddTexture2D("Dynamite", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Dynamite.png"));
		//Gear::TextureStorage::AddTexture2D("Air Strike", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Air strike.png"));
		//Gear::TextureStorage::AddTexture2D("Blow Torch", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Blow Torch.png"));
		//Gear::TextureStorage::AddTexture2D("Ninja Rope", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Ninja Rope.png"));
		//Gear::TextureStorage::AddTexture2D("Super Banana", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Super Banana.png"));
		//Gear::TextureStorage::AddTexture2D("Holy Grenade", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Holy Grenade.png"));
		//Gear::TextureStorage::AddTexture2D("Petrol Bomb", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Petrol Bomb.png"));
		//Gear::TextureStorage::AddTexture2D("Mad Cow", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Mad Cow.png"));
		//Gear::TextureStorage::AddTexture2D("Skip Go", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Skip Go.png"));
		//Gear::TextureStorage::AddTexture2D("Homing Pigeon", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Homing Pigeon.png"));
		//Gear::TextureStorage::AddTexture2D("Homing Missile", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Homing Missile.png"));
		//Gear::TextureStorage::AddTexture2D("Mortar", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Mortar.png"));
		//Gear::TextureStorage::AddTexture2D("Sheep Launcher", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Sheep Launcher.png"));
		//Gear::TextureStorage::AddTexture2D("Banana", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Banana.png"));
		//Gear::TextureStorage::AddTexture2D("Teleport", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Teleport.png"));
		//Gear::TextureStorage::AddTexture2D("Surrender", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Surrender.png"));
		//Gear::TextureStorage::AddTexture2D("Pneumatic Dril", Gear::Texture2D::Create("assets/textures/Weapon/Icon/Pneumatic Dril.png"));

		////Fly
		//Gear::TextureStorage::AddFrameTexture2D("LeftWormFly", Gear::FrameTexture2D::Create("assets/textures/Worm/Fly/LeftWormFly.png", 3, 32));
		//Gear::TextureStorage::AddFrameTexture2D("RightWormFly", Gear::FrameTexture2D::Create("assets/textures/Worm/Fly/RightWormFly.png", 3, 32));


		////animation
		//std::vector<std::pair<int, int>> orderVector;
		//for (int i = 0; i < 32; ++i)
		//{
		//	if (i < 16)
		//	{
		//		orderVector.push_back({ 0, i });
		//	}
		//	else
		//	{
		//		orderVector.push_back({ 0, 31 - i });
		//	}
		//}
		//auto redArrow = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RedFollowArrow"), 0.02f, orderVector, true);
		//auto blueArrow = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("BlueFollowArrow"), 0.02f, orderVector, true);
		//redArrow->Start();
		//blueArrow->Start();
		//Gear::TextureStorage::AddAnimation("RedFollowArrow", redArrow);
		//Gear::TextureStorage::AddAnimation("BlueFollowArrow", blueArrow);

		//auto redCursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RedCursor"), 0.0f, true);
		//auto blueCursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("BlueCursor"), 0.0f, true);
		//Gear::TextureStorage::AddAnimation("RedCursor", redCursor);
		//Gear::TextureStorage::AddAnimation("BlueCursor", blueCursor);
}