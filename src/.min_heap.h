#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <string>
#include <vector>

namespace heap {

class Heap {
 public:
  Heap(void) {}

  ~Heap(void) {}

  Heap(const Heap& other) = delete;

  Heap(const Heap&& other) = delete;

  Heap& operator=(const Heap& other) = delete;

  Heap& operator=(const Heap&& other) = delete;

  virtual void Heaplify(void) = 0;

  virtual void Adjust(void) = 0;
}; //end of class Heap

//string is the target station name, int32_t is current minimun length
//from source_station to target_station
typedef std::pair<std::string, int32_t>  distance_to_station_t;

class MinHeap : public Heap {
 public:
  static MinHeap* CreateHeap(
      const std::vector<distance_to_station_t>& pairs);

  ~MinHeap(void);

  virtual void Heaplify(void) final;

  virtual void Adjust(void) final;

 private:
  MinHeap(const std::vector<distance_to_station_t>& pairs);
}; //end of class MinHeap : public Heap

} //end of namespace heap

#endif
