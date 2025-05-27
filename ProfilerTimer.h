#ifndef PROFILER_TIMER_H
#define PROFILER_TIMER_H
#include <iostream>

// #define PROFILING 1
#ifdef PROFILING
  #define PROFILE_SCOPE(name) \
    ProfilerTimer timer##__LINE__(name)
  #define PROFILE_FUNCTION() \
    PROFILE_SCOPE(__FUNCTION__)
#else
  #define PROFILE_SCOPE(name)
#endif

class ProfilerTimer {
  std::string m_name;
  long long m_start, m_end = 0;

  void start ();
  void end   ();
  
  public:
    ProfilerTimer (const std::string& name);  
    ~ProfilerTimer();
};

#endif