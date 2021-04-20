#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;
// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key;
  long total, free;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  if(stream.is_open()) {
  	for(int i = 0 ;i< 2; i++){
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key>> (i == 0 ? total: free);
    }
  }
  return (total - free) / (float)total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptime;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if(stream.is_open()){
  	std::getline(stream,line);
    std::istringstream linestream(line);
    linestream>> uptime;
  }
 return uptime;
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

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() { 
  vector<string> res;
  string line;
  string key;
  int a,b,c,idle, rest;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream>> key >> a>>b>>c>>idle>>rest;    
   }
  float percent = (a+b + c) / (a+b +c + idle) *1.0;
  return percent; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)) {
    	std::replace(line.begin(), line.end(), ':', ' ');
    	std::istringstream linestream(line);
    	linestream>> key >> value;
    	if(key == "processes") {
    		break;
    	}
    }
  	
  }
  
  return value; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)) {
    	std::replace(line.begin(), line.end(), ':', ' ');
    	std::istringstream linestream(line);
    	linestream>> key >> value;
    }
  }
  return value;
   }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {   
  std::string line, cmd;
  std::ifstream stream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream>> cmd;
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  int uid;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)) {
    	std::replace(line.begin(), line.end(), ':', ' ');
    	std::istringstream linestream(line);
    	linestream>> key >> uid;
    	if(key == "Uid") {
    		break;
    	}
    }
  }
  return to_string(uid);

}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string uid = LinuxParser::Uid(pid);
  
  std::string line, user,x, value;

  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)) {
    	std::replace(line.begin(), line.end(), ':', ' ');
    	std::istringstream linestream(line);
    	linestream>> user >>x >> value;
    	if(value == uid) {
    		break;
    	}
    }
	}
  return user;
}
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    	
    for(int i =0;i <22; i++){
    	linestream>> value;
    }
  }  
  
  return UpTime() - stol(value) / sysconf(_SC_CLK_TCK);
   }