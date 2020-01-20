#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  long prev_active_jiffies = 0;
  long prev_jiffies = 0;
};

#endif