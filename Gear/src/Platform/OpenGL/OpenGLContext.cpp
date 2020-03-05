#include "grpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Gear {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		GR_CORE_ASSERT(windowHandle, "Window hadle is null!")
	}

	void OpenGLContext::init()
	{
		GR_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GR_CORE_ASSERT(status, "Failed to initialize Glad!");
		GR_CORE_INFO("OpenGL Info:");
		GR_CORE_INFO("Vendor : {0}", glGetString(GL_VENDOR));
		GR_CORE_INFO("Renderer : {0}", glGetString(GL_RENDERER));
		GR_CORE_INFO("Version : {0}", glGetString(GL_VERSION));

	#ifdef GR_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		GR_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Gear requires at least OpenGL version 4.5!");
	#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		GR_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
