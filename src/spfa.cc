#include <climits>
#include <algorithm>
#include "spfa.h"
#include "graph.h"
#include "utility.h"

namespace algo {
using namespace std;
using namespace graph_t;
using namespace utility;

SpfaAlgo::SpfaAlgo(const Graph& kGraph,
                   const Graph::station_pair_t& kPair, const size_t kCount)
    : kGraph_(kGraph), kMaxEnqueTimes_(kCount), kStationPair_(kPair) {}

SpfaAlgo::~SpfaAlgo(void) {
  queue<Vertex*>().swap(vex_queue_);
  unordered_map<Vertex*, bool>().swap(visited_);
  unordered_map<Vertex*, size_t>().swap(vex_enque_times_);
} // end of SpfaAlgo::~SpfaAlgo(void)

bool SpfaAlgo::Run(vector<string>* path_to_end_station) {
  printf("Use SPFA Algorithm\n");
  bool result = false;
  Vertex* source_vex{kGraph_.FindVertexByName(kStationPair_.first)};
  if (source_vex != nullptr) {
    Initialize(source_vex);

    if (RunMainStep()) {
      *path_to_end_station = shortest_path_.at(kStationPair_.second);
      result = true;
    } // end of if(RunMainStep())
  } // end of if(source_vex != nullptr)
  return result;
} // end of bool SpfaAlgo::Run(path_to_end_station)

void SpfaAlgo::Initialize(Vertex* source_vex) {
  vector<string> initial_path(1, source_vex->get_name());

  vector<Vertex*>* kVertexs = kGraph_.get_vertexs();
  for_each(kVertexs->cbegin(), kVertexs->cend(), [&](Vertex* curr_vex) {
    const string& kCurrVexName{curr_vex->get_name()};
    distance_.insert(make_pair(kCurrVexName, INT_MAX));
    shortest_path_.insert(make_pair(kCurrVexName, initial_path));
    visited_.insert(make_pair(curr_vex, false));
    vex_enque_times_.insert(make_pair(curr_vex, 0));
  });

  // Then set for source_vertex
  vex_queue_.push(source_vex);
  visited_[source_vex] = true;
  distance_[source_vex->get_name()] = 0;
  ++vex_enque_times_[source_vex];
} // end of void SpfaAlgo::Initialize(source_vex)

bool SpfaAlgo::RunMainStep(void) {
  while (!(vex_queue_.empty())) {
    auto curr_vex = vex_queue_.front();
    vex_queue_.pop();
    visited_[curr_vex] = false;

    const string& kCurrVexName{curr_vex->get_name()};
    const vector<string>& kVexOnCurrPath{shortest_path_[kCurrVexName]};
    const string kPrevLine{FindOutPrevLine(kVexOnCurrPath, kCurrVexName,
                                           kGraph_)};

    const int kCurrMinLen = distance_[kCurrVexName];
    for (auto edge = curr_vex->get_first_edge(); edge != nullptr;
         edge = edge->get_next_edge()) {
      const string& kCurrLine{edge->get_line_name()};
      int weight = edge->get_weight();
      if (kPrevLine != "" && kPrevLine.compare(kCurrLine) != 0) {
        AddWeight(kPrevLine, kCurrLine, curr_vex, &weight);
      }

      const Vertex* const kToVertex = edge->get_to_vertex();
      UpdateToVertexShortestPath(kToVertex, kCurrMinLen, weight,
                                 kVexOnCurrPath);
    } // end of for(auto edge = get_first_edge(); edge != nullptr;)
  } // end while(!vex_queue.empty())
  return true;
} //end of bool SpfaAlgo::RunMainStep(kMaxEnqueTimes)

void SpfaAlgo::UpdateToVertexShortestPath(
    const Vertex* const kToVertex, const int kCurrMinLen,
    const int kWeight, const vector<string>& kVexOnCurrPath) {
  const string& kToVexName{kToVertex->get_name()};
  if (!visited_[const_cast<Vertex*>(kToVertex)] &&
      kCurrMinLen + kWeight < distance_[kToVexName]) {
    vex_queue_.push(const_cast<Vertex*>(kToVertex));
    visited_[const_cast<Vertex*>(kToVertex)] = true;
    if (++vex_enque_times_[const_cast<Vertex*>(kToVertex)] > kMaxEnqueTimes_) {
      perror("Attention, Graph existed negative edges\n");
      exit(EXIT_FAILURE);
    }
    distance_[kToVexName] = kCurrMinLen + kWeight;
    // then update for shortest_path_;
    vector<string>& path_need_update = shortest_path_[kToVexName];
    path_need_update = {kVexOnCurrPath};
    path_need_update.push_back(kToVexName);
  } // end of if(! && )
} // end of void SpfaAlgo::UpdateToVertexShortestPath(...)

} // end of namespace algo

