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
    cpu_utilization = 0.5;
    ram = LinuxParser::Ram(init_pid);
    up_time = LinuxParser::UpTime(init_pid);
}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_utilization; }

// TODO: Return the command that generated this process
string Process::Command() { return command; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return up_time; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }