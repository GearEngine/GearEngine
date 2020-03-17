#pragma once

#include "grpch.h"

#include "Gear/Core/Core.h"
#include "Gear/Events/Event.h"

namespace Gear {
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Gear Engine",
			unsigned int width = WINDOW_WIDTH, unsigned int height = WINDOW_HEIGHT)
			: Title(title), Width(width), Height(height)
		{}
	};

	//Interface representing a desktop system based Window
	class GEAR_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}