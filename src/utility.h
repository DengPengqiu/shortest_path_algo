#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include "graph.h"

namespace utility {

std::string ResolvePathToProjectDir(const std::string& kArgv0);

void ResolveStationNameAndTime(const std::string& kStationInfo,
                               std::string* station_name,
                               int* curr_minutes);

void SplitCsvLine(const std::string& kLine, std::vector<std::string>* str_vec);

std::string MergeString(const std::string& kFromLine,
                        const std::string& kToLine);

void AddWeight(const std::string& kPrevLine, const std::string& kCurrLine,
               const graph_t::Vertex* const kTransVertex, int* weight);

std::string FindOutPrevLine(const std::vector<std::string>& kVexOnCurrPath,
                            const std::string& kCurrMinVexName,
                            const graph_t::Graph& kGraph);

} // end of namespace utility

#endif
