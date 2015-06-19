#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "shortest_path_algo.h"
#include "graph.h"

namespace algo {

class DijkstraAlgo : public AlgoInterface {
 public:
  DijkstraAlgo(const graph_t::Graph& kGraph,
               const graph_t::Graph::station_pair_t& kPair);

  virtual ~DijkstraAlgo(void);

  virtual bool Run(std::vector<std::string>* path_to_end_station) final;

 protected:
  DijkstraAlgo(const DijkstraAlgo& other) = delete;

  DijkstraAlgo& operator=(const DijkstraAlgo& other) = delete;

  DijkstraAlgo(DijkstraAlgo&& other) = delete;

  DijkstraAlgo& operator=(DijkstraAlgo&& other) = delete;

 private:
  void Initialize(const graph_t::Vertex* const kSource);

  void RunMainStep(const graph_t::Vertex* const kSource);

  void FindOutCurrMinDistanceVex(const std::string& kSourceName,
                                 graph_t::Vertex** curr_min_vertex,
                                 int* min_length) const;

  void UpdateCurrMinDistanceVexStatus(const std::string& kMinVexName,
                                      const int kMinLength);

  void UpdateConnectedVexsShortestPath(const graph_t::Vertex* kMinVertex,
                                       const int kMinLength);

  void UpdateToVertexShortestPath(
      const std::string& kToVexName, const int kMinLength, const int kWeight,
      const std::vector<std::string>& kVexOnCurrPath);

 private:
  const graph_t::Graph& kGraph_;
  const graph_t::Graph::station_pair_t& kStationPair_;
  //Once found the shortest path from kSource to curr_vex, set true.
  std::unordered_map<std::string, bool> had_found_;
  //Once found the shortest path from kSource to curr_vex, set min_distance
  std::unordered_map<std::string, int> distance_;

  std::map<std::string, std::vector<std::string>> shortest_path_;
}; //end of class DijkstraAlgo : public AlgoInterface

} //end of namespace algo

#endif

