#pragma once

#if defined(STEINS_DEBUG)
#define STEINS_ASSERT(x, ...) {if(!(x)) {STEINS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define STEINS_CORE_ASSERT(x, ...) { if(!(x)) {STEINS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define STEINS_ASSERT(x, ...)
#define STEINS_CORE_ASSERT(x, ...)
#endif