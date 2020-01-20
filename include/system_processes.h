#ifndef SYSTEM_PROCESSES_H
#define SYSTEM_PROCESSES_H

#include <vector>
#include "process.h"

class SystemProcesses {
 public:
  void RefreshData();
  std::vector<Process>& Items();

 private:
  std::vector<Process> items_ = {};
};

#endif