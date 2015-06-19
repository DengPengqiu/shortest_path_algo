#include "parse_csv_file.h"
#include "utility.h"
#include <fstream>

namespace parse_file {
using namespace std;
using namespace utility;

static void SplitLineAndStations(const string& kLine, string* curr_line,
                                 subway_lines_t* const line_stations_hash);

inline static void InitialCurrLine(const string& kLine, const size_t kLength,
                                   string* curr_line);

static void GetAllStationsInCsvLine(const string& kLine,
                                    const size_t kFirstCommaLoc,
                                    vector<string>* stations_and_time);

static void AssignLineTransferTimeMap(
    const vector<string>& kStrVec,
    line_transfer_time_map_t* line_transfer_time);

//=========================================================
void ParseAllCsvFiles(const string& kPathToProjectDir,
                      subway_lines_t* const line_stations_hash,
                      station_transfer_time_t* const transfer_time_hash) {
  string csv_file{"data/bei_jing_subway_stations.csv"};
  string path_to_csv_file{kPathToProjectDir + csv_file};
  ParseLinesAndStations(path_to_csv_file, line_stations_hash);

  csv_file = {"data/transfer_time.csv"};
  path_to_csv_file = {kPathToProjectDir + csv_file};
  ParseStationTransferTimes(path_to_csv_file, transfer_time_hash);
} //end of void ParseAllCsvFiles(kPathToProjectDir, line_stations_hash, transfer_time_hash)

void ParseLinesAndStations(const string& kPathToTargetFile,
                           subway_lines_t* const line_stations_hash) {
  ifstream input_stream(kPathToTargetFile, ifstream::in);
  if (!input_stream.is_open()) {
    printf("Fatal Error! Open %s failure\n", kPathToTargetFile.c_str());
    return;
  }

  constexpr uint8_t kBuffSize = 150;
  char buffer[kBuffSize];
  string line_str{""};
  string curr_line{""};
  while (!input_stream.eof()) {
    input_stream.getline(buffer, kBuffSize);
    line_str = {buffer};
    if (line_str.compare("") == 0) {
      continue;
    }

    SplitLineAndStations(line_str, &curr_line, line_stations_hash);
  } //end of while(input_stream.good() && !input_stream.eof())
  input_stream.close();
} //end of void ParseLinesAndStations(kPathToTargetFile, line_stations_hash)

static void SplitLineAndStations(const string& kLine, string* curr_line,
                                 subway_lines_t* const line_stations_hash) {
  //first, get the line info. if , isn't the first charctor in kLine,
  //then get curr_line. and all tmp_vector behind ","
  const auto kFirstCommaLoc = kLine.find_first_of(",", 0);
  if (kFirstCommaLoc != 0) { //It is a new line
    InitialCurrLine(kLine, kFirstCommaLoc, curr_line);
    //then get all stations and times in kLine
    vector<string> stations_and_time;
    GetAllStationsInCsvLine(kLine, kFirstCommaLoc, &stations_and_time);

    line_stations_hash->insert(make_pair(*curr_line, stations_and_time));
  } else { //all stations_and_time are prev_line
    vector<string>& stations_and_time = line_stations_hash->at(*curr_line);
    GetAllStationsInCsvLine(kLine, kFirstCommaLoc, &stations_and_time);
  }
} //end of static void SplitLineAndStations(kLine, curr_line, line_stations_hash)

inline static void InitialCurrLine(const string& kLine, const size_t kLength,
                                   string* curr_line) {
  *curr_line = {kLine.substr(0, kLength)};
} //end of inline static void InitialCurrLine(kLine, kLength, curr_line)

static void GetAllStationsInCsvLine(const string& kLine,
                                    const size_t kFirstCommaLoc,
                                    vector<string>* stations_and_time) {
  size_t next_comma = 0;
  auto curr_comma = kFirstCommaLoc;
  while (curr_comma != string::npos) {
    next_comma = kLine.find_first_of(",", curr_comma + 1);
    if (next_comma != string::npos && next_comma - curr_comma > 1) {
      size_t start_loc = curr_comma + 1;
      size_t len = next_comma - start_loc;
      if (len > 0) {
        stations_and_time->push_back(kLine.substr(start_loc, len));
      }
    } else {
      break;
    }
    curr_comma = next_comma;
  } //end of while(curr_comma != string::npos)

  //Do not forget push the last station in kLine
  if (next_comma == string::npos) {
    size_t start_loc = curr_comma + 1;
    size_t len = kLine.length() - start_loc;
    if (len > 0) {
      stations_and_time->push_back(kLine.substr(start_loc, len));
    }
  } //end of if(next_comma == string::npos)
} //static void GetAllStationsInCsvLine(kLine, kFirstCommaLoc, stations_and_time)

void ParseStationTransferTimes(const string& kPathToTargetFile,
                               station_transfer_time_t* transfer_time_hash) {
  ifstream input_stream(kPathToTargetFile, ifstream::in);
  if (!input_stream.is_open()) {
    printf("Fatal Error! Cannot open %s\n", kPathToTargetFile.c_str());
    return;
  }

  constexpr uint8_t kBuffSize = 64;
  char buffer[kBuffSize];
  string line{""};
  string station_name{""};
  while (!input_stream.eof()) {
    input_stream.getline(buffer, kBuffSize);
    line = {buffer};
    if (line.compare("") == 0) {
      continue;
    }

    vector<string> str_vec;
    SplitCsvLine(line, &str_vec);
    if (str_vec[0].compare("") != 0) {
      station_name = str_vec[0];
      line_transfer_time_map_t line_transfer_time;
      AssignLineTransferTimeMap(str_vec, &line_transfer_time);
      transfer_time_hash->insert(make_pair(station_name, line_transfer_time));
    } else {
      line_transfer_time_map_t& line_transfer_time =
        transfer_time_hash->at(station_name);
      AssignLineTransferTimeMap(str_vec, &line_transfer_time);
    } //end of else
  } //end of while(!input_stream.eof())
  input_stream.close();
} //end of void ParseStationTransferTimes(kPathToTargetFile, transfer_time_hash)

static void AssignLineTransferTimeMap(
    const vector<string>& kStrVec,
    line_transfer_time_map_t* line_transfer_time) {
  const string& kFromLine{kStrVec[1]};
  const string& kToLine{kStrVec[2]};
  const int kMinutes = stoi(kStrVec[3]);

  string line_to_line{MergeString(kFromLine, kToLine)};
  line_transfer_time->insert(make_pair(line_to_line, kMinutes));

  line_to_line = MergeString(kToLine, kFromLine);
  line_transfer_time->insert(make_pair(line_to_line, kMinutes));
} //end of static void AssignLineTransferTimeMap(kStrVec, line_transfer_time)

} //end of namespace parse_file

