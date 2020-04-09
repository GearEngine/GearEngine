#pragma once

namespace InGame {

	class TerrainExplosionEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto explosion = std::any_cast<ExplosionData>(data);
			auto mask = Gear::EntitySystem::GetTexturer(entityID)->GetMask();
			auto& terrainTranslate = Gear::EntitySystem::GetTransform2D(entityID)->GetTranslate();

			int width = mask->GetWidth();
			int height = mask->GetHeight();
			unsigned int* _data = (unsigned int*)mask->GetData();

			auto textureLocalPosition = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(explosion.Position, terrainTranslate);

			float x = textureLocalPosition.x * width;
			float y = textureLocalPosition.y * height;
			float radius;

			switch (explosion.Size)
			{
			case Explosion::Size::Size25:
				radius = 25.0f;
				break;
			case Explosion::Size::Size50:
				radius = 50.0f;
				break;
			case Explosion::Size::Size75:
				radius = 75.0f;
				break;
			case Explosion::Size::Size100:
				radius = 100.0f;
				break;
			}

			int mHeight = -(radius + 1);

			for (int yPos = y - radius; yPos <= y + radius; ++yPos)
			{
				++mHeight;
				if (yPos < 0 || yPos >= height)
				{
					continue;
				}
				float flatwidth = std::sqrt(std::powf(radius, 2) - std::powf(mHeight, 2));
				if (flatwidth < 1.0f)
				{
					continue;
				}
				if (yPos == y)
				{
					flatwidth -= 1;
				}
				for (int xPos = x - flatwidth; xPos < x + flatwidth; ++xPos)
				{
					if (xPos < 0 || xPos >= width)
					{
						continue;
					}
					_data[width * yPos + xPos] = 0;
				}
			}
			mask->SetData(_data, width * height * 4);

			GR_TRACE("No.{0} Entity : Terrain Damaged by explosion", entityID);
			handled = true;
		}
	};
	/*void BackGroundLayer::DestroyMask(float x, float y, float radius)
	{
		
	}*/
}