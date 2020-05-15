#pragma once

namespace InGame {

	class Terrain
	{
	public:
		Terrain(const InitiateData& initData);
		~Terrain();

	private:
		int m_ID;
		Gear::Ref<Gear::Texture2D> map;
		Gear::Ref<Gear::Texture2D> mask;
	};

}
