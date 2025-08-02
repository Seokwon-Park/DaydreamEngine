// Platform Detection
#if defined(_WIN32) // Windows x64/x86
	#if defined(_WIN64) // Windows x64
		#define Daydream_PLATFORM_WINDOWS
	#else // Windows x86 
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__) // IOS, MACOS
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define Daydream_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define Daydream_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__linux__) // LINUX
	#define Daydream_PLATFORM_LINUX
	#error "Linux is not supported!"
#else 
	#error "Unknown platform"
#endif