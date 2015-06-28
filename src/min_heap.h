#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <string>
#include <vector>
#include "heap.h"

namespace heap_t {
// string is the target station name, int32_t is current minimun length
// from source_station to target_station
typedef std::pair<std::string, int32_t>  distance_to_station_t;

class MinHeap : public Heap {
 public:
  static MinHeap* CreateHeap(
      const std::vector<distance_to_station_t>& pairs);

  ~MinHeap(void);

 public:
  virtual void Top();  // final

  virtual bool Empty(void);  // final

  virtual void Push(void);  // final

  virtual void Pop(void);  // final

 private:
  MinHeap(const std::vector<distance_to_station_t>& pairs);

  virtual void Heaplify(void);  // final

  virtual void Adjust(void);  // final

 private:
  heap_type_t type_;
}; //end of class MinHeap : public Heap

} //end of namespace heap_t

#endif

