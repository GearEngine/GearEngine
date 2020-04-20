#pragma once

namespace InGame {

	class ItemRemainTimeDisplayHanlder : public Gear::Status::StatusHandler
	{
		bool inFirst = true;
		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::Texture2D> m_TimeTexture;
		Gear::Ref<Gear::Transform2D> m_Transform;
		Gear::Ref<Gear::FSM> m_FSM;
		glm::vec3 m_FontScale;
		glm::mat4 m_translate;

		void init(int entityID)
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			m_Transform = Gear::EntitySystem::GetTransform2D(entityID);
			m_TimeTexture = Gear::TextureStorage::GetTexture2D("ItemTimer");
			m_FontScale = glm::vec3(0.5f, 0.5f, ZOrder::z_Font);
			m_FSM = Gear::EntitySystem::GetFSM(entityID);
			m_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, ZOrder::z_Checker)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
		}

		inline void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statusList) override
		{
			if (inFirst)
			{
				init(entityID);
			}

			if (m_FSM->GetCurrentState() == Item::State::OnUnderWater)
			{
				data.Handled = true;
				return;
			}

			float remainTime = timer->GetRemainTime() + 0.9f;
			auto position = m_Transform->GetPosition();

			m_translate[3][0] = position.x - 0.4f;
			m_translate[3][1] = position.y + 0.4f;
			
			if (remainTime >= 3)
			{
				Gear::Renderer2D::DrawQuad(m_translate, m_TimeTexture);
				Font::PrintFont(glm::vec3(m_translate[3][0], m_translate[3][1], 1.0f), m_FontScale, std::to_string((int)remainTime), FontType::WhiteSmall, 0.02f, false);
			}
			else
			{
				Gear::Renderer2D::DrawQuad(m_translate, m_TimeTexture);
				Font::PrintFont(glm::vec3(m_translate[3][0], m_translate[3][1], 1.0f), m_FontScale, std::to_string((int)remainTime), FontType::RedSmall, 0.02f, false);
			}
			if (remainTime == 0)
			{
				data.Handled = true;
			}
		}
	};
}
