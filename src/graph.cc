#include <cstdio>
#include <algorithm>
#include "graph.h"
#include "utility.h"

namespace graph_t {
using namespace std;
using namespace parse_file;
using namespace utility;

Graph* Graph::CreateInstance(const string& kPathToProjectDir) {
  subway_lines_t* line_stations_hash = new subway_lines_t;
  station_transfer_time_t* station_tranfer_time_hash =
    new station_transfer_time_t;
  ParseAllCsvFiles(kPathToProjectDir, line_stations_hash,
                   station_tranfer_time_hash);

  Graph* subway_system = new Graph();
  subway_system->Initialize(line_stations_hash, station_tranfer_time_hash);

  delete line_stations_hash;
  line_stations_hash = nullptr;
  delete station_tranfer_time_hash;
  station_tranfer_time_hash = nullptr;

  return subway_system;
} // end of static Graph* Graph::CreateInstance(const string& kPathToProjectDir)

Graph::~Graph(void) {
  size_t num_items = vertexs_->size();
  for (size_t i = 0; i < num_items; ++i) {
    delete vertexs_->at(i);
    vertexs_->at(i) = nullptr;
  }
  delete vertexs_;
  vertexs_ = nullptr;

  num_items = edges_->size();
  for (size_t i = 0; i < num_items; ++i) {
    delete edges_->at(i);
    edges_->at(i) = nullptr;
  }
  delete edges_;
  edges_ = nullptr;

  set<string>().swap(stations_);
} //end of Graph::~Graph(void)

Vertex* Graph::FindVertexByName(const string& kName) const {
  Vertex* target = nullptr;
  for (auto curr_vex : *vertexs_) {
    if (curr_vex->get_name() == kName) {
      target = curr_vex;
      break;
    }
  } // end of for(curr_vex* curr_vex : *vertexs_)
  return target;
} // end of Vertex* Graph::FindVertexByName(kName) const

Edge* Graph::FindEdgeByName(const string& kFromName,
                            const string& kToName) const {
  Edge* target = nullptr;
  if (kFromName.compare(kToName) != 0) {
    for (auto curr_edge : *edges_) {
      if (curr_edge->get_from_vertex()->get_name() == kFromName &&
          curr_edge->get_to_vertex()->get_name() == kToName) {
        target = curr_edge;
        break;
      } // end of if (== kFromName && == kToName)
    } // end of for(curr_edge* curr_edge : *edges_)
  } // end of if(kFromName != kToName)
  return target;
} // end of Edge* Graph::FindEdgeByName(kFromName, kToName)

void Graph::FindEdgesConnectToVertex(const string& kToVexName,
                                     vector<Edge*>* target_edges) const {
  target_edges->resize(0);
  for_each(edges_->cbegin(), edges_->cend(), [&](Edge* curr_edge) {
    if (kToVexName == curr_edge->get_to_vertex()->get_name()) {
      target_edges->push_back(curr_edge);
    }
  });
} //end of void Graph::FindEdgesConnectToVertex(kToVexName, target_edges) const

bool Graph::SearchStationsByName(const station_pair_t& kPair) const {
  return SearchStation(kPair.first) && SearchStation(kPair.second);
} //end of inline bool Graph::SearchStationsByName(kPair) const

Graph::Graph(void) { // protected
  vertexs_ = new vector<Vertex*>(0);
  edges_ = new vector<Edge*>(0);
} // end of Graph::Graph(void)

void Graph::Initialize(const subway_lines_t* kLineStationHash,
                       const station_transfer_time_t* kStationTransTimeHash) {
  InitialAllVexsAndEdges(kLineStationHash);

  InitAllVexTransTime(kStationTransTimeHash);
} // end of void Graph::Initialize(kLineStationHash, kStationTransTimeHash)

void Graph::InitialAllVexsAndEdges(const subway_lines_t* kLineStationHash) {
  const size_t kNumBuckets = kLineStationHash->bucket_count();
  for (size_t i = 0; i < kNumBuckets; ++i) {
    for (auto kIt = kLineStationHash->cbegin(i);
         kIt != kLineStationHash->cend(i); ++kIt) {
      const string& kLineName{kIt->first};

      int curr_minutes = 0;
      Vertex* prev_vex = nullptr;
      int minutes_prev_to_curr = 0;
      string station_name{""};
      const vector<string>& kStationInfos = kLineStationHash->at(kLineName);
      for (auto it = kStationInfos.cbegin(); it != kStationInfos.cend();
           ++it) {
        ResolveStationNameAndTime(*it, &station_name, &curr_minutes);
        Vertex* curr_vex = FindVertexByName(station_name);
        if (curr_vex == nullptr) {
          curr_vex = CreateVertex(station_name);
        }
        if (prev_vex != nullptr) {
          CreateTwoEdges(kLineName, minutes_prev_to_curr, curr_vex, prev_vex);
        } // end of if(prev_vex != nullptr)
        prev_vex = curr_vex;
        minutes_prev_to_curr = curr_minutes;
        stations_.insert(station_name);
      } // end of for(auto it = kStationInfos.cbegin(); it != .cend(); ++it)
    } // end of for(auto kIt = kLineStationHash->cbegin(i); != cend(i); ++kIt)
  } // end of for(size_t i = 0; i < kNumBuckets; ++i)
} // end of void Graph::InitialAllVexsAndEdges(kLineStationHash)

void Graph::InitAllVexTransTime(
    const station_transfer_time_t* kStationTransTimeHash) {
  const size_t kNumBuckets = kStationTransTimeHash->bucket_count();
  for (size_t i = 0; i < kNumBuckets; ++i) {
    for (auto kIt = kStationTransTimeHash->cbegin(i);
         kIt != kStationTransTimeHash->cend(i); ++kIt) {
      const string& kStationName{kIt->first};
      Vertex* curr_vex = FindVertexByName(kStationName);
      if (curr_vex != nullptr) {
        const line_transfer_time_map_t& curr_map{kIt->second};
        curr_vex->set_line_transfer_time(curr_map);
      }
    } //end of for(auto kIt = kStationTransTimeHash->cbegin(i); != cend(i); ++kIt)
  } //end of for(size_t i = 0; i < kNumBuckets; ++i)
} //end of void Graph::InitAllVexTransTime(kStationTransTimeHash)

inline Vertex* Graph::CreateVertex(const string& kStationName) {
  Vertex* vertex = new Vertex(kStationName);
  vertexs_->push_back(vertex);
  return vertex;
} //end of inline Vertex* Graph::CreateVertex(const string& kStationName)

void Graph::CreateTwoEdges(const string& kLineName, const int kWeight,
                           Vertex* curr_vex, Vertex* prev_vex) {
  // set double-directed edge, first was prev_vex-->curr_vex
  Edge* prev_to_curr = new Edge(prev_vex, curr_vex, kLineName, kWeight);
  edges_->push_back(prev_to_curr);
  prev_vex->ConnectEdgeToVertex(prev_to_curr);

  // then was curr_vex-->prev_vex
  Edge* curr_to_prev = new Edge(curr_vex, prev_vex, kLineName, kWeight);
  edges_->push_back(curr_to_prev);
  curr_vex->ConnectEdgeToVertex(curr_to_prev);
} // end of void Graph::CreateTwoEdges(kLineName, curr_vex, prev_vex)

inline bool Graph::SearchStation(const string& kName) const {
  bool result = true;
  if (stations_.find(kName) == stations_.cend()) {
    printf("Error! Can't find %s station. Please check your input.\n",
           kName.c_str());
    result = false;
  } // end of if(kIt == stations_.cend())
  return result;
} // end of inline bool Graph::SearchStation(kName) const

} // end of namespace graph_t

