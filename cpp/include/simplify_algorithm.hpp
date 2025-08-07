#pragma once
#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_SIMPLIFY_ALGORITHM
#define SIMPLIFY_ALGORITHM_PUBLIC __declspec(dllexport)
#else
#define SIMPLIFY_ALGORITHM_PUBLIC __declspec(dllimport)
#endif
#else
#ifdef BUILDING_SIMPLIFY_ALGORITHM
#define SIMPLIFY_ALGORITHM_PUBLIC __attribute__((visibility("default")))
#else
#define SIMPLIFY_ALGORITHM_PUBLIC
#endif
#endif

#include <vector>
#include <points.hpp>

namespace simplify
{
  std::vector<IPoint *> SIMPLIFY_ALGORITHM_PUBLIC simplify(const std::vector<IPoint *> &points, const double tolerance, const bool highQuality = false);
}
