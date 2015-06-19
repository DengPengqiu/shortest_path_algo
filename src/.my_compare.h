#ifndef MY_COMPARE_H
#define MY_COMPARE_H

#include <cstdint>

namespace compare {

class MyComparison {
 public:
  MyComparison(const bool reverse_param = false) {
    reverse_ = reverse_param;
  }

  bool operator() (const int32_t& lhs, const int32_t& rhs) {
    bool return_val = false;
    if (reverse_) {
      return_val = (lhs > rhs);
    } else {
      return_val = (lhs < rhs);
    }
    return return_val;
  }

 private:
   bool reverse_;
}; //end of class MyComparison

} //end of namespace compare

#endif
