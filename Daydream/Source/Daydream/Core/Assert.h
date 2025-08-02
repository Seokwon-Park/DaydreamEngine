#pragma once

#if defined(Daydream_DEBUG)
#define Daydream_ASSERT(x, ...) {if(!(x)) {Daydream_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define Daydream_CORE_ASSERT(x, ...) { if(!(x)) {Daydream_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
//#define Daydream_BREAK(...) { Daydream_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();  }

#else
#define Daydream_ASSERT(x, ...)
#define Daydream_CORE_ASSERT(x, ...)
#endif