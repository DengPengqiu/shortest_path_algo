#include "shortest_path_algo.h"
#include "dijkstra.h"
#include "bellman_ford.h"
#include "spfa.h"

namespace algo {
using namespace std;
using namespace graph_t;

AlgoInterface* AlgoInterface::CreateInstance(const int kArgc,
                                             const string& kArgv1,
                                             const Graph::station_pair_t& kPair,
                                             const Graph& kGraph) {
  algorithm_t algo_type = algorithm_t::DIJKSTRA;
  if (kArgc == 4) {
    algo_type = AlgoInterface::GetAlgoType(kArgv1);
  }
  // Find the shortest path from source_vertex to target_vertex
  AlgoInterface* shortest_path_algo = nullptr;
  if (algo_type == algorithm_t::DIJKSTRA) {
    shortest_path_algo = new DijkstraAlgo(kGraph, kPair);
  } else if (algo_type == algorithm_t::BELLFORD) {
    shortest_path_algo = new BellmanFordAlgo(kGraph, kPair);
  } else {
    shortest_path_algo = new SpfaAlgo(kGraph, kPair,
                                      kGraph.get_vertexs()->size() - 1);
  }

  return shortest_path_algo;
} // end of AlgoInterface* AlgoInterface::CreateInstance(...)

AlgoInterface::algorithm_t AlgoInterface::GetAlgoType(const string& kAlgoType) {
  algorithm_t algo_type = algorithm_t::DIJKSTRA;
  if (kAlgoType == "-bf" || kAlgoType == "-bell") {
    algo_type = algorithm_t::BELLFORD;
  }
  if (kAlgoType == "-sp" || kAlgoType == "-spfa") {
    algo_type = algorithm_t::SPFA;
  }

  return algo_type;
} // end of AlgoInterface::GetAlgoType(kAlgoType)

AlgoInterface::~AlgoInterface(void) {}
} // end of namespace algo
