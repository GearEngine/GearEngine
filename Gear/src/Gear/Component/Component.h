#pragma once

namespace Gear {

	class Component 
	{
	public:
		virtual ~Component() = default;

		inline void InActivate() { m_OnActivate = false; }
		inline void Activate() { m_OnActivate = true; }
		inline bool IsActivate() { return m_OnActivate; }

	private:
		bool m_OnActivate = true;
	};

}