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

	void Coord2DManger::init()
	{
		GR_CORE_ASSERT(!s_Inst, "Coord2DManager aleady initiated!");

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			s_Inst = nullptr;
		case RendererAPI::API::OpenGL:
			s_Inst = new OpenGLCoord2DManager;
		}
		GR_CORE_ASSERT(false, "Unknown RendererAPI!");
		s_Inst = nullptr;
	}

}