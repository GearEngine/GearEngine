#pragma once

#include "InGame/Layer/BackGroundLayer.h"
#include "InGame/Layer/ObjectLayer.h"
#include "InGame/Layer/UILayer.h"

namespace InGame {

	class InGameScene : public Gear::Scene
	{
	public:
		InGameScene(const std::string& name, Gear::Layer* imGuiLayer, const InitiateData& initData)
			: Gear::Scene(name)
		{
			//Load Texture
			Gear::TextureStorage::AddTexture2D("Grad", Gear::Texture2D::Create("assets/textures/Grad.png"));
			Gear::TextureStorage::AddTexture2D("default", Gear::Texture2D::Create("assets/textures/Checkerboard.png"));
			Gear::TextureStorage::AddTexture2D("TerrainMask", Gear::Texture2D::Create("assets/textures/TerrainMask.png"));
			Gear::TextureStorage::AddTexture2D("Terrain", Gear::Texture2D::Create("assets/textures/Terrain.png"));
			Gear::TextureStorage::AddFrameTexture2D("Water", Gear::FrameTexture2D::Create("assets/textures/water2.png", 1, 12));
			Gear::TextureStorage::AddFrameTexture2D("Cursor", Gear::FrameTexture2D::Create("assets/textures/Cursor.png", 5, 1));
			Gear::TextureStorage::AddFrameTexture2D("OnMove", Gear::FrameTexture2D::Create("assets/textures/wwalk.png", 1, 15));
			Gear::TextureStorage::AddFrameTexture2D("OnUseItem", Gear::FrameTexture2D::Create("assets/textures/wairbakd.png", 1, 10));
			Gear::TextureStorage::AddFrameTexture2D("OnIdle", Gear::FrameTexture2D::Create("assets/textures/wbrth1.png", 1, 20));

			PushOverlay(imGuiLayer);
			PushLayer(new UILayer(initData));
			PushLayer(new BackGroundLayer(initData));
			PushLayer(new ObjectLayer(initData));
		}
	};

}
