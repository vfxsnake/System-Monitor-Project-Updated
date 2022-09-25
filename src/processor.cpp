#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "processor.h"
#include "linux_parser.h"


float Processor::Utilization() 
{
    float total_time_1 = 0;
    float total_time_2 = 0;
    float idle_time_1 = 0;
    float idle_time_2 = 0;
    // time sample 1
    ProcessDataSample(total_time_1, idle_time_1);
    // wait
    sleep(1);
    // time sample 2
    ProcessDataSample(total_time_2, idle_time_2);

    float total_time  = total_time_2 - total_time_1;
    float idle_time = idle_time_2 - idle_time_1;
    return (total_time - idle_time)/ total_time; 
}

void Processor::ProcessDataSample(float &total_time, float &idle_time) const
{
    std::vector<std::string> cpu_usage_data =  LinuxParser::CpuUtilization();
    // idle is the sum of idle + iowait (index 3 and 4) from the incoming data.
    idle_time = std::stof(cpu_usage_data[3]) + std::stof(cpu_usage_data[4]);
    for(std::string data : cpu_usage_data)
    {
        total_time += std::stof(data);
    }
    
}