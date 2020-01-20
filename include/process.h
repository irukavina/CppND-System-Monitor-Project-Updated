#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  void RefreshData();
  long int UpTime();
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

 private:
  int pid_;
  float cpu_utilization_ = 0.0;
  long int prev_process_jiffies_ = 0;
  long int prev_system_jiffies_ = 0;
  std::string command_;
  std::string ram_;
  std::string user_;
  long int up_time_;

  float CalculateCpuUtilization();
};

#endif