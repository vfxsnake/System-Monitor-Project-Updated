#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int init_pid) : pid(init_pid)
{
    user = LinuxParser::User(init_pid);
    command = LinuxParser::Command(init_pid);
    ram = LinuxParser::Ram(init_pid);
    up_time = LinuxParser::UpTime(init_pid);
    ComputeCpuUtilization();
}


int Process::Pid() { return pid; }

float Process::CpuUtilization() {return cpu_utilization;}

void Process::ComputeCpuUtilization()
{

    vector<string> cpu_stat_data = LinuxParser::CpuUtilization(pid);
    long process_time = 0;
    // the process tieme use is in the indext 13 to 16 from the stat file
    for(int i=13; i<=16; i++)
    {
        process_time += std::stol(cpu_stat_data[i]);
    }
    float process_in_seconds = (float)process_time / (float)sysconf(_SC_CLK_TCK);
    cpu_utilization =  process_in_seconds/ (float)up_time;
}

string Process::Command() { return command; }

string Process::Ram() { return ram; }

string Process::User() { return user; }

long Process::UpTime() { return up_time; }

bool Process::operator<(Process const& a) const 
{ 
    if(cpu_utilization < a.cpu_utilization)
        return true;
    return false;
}