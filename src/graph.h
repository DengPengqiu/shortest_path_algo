#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include "vertex.h"
#include "edge.h"
#include "parse_csv_file.h"

namespace graph_t {
class Vertex;
class Edge;

class Graph {
 public:
  //Don't use C-style enum, use scoped enum
  typedef enum class e_data_structure : uint8_t {
    LINKED_MATRIX = 0,
    LINKED_LIST
  } data_structure_t;
  //using graph_t = enum e_graph_type;

  //the first element is start_station name, the second one is end_station name
  typedef std::pair<std::string, std::string> station_pair_t;

 public:
  static Graph* CreateInstance(const std::string& kPathToProjectDir);

  virtual ~Graph(void);

  Vertex* FindVertexByName(const std::string& kName) const;

  Edge* FindEdgeByName(const std::string& kFromName,
                       const std::string& kToName) const;

  std::vector<Vertex*>* get_vertexs(void) const {
    return vertexs_;
  }

  std::vector<Edge*>* get_edges(void) const {
    return edges_;
  }

  void FindEdgesConnectToVertex(const std::string& kToVexName,
                                std::vector<Edge*>* target_edges) const;

  bool SearchStationsByName(const station_pair_t& kPair) const;

 protected:
  Graph(void);

  Graph(const Graph& other) = delete;

  Graph& operator=(const Graph& other) = delete;

  Graph(Graph&& other) = delete;

  Graph& operator=(Graph&& other) = delete;

 private:
  void Initialize(
      const parse_file::subway_lines_t* kLineStationHash,
      const parse_file::station_transfer_time_t* kStationTransTimeHash);

  void InitialAllVexsAndEdges(
    const parse_file::subway_lines_t* kLineStationHash);

  void InitAllVexTransTime(
    const parse_file::station_transfer_time_t* kStationTransTimeHash);

  inline Vertex* CreateVertex(const std::string& kName);

  void CreateTwoEdges(const std::string& kLineName, const int kWeight,
                      Vertex* curr_vex, Vertex* prev_vex);

  inline bool SearchStation(const std::string& kName) const;

 private:
  std::vector<Vertex*>* vertexs_;
  std::vector<Edge*>*   edges_;
  std::set<std::string> stations_;
}; //end of class Graph

} //end of namespace graph_t

#endif

