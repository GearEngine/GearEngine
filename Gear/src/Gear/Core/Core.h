#pragma once

#include <memory>

//Platform detection using predefined macros
#ifdef _WIN32
	// Window x64/x86
	#ifdef _WIN64
		/* Window x64 */
		#define GR_PLATFORM_WINDOWS
	#else
		/* Window x86 */
		#error "x86 Builds are nont supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms 
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define GR_PLATFORM_IOS
		#error "IOS is not supported!"
	#else TARGET_OS_MAC == 1
		#define GR_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#endif	
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * if has __linux__ defined */
#elif defined(__ANDROID__)
	#define GR_PLATFORM_ANDROID
	#error "Android is not supported"
#elif defined(__linux__)
	#define GR_PLATFORM_LINUX
	#error "Linux is not supported"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection

// DLL support
#ifdef GR_PLATFORM_WINDOWS
#if GR_DYNAMIC_LINK
	#ifdef GR_BUILD_DLL
		#define GEAR_API __declspec(dllexport)
	#else
		#define GEAR_API __declspec(dllimport)
	#endif
#else
	#define GEAR_API
#endif
#else
	#error Gear only supports Windows!
#endif // End of DLL support

#ifdef GR_DEBUG
	#define GR_ENABLE_ASSERTS
#endif 

#ifdef GR_ENABLE_ASSERTS
	#define GR_ASSERT(x, ...)		{ if(!(x)) { GR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }		
	#define GR_CORE_ASSERT(x, ...)	{ if(!(x)) { GR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }		
#else	
	#define GR_ASSERT(x, ...)
	#define GR_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define GR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Gear {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	using EnumType = unsigned int;

	const unsigned int WINDOW_HEIGHT = 720;
	const unsigned int WINDOW_WIDTH	 = 1280;

}