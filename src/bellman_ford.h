#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "shortest_path_algo.h"
#include "graph.h"

namespace algo {

class BellmanFordAlgo : public AlgoInterface {
 public:
  BellmanFordAlgo(const graph_t::Graph& kGraph,
                  const graph_t::Graph::station_pair_t& kPair);

  virtual ~BellmanFordAlgo(void);

  virtual bool Run(std::vector<std::string>* path_to_end_station) final;

 protected:
  BellmanFordAlgo(const BellmanFordAlgo& other) = delete;

  BellmanFordAlgo& operator=(const BellmanFordAlgo& other) = delete;

  BellmanFordAlgo(BellmanFordAlgo&& other) = delete;

  BellmanFordAlgo& operator=(BellmanFordAlgo&& other) = delete;

 private:
  void Initialize(const std::string& kSourceName);

  void RunMainStep(void);

  bool HadNegativeEdge(void);

  void FindOutShortestPathToEndStation(
      std::vector<std::string>* path_to_end_station) const;

private:
  const graph_t::Graph& kGraph_;
  const graph_t::Graph::station_pair_t& kStationPair_;

  std::unordered_map<std::string, int> distance_;
}; //end of class BellmanFordAlgo : public AlgoInterface

} //end of namespace algo

#endif
