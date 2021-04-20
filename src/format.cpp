#include <string>

#include "format.h"
using std::to_string;
using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int h = seconds/60/60;
  seconds -= h *60*60;
    
    int m = seconds / 60;
    seconds -= m *  60;

    int s = seconds;
    seconds -= s ;
    return  to_string(h)+":" + to_string(m) + ":" + to_string(s) ;
  }