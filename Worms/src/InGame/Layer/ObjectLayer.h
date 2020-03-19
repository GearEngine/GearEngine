#pragma once
#include "Gear.h"
#include "InGame/Entity/Object/Worm/Worm.h"
#include "InGame/Data/InitiateData.h"

namespace InGame {

	class ObjectLayer : public Gear::Layer
	{
	public:
		ObjectLayer(const InitiateData& initData);
		virtual ~ObjectLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Gear::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Gear::Event& e) override;

	private:
		std::vector<Gear::Ref<Worm>> worms;
	};

}
