#ifndef PROFILER_H
#define PROFILER_H
#include <fstream>

struct ProfileResult {
  std::string name  = "Default";
  long long start   = 0;
  long long end     = 0;
  size_t threadID   = 0;
};

class Profiler {
  std::ofstream m_fout = std::ofstream("result.json");

  
  public:
  static Profiler& Instance ();
  void writeProfile (const ProfileResult& r);
    Profiler ();
    ~Profiler();
};

#endif