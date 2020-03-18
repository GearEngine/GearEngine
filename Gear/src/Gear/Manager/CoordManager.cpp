#include "grpch.h"
#include "CoordManager.h"
#include "Gear/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLCoord2DManager.h"

namespace Gear {

	Coord2DManger* Coord2DManger::s_Inst = nullptr;

	Coord2DManger* Coord2DManger::Get()
	{
		GR_CORE_ASSERT(s_Inst, "Coord2DManager isn't initiated!");
		return s_Inst;
	}

	void Coord2DManger::Init()
	{
		GR_CORE_ASSERT(!s_Inst, "Coord2DManager aleady initiated!");

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			s_Inst = nullptr;
			return;
		case RendererAPI::API::OpenGL:
			s_Inst = new OpenGLCoord2DManager;
			return;
		}
		GR_CORE_ASSERT(false, "Unknown RendererAPI!");
		s_Inst = nullptr;
	}

	void Coord2DManger::Destroy()
	{
		if (s_Inst)
			delete s_Inst;
	}

	void Coord2DManger::SetResolution(float windowWidth, float windowHeight) 
	{ 
		m_WindowResolution = { windowWidth, windowHeight }; 
		m_AspectRatio = windowWidth / windowHeight;
	}

}