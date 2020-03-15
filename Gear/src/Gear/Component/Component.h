#pragma once

namespace Gear {

	class Component 
	{
		friend class EntitySystem;
	protected:
		Component() = default;
		virtual ~Component() = default;

	private:
		virtual void Update(Timestep ts) = 0;
		virtual void Render() {}

		inline void InActivate() { m_OnActivate = false; }
		inline void Activate() { m_OnActivate = true; }
		inline bool IsActivate() { return m_OnActivate; }

	private:
		bool m_OnActivate = true;
	};

}