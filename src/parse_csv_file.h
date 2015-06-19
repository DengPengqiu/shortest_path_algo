#ifndef RESOVLE_LINES_AND_STATIONS_H
#define RESOVLE_LINES_AND_STATIONS_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

namespace parse_file {

typedef std::unordered_map<std::string, std::vector<std::string>>
  subway_lines_t; //<line_name, vector<stations>>

//<line-to-line, minutes>, such as <"13-to-2", 8>
//using line_transfer_time_map_t = std::map<string, string>
typedef std::map<std::string, int> line_transfer_time_map_t;

//such as <"xi_zhi_men", <"2-to-4", 2>>
typedef std::unordered_map<std::string, line_transfer_time_map_t>
  station_transfer_time_t;

void ParseAllCsvFiles(const std::string& kPathToProjectDir,
                      subway_lines_t* const line_stations_hash,
                      station_transfer_time_t* const transfer_time_hash);

void ParseLinesAndStations(const std::string& kPathToTargetFile,
                           subway_lines_t* const line_stations_hash);

void ParseStationTransferTimes(const std::string& kPathToTargetFile,
                               station_transfer_time_t* transfer_time_hash);
} //end of namespace parse_file

#endif

