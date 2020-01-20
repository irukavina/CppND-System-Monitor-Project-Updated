#include <algorithm>
#include <map>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "system_processes.h"

using std::map;
using std::vector;

void SystemProcesses::RefreshData() {
  map<int, Process> itemsLookup;
  vector<int> newPids = LinuxParser::Pids();

  for (Process& item : items_) {
    itemsLookup.emplace(item.Pid(), item);
  }
  items_.clear();

  for (int newPid : newPids) {
    if (itemsLookup.count(newPid) > 0) {
      Process& item = itemsLookup.at(newPid);
      item.RefreshData();
      items_.push_back(item);
    } else {
      Process item(newPid);
      item.RefreshData();
      items_.push_back(item);
    }
  }

  std::sort(items_.begin(), items_.end(), std::greater<Process>());
}

vector<Process>& SystemProcesses::Items() { return items_; }