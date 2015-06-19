#include "validate_input_parameters.h"
#include "graph.h"
#include "utility.h"
#include "dijkstra.h"
#include "bellman_ford.h"
#include "spfa.h"
#include "log_file_stream.h"

using namespace std;
using namespace validate;
using namespace graph_t;
using namespace utility;
using namespace algo;
using namespace log_stream;

static bool RunAlgorithm(const int kArg, const string& kArgv1,
                         const Graph::station_pair_t& kPair,
                         const Graph& kGraph,
                         vector<string>* path_to_end_station);

int main(const int kArgc, const char** kArgv) {
  // First validate the input parameters legality
  Graph::station_pair_t station_pair("", "");
  int result = EXIT_FAILURE;
  if (ValidateInputParameters(kArgc, kArgv, &station_pair)) {
    // For both C++11 and Pyton2.7.6 cannot support Linux's style(..) path
    // So do not use ~ and .. when setting the path
    string path_to_project_dir{ResolvePathToProjectDir(kArgv[0])};

    // Third, create Graph, default use linked_list and Dijkstra algorithm
    Graph* subway_system = Graph::CreateInstance(path_to_project_dir);
    // Forth, Run algorithm and write shortest_path_to_end_station to log file
    vector<string> path_to_end_station;
    if (RunAlgorithm(kArgc, kArgv[1], station_pair, *subway_system,
                     &path_to_end_station)) {
      LogFileStream* log_file = new LogFileStream(path_to_project_dir,
                                                  path_to_end_station);
      log_file->Output(subway_system);
      delete log_file;
      log_file = nullptr;

      result = EXIT_SUCCESS;
    } // end of if(RunAlgorithm(kArgc, kArgv[1], kPair, subway_system, ...)
    delete subway_system;
    subway_system = nullptr;
  } // end of if(ValidateInputParameters(kArgc, kArgv, &start_station, &end_station))
  return result;
} // end of int main(kArgc, kArgv)

static bool RunAlgorithm(const int kArgc, const string& kArgv1,
                         const Graph::station_pair_t& kPair,
                         const Graph& kGraph,
                         vector<string>* path_to_end_station) {
  // Do not forget check the start_station and end_station
  bool result = false;
  if (kGraph.SearchStationsByName(kPair)) {
    AlgoInterface* algo_instance = AlgoInterface::CreateInstance(kArgc, kArgv1,
                                                                 kPair, kGraph);
    if (algo_instance->Run(path_to_end_station)) {
      result = true;
    }
    // Do not forget free memory
    delete algo_instance;
    algo_instance = nullptr;
  } // end of if(kGraph->SearchStationsByName(kPair))
  return result;
} // end of static bool RunAlgorithm(kArgc, kArgv0, kPair, kGraph, path_to_end_station)

