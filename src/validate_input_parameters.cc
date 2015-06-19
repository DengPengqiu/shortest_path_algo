#include <string>
#include <algorithm>
#include "validate_input_parameters.h"

namespace validate {
using namespace std;
using namespace graph_t;

static bool ValidateStationName(const string& kName);

static bool ValidateAlgoType(const string& kAlgoType);

static string ExtractStation(const int kArgc, const char** kArgv,
                             Graph::station_pair_t* station_pair);

bool ValidateInputParameters(const int kArgc, const char** kArgv,
                             Graph::station_pair_t* station_pair) {
  bool result = true;
  if (kArgc < 3 || kArgc > 4) {
    perror("You must input start and end station at least\n");
    result = false;
  } else {
    string algo_type{ExtractStation(kArgc, kArgv, station_pair)};

    if (station_pair->first.compare(station_pair->second) == 0) {
      perror("Error! Start and end station were same.\n");
      result = false;
    }
    result = result && ValidateStationName(station_pair->first) &&
             ValidateStationName(station_pair->second);
    if (kArgc == 4) {
      result = result && ValidateAlgoType(algo_type);
    }
  } // end of else
  return result;
} // end of const bool ValidateInputParameters(kArgc, kArgv)

static bool ValidateStationName(const string& kName) {
  int error_count = 0;
  for_each(kName.cbegin(), kName.cend(), [&](const char kC) {
    if (!(islower(kC) || kC == '_' || isdigit(kC) || kC == '[' || kC == ']')) {
      printf("Fatal Error! %c is illegal in station name.\n", kC);
      ++error_count;
    }
  });
  return error_count > 0 ? false : true;
} // end of static bool ValidateStationName(kName)

static bool ValidateAlgoType(const string& kAlgoType) {
  bool result = false;
  if ((kAlgoType == "-dij" || kAlgoType == "-dijk") ||
      (kAlgoType == "-bf" || kAlgoType == "-bell") ||
      (kAlgoType == "-sp" || kAlgoType == "-spfa")) {
    result = true;
  }
  return result;
} // end of static bool ValidateAlgoType(kAlgoType)

static string ExtractStation(const int kArgc, const char** kArgv,
                             Graph::station_pair_t* station_pair) {
  // ./a.out dong_zhi_men xi_zhi_men or ./a.out -spfa dong_zhi_men xi_zhi_men
  const int start_station_loc{(kArgc == 3) ? 1 : 2};
  station_pair->first = kArgv[start_station_loc];

  const int end_station_loc{(kArgc == 3) ? 2 : 3};
  station_pair->second = kArgv[end_station_loc];

  return (kArgc == 4) ? kArgv[1] : "";
}

} //end of namespace validate

