#pragma once

#ifdef STEINS_USE_DLL
	#ifdef STEINS_BUILD_DLL
		#define STEINS_API __declspec(dllexport)
	#else
		#define STEINS_API __declspec(dllimport)
	#endif
#else
#define STEINS_API 
#endif