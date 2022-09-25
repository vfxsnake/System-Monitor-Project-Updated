#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:

  Process(int init_pid);
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
    int pid;
    std::string user;
    std::string command;
    float cpu_utilization;
    std::string ram;
    long int up_time;
};

#endif