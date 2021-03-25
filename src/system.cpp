#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  kernel_ = LinuxParser::Kernel();
  memUtil_ = LinuxParser::MemoryUtilization();
  osName_ = LinuxParser::OperatingSystem();
  totalProcess_ = LinuxParser::TotalProcesses();
  runningProcess_ = LinuxParser::RunningProcesses();
  upTime_ = LinuxParser::UpTime();
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  for (int pid : LinuxParser::Pids()) {
    Process p(pid);
    processes_.emplace_back(p);
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return memUtil_; }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return osName_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return runningProcess_; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return totalProcess_; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return upTime_; }