#pragma once

#ifdef GR_PLATFORM_WINDOWS

extern Gear::Application* Gear::CreateApplication();

int main(int argc, char** argv)
{
#ifdef GR_RELEASE
	HWND hWndConsole = GetConsoleWindow();
	ShowWindow(hWndConsole, SW_HIDE);
#endif

	Gear::Log::Init();

	GR_PROFILE_BEGIN_SESSION("Startup", "GearProfile-Startup.json");
	auto app = Gear::CreateApplication();
	GR_PROFILE_END_SESSION();

	GR_PROFILE_BEGIN_SESSION("Runtime", "GearProfile-Runtime.json");
	app->Run();
	GR_PROFILE_END_SESSION();

	GR_PROFILE_BEGIN_SESSION("Shutdown", "GearProfile-Shutdown.json");
	delete app;
	GR_PROFILE_END_SESSION();
}

#endif 