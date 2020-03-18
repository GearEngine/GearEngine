#include "Terrain.h"

namespace InGame {

	/*Gear::Ref<Gear::Texture2D> Mask::s_Mask = nullptr;
	glm::mat4 Mask::s_MaskTranslate = glm::mat4(1.0f);

	void Mask::SetMask(std::string maskName, const glm::vec3 & position, const glm::vec2 & scale)
	{
		s_Mask = Gear::TextureStorage::GetTexture2D(maskName);
		GR_ASSERT(s_Mask, "There is no mask image {0}", maskName);

		glm::vec2 maskScale = { s_Mask->GetWidth() / scale.x , s_Mask->GetHeight() / scale.y };
		s_MaskTranslate = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { maskScale.x, maskScale.y, 1.0f });
	}*/

	Terrain::Terrain(const InitiateData& initData)
	{
		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true);

		//Attach Component
		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Drawer, Gear::ComponentID::Transform
		});

		//Set Component specific
		Gear::EntitySystem::SetTransform(m_ID, glm::vec3(0.0f, 0.0f, -0.1f), 0.0f, glm::vec2(1.0f));

		//Gear::EntitySystem::Set

	}

}

