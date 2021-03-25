#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : _pid(pid) {
  _cmd = LinuxParser::Command(_pid);
  _memUtil = LinuxParser::Ram(_pid);
  _user = LinuxParser::User(_pid);
  _upt = LinuxParser::UpTime(_pid);
}

// TODO: Return this process's ID
int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  float total_time =
      (float)LinuxParser::ActiveJiffies(_pid) / sysconf(_SC_CLK_TCK);
  float seconds = LinuxParser::UpTime() -
                  ((float)LinuxParser::UpTime(_pid) / sysconf(_SC_CLK_TCK));

  return total_time / seconds;
}

// TODO: Return the command that generated this process
string Process::Command() { return _cmd; }

// TODO: Return this process's memory utilization
string Process::Ram() { return _memUtil; }

// TODO: Return the user (name) that generated this process
string Process::User() { return _user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return _upt; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return a.CpuUtilization() < this->CpuUtilization();
}