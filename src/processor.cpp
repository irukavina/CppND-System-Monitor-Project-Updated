#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long active_jiffies = LinuxParser::ActiveJiffies();
  long jiffies = LinuxParser::Jiffies();

  long delta_active_jiffies = active_jiffies - prev_active_jiffies;
  long delta_jiffies = jiffies - prev_jiffies;

  prev_active_jiffies = active_jiffies;
  prev_jiffies = jiffies;

  return (float)delta_active_jiffies / delta_jiffies;
}