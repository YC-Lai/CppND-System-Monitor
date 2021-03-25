#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float PrevTotal_{0};
  float PrevActive_{0};
  float Total_;
  float Active_;
  float Usage_;
};

#endif