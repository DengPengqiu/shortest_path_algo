#ifndef LOG_FILE_STREAM_H
#define LOG_FILE_STREAM_H

#include <vector>
#include <string>
#include "graph.h"

namespace log_stream {

class LogFileStream {
 public:
  LogFileStream(const std::string& kPathToProjectDir,
                const std::vector<std::string>& kPathToEndStation);

  ~LogFileStream(void) {}

  void Output(const graph_t::Graph* kGraph) const;

  std::string get_file_path(void) const {
    return path_to_log_file_;
  }

 protected:
  LogFileStream(const LogFileStream& other) = delete;

  LogFileStream& operator=(const LogFileStream& other) = delete;

  LogFileStream(LogFileStream&& other) = delete;

  LogFileStream& operator=(LogFileStream&& other) = delete;

 private:
  void set_file_path(const std::string& kPathToProjectDir);

 private:
  std::string path_to_log_file_;
  const std::vector<std::string>& kPathToEndStation_;
}; // end of class LogFileStream

} //end of namespace log_stream

#endif
