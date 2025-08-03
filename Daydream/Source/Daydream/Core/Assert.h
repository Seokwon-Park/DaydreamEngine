#pragma once

#if defined(DAYDREAM_DEBUG)
#define DAYDREAM_ASSERT(x, ...) {if(!(x)) {DAYDREAM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define DAYDREAM_CORE_ASSERT(x, ...) { if(!(x)) {DAYDREAM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
//#define DAYDREAM_BREAK(...) { DAYDREAM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();  }

#else
#define DAYDREAM_ASSERT(x, ...)
#define DAYDREAM_CORE_ASSERT(x, ...)
#endif