#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid){};

int Process::Pid() { return pid_; }

float Process::CpuUtilization() const { return cpu_utilization_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return up_time_; }

bool Process::operator<(Process const& a) const {
  return cpu_utilization_ < a.cpu_utilization_;
}

bool Process::operator>(Process const& a) const {
  return cpu_utilization_ > a.cpu_utilization_;
}

void Process::RefreshData() {
  cpu_utilization_ = CalculateCpuUtilization();
  command_ = LinuxParser::Command(pid_);
  ram_ = LinuxParser::Ram(pid_);
  user_ = LinuxParser::User(pid_);
  up_time_ = LinuxParser::UpTime(pid_);
}

float Process::CalculateCpuUtilization() {
  long process_jiffies = LinuxParser::ActiveJiffies(pid_);
  long system_jiffies = LinuxParser::Jiffies();

  long delta_process_jiffies = process_jiffies - prev_process_jiffies_;
  long delta_system_jiffies = system_jiffies - prev_system_jiffies_;

  prev_process_jiffies_ = process_jiffies;
  prev_system_jiffies_ = system_jiffies;

  return (float)delta_process_jiffies / delta_system_jiffies;
}
