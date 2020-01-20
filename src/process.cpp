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

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_utilization_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return up_time_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
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

Process Process::FromLinuxParser(int pid) {
  Process process(pid);
  process.RefreshData();
  return process;
}