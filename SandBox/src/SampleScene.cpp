#include "SampleScene.h"

void SampleScene::Update(Gear::Timestep ts)
{
	for (auto& layer : m_LayerStack)
	{
		layer->OnUpdate(ts);
	}
}
