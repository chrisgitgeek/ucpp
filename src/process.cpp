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

Process::Process(int pid){
  pid_=pid;
  user_ = LinuxParser::User(pid);
  command_ = LinuxParser::Command(pid);
  ram_= LinuxParser::Ram(pid);
  
}
// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  auto stats = LinuxParser::PidStat(pid_);
  auto total_time = stats[0] + stats[1]+stats[2]+stats[3];
  auto uptime = LinuxParser::UpTime();
  auto hertz = sysconf(_SC_CLK_TCK);
  auto seconds = uptime - (stats[4] / hertz);

  return 100 * ((total_time / hertz) / seconds); }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }