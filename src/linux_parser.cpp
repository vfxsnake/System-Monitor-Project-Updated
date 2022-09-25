#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

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

float LinuxParser::MemoryUtilization() 
{ 

  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) 
  {
    float total_memory = -1;
    float free_memory = -1;
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "MemTotal:") 
          total_memory = stof(value);
        if (key ==  "MemFree:")
          free_memory = stof(value);  
    }
    return (total_memory - free_memory)/total_memory;
  }
  return 0;
}


long LinuxParser::UpTime() 
{ 
  string uptime_in_seconds;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_in_seconds;
    return std::stol(uptime_in_seconds);
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization()
{
  vector<string> out_cpu_utilization_data;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    string data;
    std::istringstream linestream(line);
    while(linestream >> data)
    {
      if (data == "cpu")
        continue;
      out_cpu_utilization_data.emplace_back(data);
    }
  }
  return out_cpu_utilization_data;
}

int LinuxParser::TotalProcesses() 
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> key >> value; 
      if (key == "processes") 
        return stoi(value);
    }
  }
  return 0;
}


int LinuxParser::RunningProcesses() 
{ 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> key >> value; 
      if (key == "procs_running") 
        return stoi(value);
    }
  }
  return 0;
}


string LinuxParser::Command(int pid) 
{
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) 
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    return value;
  }
  return string(" ");
}


string LinuxParser::Ram(int pid) 
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> key >> value; 
      if (key == "VmSize:")
      {
        float value_in_mb = stof(value) * 0.001;
        string val_str = to_string(value_in_mb);
        return val_str.substr(0, val_str.find(".")+4);
      }
    }
  }
  return string(" ");
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> key >> value; 
      if (key == "Uid:")
        return value;
    }
  }
  return string(" ");
}

string LinuxParser::User(int pid) 
{
  string uid = Uid(pid);
  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      // replace : by ' ' 
      std::replace(line.begin(), line.end(), ':', ' ');
      string key;
      string x_;
      string value;
      std::istringstream linestream(line);
      linestream >> key >> x_ >> value; 
      if (value == uid)
        return key;
    }
  }
    return string(" ");
}
  
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  vector<string> pid_cpu_utilization_data;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) +kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    string data;
    std::istringstream linestream(line);
    while(linestream >> data)
    {
      pid_cpu_utilization_data.emplace_back(data);
    }

    if(pid_cpu_utilization_data.size() > 22)
    {
      string up_time = pid_cpu_utilization_data[22];
      return  std::stol(up_time) / sysconf(_SC_CLK_TCK);
    }
  }
  return 0;
   
}
