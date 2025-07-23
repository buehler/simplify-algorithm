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

namespace simplify
{

  void SIMPLIFY_ALGORITHM_PUBLIC simplify();

  // class SIMPLIFY_ALGORITHM_PUBLIC Simplify_algorithm {

  // public:
  //   Simplify_algorithm();
  //   int get_number() const;

  // private:

  //   int number;

  // };

}
