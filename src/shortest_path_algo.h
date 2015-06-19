#ifndef SHORTEST_PATH_ALGO_H
#define SHORTEST_PATH_ALGO_H

#include <string>
#include <vector>
#include "graph.h"

namespace algo {

class AlgoInterface {
 public:
  typedef enum class e_algorithm_t {
    DIJKSTRA = 0,
    BELLFORD,
    SPFA,
    UNKNOWN
  } algorithm_t;

  static AlgoInterface* CreateInstance(
      const int kArgc, const std::string& kArgv1,
      const graph_t::Graph::station_pair_t& kPair,
      const graph_t::Graph& kGraph);

  // Due to clang++'s -Wweak-tables, I move destructor to .cc file
  virtual ~AlgoInterface(void);

  virtual bool Run(std::vector<std::string>* path_to_end_station) = 0;

 protected:
  AlgoInterface(void) {}

  AlgoInterface(const AlgoInterface& other) = delete;

  AlgoInterface& operator=(const AlgoInterface& other) = delete;

  AlgoInterface(AlgoInterface&& other) = delete;

  AlgoInterface& operator=(AlgoInterface&& other) = delete;

  static algorithm_t GetAlgoType(const std::string& kAlgoType);
}; // end of class AlgoInterface

} // end of namespace algo

#endif

