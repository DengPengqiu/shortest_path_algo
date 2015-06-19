#ifndef SPFA_H
#define SPFA_H

#include <queue>
#include "shortest_path_algo.h"
#include "graph.h"

namespace algo {

class SpfaAlgo : public AlgoInterface {
 public:
  SpfaAlgo(const graph_t::Graph& kGraph,
           const graph_t::Graph::station_pair_t& kPair, const size_t kCount);

  virtual ~SpfaAlgo(void);

  virtual bool Run(std::vector<std::string>* path_to_end_station) final;

 protected:
  SpfaAlgo(const SpfaAlgo& other) = delete;

  SpfaAlgo& operator=(const SpfaAlgo& other) = delete;

  SpfaAlgo(SpfaAlgo&& other) = delete;

  SpfaAlgo& operator=(SpfaAlgo&& other) = delete;

 private:
  void Initialize(graph_t::Vertex* source_vex);

  bool RunMainStep(void);

  void UpdateToVertexShortestPath(
      const graph_t::Vertex* const kToVertex, const int kCurrMinLen,
      const int kWeight, const std::vector<std::string>& kVexOnCurrPath);

 private:
  const graph_t::Graph& kGraph_;
  const size_t kMaxEnqueTimes_;
  const graph_t::Graph::station_pair_t& kStationPair_;
  std::unordered_map<std::string, int> distance_;
  std::map<std::string, std::vector<std::string>> shortest_path_;

  std::queue<graph_t::Vertex*> vex_queue_;
  // Once a Vertex push() into queue, set it true; if Vertex pop()
  // from queue, set it false
  std::unordered_map<graph_t::Vertex*, bool> visited_;
  std::unordered_map<graph_t::Vertex*, size_t> vex_enque_times_;
}; // end of class SpfaAlgo : public AlgoInterface

} // end of namespace algo

#endif
