#pragma once

namespace Gear {

	class Component 
	{
		friend class EntitySystem;
	protected:
		Component(int id)
			: m_ID(id)
		{}
		virtual ~Component() = default;

	private:
		virtual void Update(Timestep ts) = 0;

		inline void InActivate() { m_OnActivate = false; }
		inline void Activate() { m_OnActivate = true; }
		inline bool IsActivate() { return m_OnActivate; }

	protected:
		int m_ID;

	protected:
		bool m_OnActivate = true;
	};

}