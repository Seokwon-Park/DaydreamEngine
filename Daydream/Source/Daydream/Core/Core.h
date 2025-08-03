#pragma once

#ifdef DAYDREAM_USE_DLL
	#ifdef DAYDREAM_BUILD_DLL
		#define DAYDREAM_API __declspec(dllexport)
	#else
		#define DAYDREAM_API __declspec(dllimport)
	#endif
#else
#define DAYDREAM_API 
#endif