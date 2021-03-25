#include "processor.h"

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  Active_ = (float)LinuxParser::ActiveJiffies();
  Total_ = (float)LinuxParser::Jiffies();
  Usage_ = (Active_ - PrevActive_) / (Total_ - PrevTotal_);
  PrevActive_ = Active_;
  PrevTotal_ = Total_;
  return Usage_;
}