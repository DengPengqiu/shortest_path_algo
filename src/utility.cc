#include <unistd.h>
#include <cassert>
#include "utility.h"
#include "parse_csv_file.h"

namespace utility {
using namespace std;

inline static string GetCurrentWorkDir(void);

string ResolvePathToProjectDir(const string& kArgv0) {
  string curr_dir_path{GetCurrentWorkDir()};
  // then resvole path_to_project_dir by curr_dir_path and kArgv0
  const string kProjectDir{"shortest_path"};
  const size_t kLength = kProjectDir.length();
  string path_to_project_dir{""};
  // i.e.  ../../../cxx/shortest_path/bin/a.out wu_dao_kou bei_jing_zhan
  if (kArgv0.substr(0, 2).compare("..") == 0) {
    size_t start_loc = 0;
    while (kArgv0.find("..", start_loc) != string::npos) {
      auto last_splash_loc = curr_dir_path.find_last_of("/\\");
      curr_dir_path.erase(last_splash_loc);
      start_loc += 3;
    } // Erase all ../ string in kArgv0

    // for previous example, kArgv0SubStr is: cxx/shortest_path/bin/a.out
    const string kArgv0SubStr{kArgv0.substr(start_loc)};
    const auto kLoc = kArgv0SubStr.find(kProjectDir);
    if (kLoc != string::npos) {
      path_to_project_dir = curr_dir_path + "/" +
                            kArgv0SubStr.substr(0, kLoc + kLength) + "/";
    } else { // i.e. ../../bin/a.out xi_zhi_men dong_zhi_men
      path_to_project_dir = {curr_dir_path + "/"};
    }
  } else if (kArgv0.substr(0, 1).compare(".") == 0) {
    const size_t kLoc = kArgv0.find(kProjectDir);
    if (kLoc != string::npos) {
      // i.e. ./cxx/shortest_path/bin/a.out
      path_to_project_dir = curr_dir_path + kArgv0.substr(1, kLoc + kLength);
    } else {
      // i.e. ./bin/a.out
      path_to_project_dir = {curr_dir_path + "/"};
    }
  } // end of else if(kArgv0.substr(0, 1).compare(".") == 0)
  return move(path_to_project_dir);
} // end of string ResolvePathToProjectDir(kArgv0)

void ResolveStationNameAndTime(const string& kStationInfo, string* station_name,
                               int* curr_minutes) {
  const auto kLeftLoc = kStationInfo.find("("); //left "(" location
  if (kLeftLoc != string::npos) {
    size_t start_locate = 0;
    size_t len = kLeftLoc; //(kLeftLoc-1) - 0 + 1
    *station_name = {kStationInfo.substr(start_locate, len)};

    const auto kRightLoc = kStationInfo.find(")", kLeftLoc + 1);
    start_locate = kLeftLoc + 1;
    len = kRightLoc - kLeftLoc - 1; //(kRightLoc-1) - (kLeftLoc+1) + 1
    *curr_minutes = stoi(kStationInfo.substr(start_locate, len));
  } else {
    *station_name = {kStationInfo};
    *curr_minutes = 1;
  }
} //end of void ResolveStationNameAndTime(kStationInfo, station_name, minutes)

void SplitCsvLine(const string& kLine, vector<string>* str_vec) {
  size_t curr_comma = kLine.find(",", 0);
  size_t len = curr_comma;
  if (curr_comma == 0) {
    str_vec->push_back(string(""));
  } else {
    str_vec->push_back(kLine.substr(0, len)); //station_name
  }

  size_t prev_comma = curr_comma;
  size_t start_loc = prev_comma + 1;
  while ((curr_comma = kLine.find(",", start_loc)) != string::npos) {
    len = curr_comma - prev_comma - 1;
    str_vec->push_back(kLine.substr(start_loc, len));

    prev_comma = curr_comma;
    start_loc = prev_comma + 1;
  } //end of while(curr_comma != string::npos)

  if (curr_comma == string::npos) {
    len = kLine.length() - start_loc;
    str_vec->push_back(kLine.substr(start_loc, len));
  }
} //end of void SplitCsvLine(kLine, str_vec)

string MergeString(const string& kFromLine, const string& kToLine) {
  string line_transfer{kFromLine + "-"};
  line_transfer += kToLine; //such as "2-4", "4-2"
  return line_transfer;
} //end of void MergeString(kFromLine, kToLine, line_transfer)

void AddWeight(const string& kPrevLine, const string& kCurrLine,
               const graph_t::Vertex* const kTransVertex, int* weight) {
  string line_to_line{MergeString(kPrevLine, kCurrLine)};
  const parse_file::line_transfer_time_map_t& kCurrMap =
    kTransVertex->get_line_transfer_time();
  *weight += kCurrMap.at(line_to_line);
} //end of void AddWeight(kPrevLine, kCurrLine, kTransVertex, weight)

inline static string GetCurrentWorkDir(void) {
  constexpr uint8_t kBuffSize = 128;
  char buffer[kBuffSize];
  string curr_work_dir{getcwd(buffer, kBuffSize)};
  return curr_work_dir;
} //end of inline static string GetCurrentWorkDir(void)

string FindOutPrevLine(const vector<string>& kVexOnCurrPath,
                       const string& kCurrMinVexName,
                       const graph_t::Graph& kGraph) {
  const size_t kNumVertex = kVexOnCurrPath.size();
  const size_t kPrevVexIndex = (kNumVertex > 2) ? (kNumVertex - 2) : 0;
  const auto kPrevEdge = kGraph.FindEdgeByName(kVexOnCurrPath[kPrevVexIndex],
                                               kCurrMinVexName);

  return (kPrevEdge != nullptr) ? (kPrevEdge->get_line_name()) : "";
} //end of string FindOutPrevLine(kVexOnCurrPath, kCurrMinVexName, kGraph)

} //end of namespace utility

