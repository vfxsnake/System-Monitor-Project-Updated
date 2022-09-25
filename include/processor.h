#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
 void ProcessDataSample(float &total_time, float &idle_time) const; 
};

#endif