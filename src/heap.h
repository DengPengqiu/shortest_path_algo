#ifndef HEAP_H
#define HEAP_H

namespace heap_t {

class Heap {
 public:
   typedef enum e_heap_type_t {
     MIN_HEAP = 0,
     MAX_HEAP
   } heap_type_t;

 public:
  virtual void Top(void) = 0; // override

  virtual bool Empty(void) = 0; // override

  virtual void Push() = 0; // override

  virtual void Pop() = 0; // override

 protected:
  Heap(void) {}

  virtual ~Heap(void) {}

  Heap(const Heap& other) = delete;

  Heap& operator=(const Heap& other) = delete;

  Heap(Heap&& other) = delete;

  Heap& operator=(Heap&& other) = delete;

 private:
  virtual void Heaplify(void) = 0;

  virtual void Adjust(void) = 0;
}; //end of class Heap

}  // end of namespace heap_t
#endif
