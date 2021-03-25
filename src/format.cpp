#include "format.h"

#include <iomanip>
#include <string>

#define SEC_IN_MIN 60
#define MIN_IN_HR 60
#define SEC_IN_HR (SEC_IN_MIN * MIN_IN_HR)

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hours, mins;
  hours = seconds / SEC_IN_HR;
  seconds %= SEC_IN_HR;
  mins = seconds / SEC_IN_MIN;
  seconds %= SEC_IN_MIN;

  std::ostringstream stream;
  stream << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2)
         << std::setfill('0') << mins << ":" << std::setw(2)
         << std::setfill('0') << seconds;
  return stream.str();
}