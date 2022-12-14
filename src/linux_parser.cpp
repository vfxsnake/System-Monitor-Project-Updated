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

vector<string> LinuxParser::CpuUtilization(int pid)
{
  vector<string> out_cpu_utilization_data;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) +kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    string data;
    std::istringstream linestream(line);
    while(linestream >> data)
    {
      out_cpu_utilization_data.emplace_back(data);
    }
  }
  stream.close();
  return out_cpu_utilization_data;
}

// helper function to renturn the value from a particular key.
// to be used under all search from a key in a line by line loop.
string FindValueByKeyInFile(const string &matching_key, const string &path_to_file)
{  
  string line;
  string key;
  string value = string();
  std::ifstream filestream(path_to_file);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> key >> value; 
      if (key == matching_key) 
        return value;
    }
  }
  return value;
  
}

int LinuxParser::TotalProcesses() 
{
  string value = FindValueByKeyInFile("processes", kProcDirectory + kStatFilename);
  if(value == string())
    return 0;
  return stoi(value);
}

int LinuxParser::RunningProcesses() 
{ 
  string value = FindValueByKeyInFile("procs_running", kProcDirectory + kStatFilename);
  if(value == string())
    return 0;
  return stoi(value);
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
  return string();
}

string LinuxParser::Ram(int pid) 
{
  string value = FindValueByKeyInFile("VmSize:", kProcDirectory + to_string(pid) + kStatusFilename);
  if(value == string())
    return string();
  float value_in_mb = stof(value) * 0.001;
  string val_str = to_string(value_in_mb);
  return val_str.substr(0, val_str.find(".")+4);
}

string LinuxParser::Uid(int pid) 
{ 
  return FindValueByKeyInFile("Uid:", kProcDirectory + to_string(pid) + kStatusFilename);
 
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
  
long LinuxParser::UpTime(int pid) 
{ 
  vector<string> pid_cpu_utilization_data = LinuxParser::CpuUtilization(pid);
  if(pid_cpu_utilization_data.size() > 22)
  {
    string up_time = pid_cpu_utilization_data[21];
    return  UpTime() - std::stol(up_time) / sysconf(_SC_CLK_TCK);
  }
  return 0;   
}
