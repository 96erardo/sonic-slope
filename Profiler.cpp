#include "Profiler.h"

Profiler::Profiler () {
  m_fout << "{";
  m_fout << "\"displayTimeUnit\": \"ms\",";
  m_fout << "\"traceEvents\":[";
}

Profiler::~Profiler () {
  long pos = m_fout.tellp();
  m_fout.seekp(pos - 1);
  m_fout.write(" ", 1);
  m_fout << "]}";
  m_fout.close();
}

Profiler& Profiler::Instance () {
  static Profiler instance;

  return instance;
}

void Profiler::writeProfile(const ProfileResult& r) {
  m_fout << "{";
  m_fout << "\"name\":" << "\"" << r.name << "\",";
  m_fout << "\"cat\":" << "\"function\",";
  m_fout << "\"ph\":" << "\"X\",";
  m_fout << "\"pid\":" << "0,";
  m_fout << "\"tid\":" << "123456,";
  m_fout << "\"dur\":" << (r.end - r.start) << ",";
  m_fout << "\"ts\":" << r.start;
  m_fout << "},";
}