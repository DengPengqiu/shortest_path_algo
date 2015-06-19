#include <climits>
#include <algorithm>
#include "bellman_ford.h"
#include "utility.h"

namespace algo {
using namespace std;
using namespace graph_t;
using namespace utility;

BellmanFordAlgo::BellmanFordAlgo(const Graph& kGraph,
                                 const Graph::station_pair_t& kPair)
    : kGraph_(kGraph), kStationPair_(kPair) {}

BellmanFordAlgo::~BellmanFordAlgo(void) {
  unordered_map<string, int>().swap(distance_); //distance_.clear();
}

bool BellmanFordAlgo::Run(vector<string>* path_to_end_station) {
  printf("Use BellmanFord Algorithm\n");
  const string kStartStation{kStationPair_.first};
  Initialize(kStartStation);

  RunMainStep();

  bool result = false;
  if (!HadNegativeEdge()) {
    path_to_end_station->push_back(kStationPair_.second);

    FindOutShortestPathToEndStation(path_to_end_station);

    result = true;
  } //end of if(!HadNegativeEdge())
  return result;
} //end of bool BellmanFordAlgo::Run(path_to_end_station)

void BellmanFordAlgo::Initialize(const string& kSourceName) {
  const vector<Vertex*>* kVertexs = kGraph_.get_vertexs();
  for_each(kVertexs->cbegin(), kVertexs->cend(), [&](const Vertex* kCurrVex) {
    distance_.insert(make_pair(kCurrVex->get_name(), INT_MAX));
  });

  // the distance to source vertex initial as 0
  distance_.at(kSourceName) = 0;
} // end of void BellmanFordAlgo::InitBellmanFordAlgo(kStartStation, )

void BellmanFordAlgo::RunMainStep(void) {
  string prev_line{""};
  string prev_to_vex_name{""};
  const vector<Edge*>* kEdges = kGraph_.get_edges();
  const size_t kNumVertexs = kGraph_.get_vertexs()->size();
  for (size_t times = 0; times < kNumVertexs - 1; ++times) {
    size_t change_count = 0;
    for (const Edge* const kCurrEdge : *kEdges) {
      const string& kFromVexName{kCurrEdge->get_from_vertex()->get_name()};
      const string& kCurrLine{kCurrEdge->get_line_name()};
      int weight = kCurrEdge->get_weight();
      if (prev_line.compare("") != 0 && prev_line.compare(kCurrLine) != 0 &&
          prev_to_vex_name.compare(kFromVexName) == 0) {
        AddWeight(prev_line, kCurrLine, kCurrEdge->get_from_vertex(), &weight);
      }
      const string& kToVexName{kCurrEdge->get_to_vertex()->get_name()};
      if (distance_.at(kFromVexName) != INT_MAX &&
          distance_.at(kFromVexName) + weight < distance_.at(kToVexName)) {
        distance_[kToVexName] = distance_[kFromVexName] + weight;
        ++change_count;
      }
      prev_line = kCurrLine;
      prev_to_vex_name = kToVexName;
    } // end of for(const Edge* const kCurrEdge : *kEdges)

    if (change_count == 0) {
      break;
    }
  } // end of for(size_t times = 0; times < kNumVertexs - 1; ++times)
} // end of void BellmanFordAlgo::RunMainStep(void)

bool BellmanFordAlgo::HadNegativeEdge(void) {
  const vector<Edge*>* kEdges = kGraph_.get_edges();
  for (const auto kCurrEdge : *kEdges) {
    const string& kFromVexName{kCurrEdge->get_from_vertex()->get_name()};
    const string& kToVexName{kCurrEdge->get_to_vertex()->get_name()};
    const int kWeight = kCurrEdge->get_weight();
    if (distance_[kFromVexName] + kWeight < distance_[kToVexName]) {
      perror("Graph had negative edge\n");
      return true;
    }
  } // end of for(const auto kCurrEdge : *kEdges)

  return false;
} // end of bool BellmanFordAlgo::HadNegativeEdge(void)

void BellmanFordAlgo::FindOutShortestPathToEndStation(
    vector<string>* path_to_end_station) const {
  string to_vex_name{kStationPair_.second};
  string curr_min_vex_name{""};
  vector<Edge*> target_edges;
  while (true) {
    // first find all edges connect to to_vertex
    kGraph_.FindEdgesConnectToVertex(to_vex_name, &target_edges);

    // then compute the distance and get current shortest vertex name
    for (auto it = target_edges.cbegin(); it != target_edges.cend(); ++it) {
      const string& kFromVexName{(*it)->get_from_vertex()->get_name()};
      const int kWeight = (*it)->get_weight();
      if (distance_.at(kFromVexName) + kWeight == distance_.at(to_vex_name)) {
        curr_min_vex_name = {kFromVexName};
        break;
      } // end of if(distance_(from) + kWeight == distance_.at(to))
    } // end of for(auto kIt = cbegin(); kIt != target_edges.cend(); ++kIt)

    to_vex_name = curr_min_vex_name;
    path_to_end_station->push_back(to_vex_name);
    if (to_vex_name.compare(kStationPair_.first) == 0) {
      break; // had found all vertexs from start_station to end_station
    }
  } // end of while(true)
  reverse(path_to_end_station->begin(), path_to_end_station->end());
} // end of void BellmanFordAlgo::FindOutShortestPathToEndStation(...)

} // end of namespace BellmanFordAlgo

