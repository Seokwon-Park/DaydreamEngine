#pragma once

#ifdef Daydream_USE_DLL
	#ifdef Daydream_BUILD_DLL
		#define Daydream_API __declspec(dllexport)
	#else
		#define Daydream_API __declspec(dllimport)
	#endif
#else
#define Daydream_API 
#endif