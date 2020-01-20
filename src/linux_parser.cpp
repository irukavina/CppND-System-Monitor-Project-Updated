#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  long mem_total = stol(LinuxParser::ReadProcMeminfoItem("MemTotal:"));
  long mem_free = stol(LinuxParser::ReadProcMeminfoItem("MemFree:"));

  return (float)(mem_total - mem_free) / mem_total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  double sys_uptime;
  std::ifstream(kProcDirectory + kUptimeFilename, std::ios::in) >> sys_uptime;

  return static_cast<long>(sys_uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> stat_items =
      ReadProcStatItems(pid, vector<int>{13, 14, 15, 16});

  long utime = stol(stat_items[0]);
  long stime = stol(stat_items[1]);
  long cutime = stol(stat_items[2]);
  long cstime = stol(stat_items[3]);

  return utime + stime + cutime + cstime;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> cpu_utilization = CpuUtilization();
  long user = stoll(cpu_utilization[0]);
  long nice = stoll(cpu_utilization[1]);
  long system = stoll(cpu_utilization[2]);
  long irq = stoll(cpu_utilization[5]);
  long softirq = stoll(cpu_utilization[6]);
  long steal = stoll(cpu_utilization[7]);
  // long guest = stoll(cpu_utilization[8]);
  // long guest_nice = stoll(cpu_utilization[9]);
  return user + nice + system + irq + softirq + steal;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpu_utilization = CpuUtilization();
  long idle = stoll(cpu_utilization[3]);
  long iowait = stoll(cpu_utilization[4]);
  return idle + iowait;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream filestream(kStatPath, std::ios::in);
  string line;
  string token;
  getline(filestream, line);
  std::istringstream linestream(line);

  vector<string> result;
  linestream >> token;  // skip cpu name
  for (auto i = 0; i <= 10; i++) {
    linestream >> token;
    result.push_back(token);
  }

  return result;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return stoi(LinuxParser::ReadProcStatItem("processes"));
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return stoi(LinuxParser::ReadProcStatItem("procs_running"));
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string result;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename,
                           std::ios::in);
  std::getline(filestream, result);
  return result;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string vmsize = ReadProcStatusItem(pid, "VmSize:");
  unsigned long long vmsize_kb;
  std::istringstream(vmsize) >> vmsize_kb;

  return to_string(vmsize_kb / 1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { return ReadProcStatusItem(pid, "Uid:"); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line;
  string user;
  string token;
  string result;

  std::ifstream passwdFilestream(kPasswordPath, std::ios::in);
  while (std::getline(passwdFilestream, line)) {
    std::istringstream passwdRow(line);
    getline(passwdRow, user, ':');
    getline(passwdRow, token, ':');
    getline(passwdRow, token, ':');
    if (token == uid) {
      result = user;
      break;
    }
  }
  return result;
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  vector<string> proc_stat_items = ReadProcStatItems(pid, vector<int>{21});
  auto process_start_time = std::stoll(proc_stat_items[0]);
  process_start_time /= sysconf(_SC_CLK_TCK);

  return UpTime() - (long)process_start_time;
}

float LinuxParser::CpuUtilization(int pid) {
  long total_time_seconds = ActiveJiffies(pid) / sysconf(_SC_CLK_TCK);
  long uptime_seconds = UpTime(pid);

  float percentage = (double)total_time_seconds / uptime_seconds;
  return percentage;
}

vector<std::string> LinuxParser::ReadProcStatItems(unsigned int pid,
                                                   const vector<int>& ats) {
  vector<std::string> results;
  string token;
  std::ifstream proc_stream(kProcDirectory + to_string(pid) + kStatFilename,
                            std::ios::in);

  auto max_at = *std::max_element(ats.begin(), ats.end());
  vector<string> all_values;

  for (auto i = 0; i <= max_at; i++) {
    proc_stream >> token;
    all_values.push_back(token);
  }

  std::for_each(ats.begin(), ats.end(),
                [&](const int& at) { results.push_back(all_values[at]); });

  return results;
}

string LinuxParser::ReadProcStatusItem(int pid, string prefix) {
  string result;
  string row =
      ReadFileRow(kProcDirectory + to_string(pid) + kStatusFilename, prefix);
  std::istringstream(row) >> result;
  return result;
}

string LinuxParser::ReadProcMeminfoItem(string prefix) {
  string result;
  string row = ReadFileRow(kProcDirectory + kMeminfoFilename, prefix);
  std::istringstream(row) >> result;
  return result;
}

string LinuxParser::ReadProcStatItem(string prefix) {
  string result;
  string row = ReadFileRow(kProcDirectory + "stat", prefix);
  std::istringstream(row) >> result;
  return result;
}

string LinuxParser::ReadFileRow(string path, string prefix) {
  string line;
  std::ifstream filestream(path, std::ios::in);
  while (getline(filestream, line)) {
    if (line.substr(0, prefix.size()) == prefix) {
      return line.substr(prefix.size());
    }
  }
  return "0";
}