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
int Process::Pid() { return pid_; }

float Process::CpuUtilization() { 
  auto stats = LinuxParser::PidStat(pid_);
  auto total_time = stats[0] + stats[1]+stats[2]+stats[3];
  auto uptime = LinuxParser::UpTime();
  auto hertz = sysconf(_SC_CLK_TCK);
  auto seconds = uptime - (stats[4] / hertz);

  return (total_time / hertz) / seconds; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }


bool Process::operator<(Process const& a) const { return true; }