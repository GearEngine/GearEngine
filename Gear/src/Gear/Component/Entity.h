#pragma once

namespace Gear {

	class Entity 
	{
	public:


	private:
		class SoundPlayer*	m_SoundPlayer = nullptr;
		class State*		m_State	= nullptr;
		class Transform*	m_Transform	= nullptr;
		class Animator*		m_Animator = nullptr;
		class Physics*		m_Physics = nullptr;
	};


}