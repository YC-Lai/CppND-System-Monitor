#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

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
// done
float LinuxParser::MemoryUtilization() {
  float totalmemory;
  float freememory;
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        totalmemory = stof(value);
      } else if (key == "MemFree:") {
        freememory = stof(value);
      }
    }
    return static_cast<float>(totalmemory - freememory) / totalmemory;
  }
  return 0;
}

// TODO: Read and return the system uptime
// done
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream uptime_stream(line);
    uptime_stream >> uptime;
    return stol(uptime);
  }

  return 0;
}

// TODO: Read and return the number of jiffies for the system
// done
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// DONE: Read and return the number of active jiffies for a PID
vector<string> LinuxParser::PidStat(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    string line{}, jiffy{};
    vector<string> contents{};
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (std::getline(linestream, jiffy, ' ')) {
      contents.emplace_back(jiffy);
    }
    return contents;
  }

  return {};
}

// Done: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> pid_stat = PidStat(pid);
  if (pid_stat.size() != 0) {
    return stol(pid_stat[kUtime_]) + stol(pid_stat[kStime_]) +
           stol(pid_stat[kCutime_]) + stol(pid_stat[kCstime_]);
  }

  return 0;  // in clock ticks
}

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto utilization = LinuxParser::CpuUtilization();
  if (utilization.size() != 0) {
    return stol(utilization[kUser_]) + stol(utilization[kNice_]) +
           stol(utilization[kSystem_]) + stol(utilization[kIRQ_]) +
           stol(utilization[kSoftIRQ_]) + stol(utilization[kSteal_]);
  }

  return 0;
}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto utilization = LinuxParser::CpuUtilization();
  if (utilization.size() != 0) {
    return stol(utilization[kIdle_]) + stol(utilization[kIOwait_]);
  }

  return 0;
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string jiffies_string;
  string cpu;
  vector<string> utilization;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream jiffies_stream(line);
    // jiffies_stream >> cpu;
    while (jiffies_stream >> jiffies_string) {
      utilization.push_back(jiffies_string);
    }
    return utilization;
  }
  return {};
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return value;
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
// done
int LinuxParser::RunningProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return value;
      }
    }
  }
  return 0;
}

// Done: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// Done: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string key, line, value;
  long ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        ram = stof(value);
        ram *= 0.001;
        return to_string(ram);
      }
    }
  }
  return string();
}

// Done: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string key, value, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return string();
}

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string user, x, uid, line;
  string target_uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid;
      if (uid == target_uid) {
        if (static_cast<int>(user.length()) <= 8) {
          return user;
        } else {
          user.resize(8);
          user.shrink_to_fit();
          return user + "..";
        }
      }
    }
  }
  return user;
}

// Done: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  vector<string> contents = PidStat(pid);
  if (contents.size() != 0) {
    return static_cast<long>(stol(contents[kStarttime_]) /
                             sysconf(_SC_CLK_TCK));  // in clock ticks
  }
  return 0;
}