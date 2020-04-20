#pragma once

namespace InGame {

	class TerrainExplosionEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto explosion = std::any_cast<ExplosionData>(data);
			auto mask = Gear::EntitySystem::GetTexturer(entityID)->GetMask();
			auto texture = Gear::EntitySystem::GetTexturer(entityID)->GetTexture();
			auto& terrainTranslate = Gear::EntitySystem::GetTransform2D(entityID)->GetTranslate();

			int width = mask->GetWidth();
			int height = mask->GetHeight();
			unsigned int* maskData = (unsigned int*)mask->GetData();
			unsigned int* textureData = (unsigned int*)texture->GetData();

			auto textureLocalPosition = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(explosion.Position, terrainTranslate);

			SetCircle(textureLocalPosition, width, height, explosion, maskData, textureData, 5);
			SetCircle(textureLocalPosition, width, height, explosion, maskData, nullptr, 0);

			mask->SetData(maskData, width * height * 4);
			texture->SetData(textureData, width * height * 4);

			GR_TRACE("No.{0} Entity : Terrain Damaged by explosion", entityID);
			handled = true;
		}

		void SetCircle(glm::vec2 textureLocalPosition, int width, int height, InGame::ExplosionData explosion, unsigned int* maskData, unsigned int* textureData, int r)
		{
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

			radius += r;

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
					if (textureData == nullptr)
					{
						if (maskData[width * yPos + xPos] == 0xffffffff)
						{
							maskData[width * yPos + xPos] = 0;
						}
					}
					else
					{
						if (maskData[width * yPos + xPos] == 0xffffffff)
						{
							textureData[width * yPos + xPos] = 0xfff6bbb7;
						}
					}
					/*if (_data[width * yPos + xPos] == 0xffffffff){
						_data[width * yPos + xPos] = color;
					}
					if(_data[width * yPos + xPos] == 25)
						_data[width * yPos + xPos] = color;*/
				}
			}
		}
	};
	/*void BackGroundLayer::DestroyMask(float x, float y, float radius)
	{
		
	}*/
}