#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include "log_file_stream.h"
#include "graph.h"

namespace log_stream {
using namespace std;

LogFileStream::LogFileStream(const string& kPathToProjectDir,
                             const vector<string>& kPathToEndStation)
    : path_to_log_file_(""), kPathToEndStation_(kPathToEndStation) {
  set_file_path(kPathToProjectDir);
}

void LogFileStream::Output(const graph_t::Graph* kGraph) const {
  ofstream output(path_to_log_file_.c_str(),
                  ofstream::out | ofstream::ate | ofstream::app);
  if (!output.is_open()) {
    perror("Write shortest path log file Failure!\n");
    return;
  }

  output << kPathToEndStation_.front(); // first output start_station
  string prev_vex_name{""};
  for_each(kPathToEndStation_.cbegin(), kPathToEndStation_.cend(),
    [&](const string& kCurrVexName) {
      if (prev_vex_name != "") {
        const auto kEdge = kGraph->FindEdgeByName(prev_vex_name, kCurrVexName);
        if (kEdge != nullptr) {
          output << "--(" + kEdge->get_line_name() + ")-->" + kCurrVexName;
        } // end of if(kEdge != nullptr)
      } // end of if(prev_vex_name != "")
      prev_vex_name = kCurrVexName;
  });
  output << endl;
  output.close();
} //end of void LogFileStream::Output(const Graph* kGraph) const

void LogFileStream::set_file_path(const string& kPathToProjectDir) {
  const string kLogFileDir{kPathToProjectDir + "result/"};
  if (access(kLogFileDir.c_str(), F_OK) == -1 &&
      mkdir(kLogFileDir.c_str(), S_IRWXU) == -1) {
    perror("Create result/ failure! Maybe you didn't had authority\n");
    return;
  }

  //then set for file name
  string tmp_file_name{kPathToEndStation_.front() + "-to-"};
  tmp_file_name += {kPathToEndStation_.back() + ".txt"};
  path_to_log_file_ = kLogFileDir + "/" + tmp_file_name;
} // end of void LogFileStream::set_file_path(const string& kPathToProjectDir)

} // end of namespace log_stream
