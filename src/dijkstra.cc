#include <climits>
#include <algorithm>
#include "dijkstra.h"
#include "utility.h"

namespace algo {
using namespace std;
using namespace graph_t;
using namespace utility;

DijkstraAlgo::DijkstraAlgo(const Graph& kGraph,
                           const Graph::station_pair_t& kPair)
    : kGraph_(kGraph), kStationPair_(kPair) {}

DijkstraAlgo::~DijkstraAlgo(void) {
  unordered_map<string, bool>().swap(had_found_);
  unordered_map<string, int>().swap(distance_);
  map<string, vector<string>>().swap(shortest_path_);
} // end of DijkstraAlgo::~DijkstraAlgo(void)

bool DijkstraAlgo::Run(vector<string>* path_to_end_station) {
  printf("Use Dijkstra Algorithm\n");
  const Vertex* kSourceVex = kGraph_.FindVertexByName(kStationPair_.first);
  Initialize(kSourceVex);

  RunMainStep(kSourceVex);

  bool result = false;
  const string& kEndStation{kStationPair_.second};
  if (!(shortest_path_.at(kEndStation).empty())) {
    *path_to_end_station = shortest_path_.at(kEndStation);
    result = true;
  } // end of if(!kPathToEndStation.empty())
  return result;
} // end of bool DijkstraAlgo::RunDijkstra(shortest_path)

void DijkstraAlgo::Initialize(const Vertex* const kSourceVex) {
  const string& kSourceName{kSourceVex->get_name()};
  vector<string> initial_path(1, kSourceName);

  vector<Vertex*>* kVertexs = kGraph_.get_vertexs();
  for_each(kVertexs->cbegin(), kVertexs->cend(), [&](const Vertex* kCurrVex) {
    const string& kCurrVexName{kCurrVex->get_name()};
    if (kCurrVexName != kSourceName) {
      had_found_.insert(make_pair(kCurrVexName, false));
      distance_.insert(make_pair(kCurrVexName, INT_MAX));
      shortest_path_.insert(make_pair(kCurrVexName, initial_path));
    } // end of if(kCurrVexName != kSourceName)
  });

  // then initial those vertexs that connect to source vertex
  for (auto edge = kSourceVex->get_first_edge(); edge != nullptr;
       edge = edge->get_next_edge()) {
    const int kWeight = edge->get_weight();
    if (kWeight < INT_MAX) {
      Vertex* to_vertex = edge->get_to_vertex();
      distance_[to_vertex->get_name()] = kWeight;
    }
  } // end of for(auto edge = get_first_edge(); edge != nullptr;)
} // end of void DijkstraAlgo::InitDijkstraAlgo(kSourceVex)

void DijkstraAlgo::RunMainStep(const Vertex* const kSource) {
  const string& kSourceName{kSource->get_name()};
  const size_t kNumVertexs = kGraph_.get_vertexs()->size();
  for (size_t times = 0; times < kNumVertexs - 1; ++times) {
    // First find out the vertex which had minimum distance to source_vertex
    Vertex* curr_min_vertex = nullptr;
    int min_length = INT_MAX;
    FindOutCurrMinDistanceVex(kSourceName, &curr_min_vertex, &min_length);

    if (curr_min_vertex != nullptr) {
      // Found curr_min_vertex and update the 3 member parameters
      const string& kMinVexName{curr_min_vertex->get_name()};
      UpdateCurrMinDistanceVexStatus(kMinVexName, min_length);

      // update all vertexs connect to curr_min_vertex
      UpdateConnectedVexsShortestPath(curr_min_vertex, min_length);
    } // end of if(curr_min_vertex != nullptr)
  } // end for(size_t times = 0; times < kNumVertexs - 1; ++times)
} // end of void DijkstraAlgo::RunMainStep(kSource)

// Attention, this should use Vertex**, not Vertex*
void DijkstraAlgo::FindOutCurrMinDistanceVex(const string& kSourceName,
                                             Vertex** curr_min_vertex,
                                             int* min_length) const {
  vector<Vertex*>* kVertexs = kGraph_.get_vertexs();
  for (auto curr_vex : *kVertexs) {
    const string& kCurrVexName{curr_vex->get_name()};
    if (kCurrVexName != kSourceName && !had_found_.at(kCurrVexName) &&
        distance_.at(kCurrVexName) < *min_length) {
      *curr_min_vertex = curr_vex;
      *min_length = distance_.at(kCurrVexName);
    } // end of if(...)
  } // end of for(Vertex* curr_vex : *kVertexs)
} // end of void DijkstraAlgo::FindOutCurrMinDistanceVex(...) const

void DijkstraAlgo::UpdateCurrMinDistanceVexStatus(const string& kMinVexName,
                                                  const int kMinLength) {
  had_found_[kMinVexName] = true;
  distance_[kMinVexName] = kMinLength;
  // Don't forget append kMinVexName to its shortest_path
  vector<string>& path_to_min_vex = shortest_path_.at(kMinVexName);
  if (path_to_min_vex.back().compare(kMinVexName) != 0) {
    path_to_min_vex.push_back(kMinVexName);
  }
} // end of inline void DijkstraAlgo::UpdateCurrMinDistanceVexStatus(name,..)

void DijkstraAlgo::UpdateConnectedVexsShortestPath(const Vertex* kMinVertex,
                                                   const int kMinLength) {
  const string& kMinVexName{kMinVertex->get_name()};
  const vector<string>& kVexOnCurrPath{shortest_path_.at(kMinVexName)};
  const string kPrevLine{FindOutPrevLine(kVexOnCurrPath, kMinVexName, kGraph_)};
  // then update for those vertexs, which connected to kMinVertex
  for (auto edge = kMinVertex->get_first_edge(); edge != nullptr;
       edge = edge->get_next_edge()) {
    int weight = edge->get_weight();
    const string& kCurrLine{edge->get_line_name()};
    if (kPrevLine.compare(kCurrLine) != 0) {
      AddWeight(kPrevLine, kCurrLine, kMinVertex, &weight);
    }

    const string& kToVexName{edge->get_to_vertex()->get_name()};
    UpdateToVertexShortestPath(kToVexName, kMinLength, weight, kVexOnCurrPath);
  } // end of for(auto edge = kMinVexName->get_first_edge(); edge != nullptr;)
} // end of void DijkstraAlgo::UpdateConnectedVexsShortestPath(kMinVertex,...)

void DijkstraAlgo::UpdateToVertexShortestPath(
    const string& kToVexName, const int kMinLength, const int kWeight,
    const vector<string>& kVexOnCurrPath) {
  if (!had_found_[kToVexName] && kMinLength + kWeight < distance_[kToVexName]) {
    distance_[kToVexName] = kMinLength + kWeight;
    vector<string>& path_need_update = shortest_path_.at(kToVexName);
    path_need_update.assign(kVexOnCurrPath.cbegin(), kVexOnCurrPath.cend());
    path_need_update.push_back(kToVexName);
  } // end of if(!had_found_[kToVexName] &&)
} // end of void DijkstraAlgo::UpdateToVertexShortestPath(kToVexName,...)

} // end of namespace algo

