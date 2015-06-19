#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <vector>
#include <map>

namespace graph_t {
class Edge;

class Vertex {
 public:
  explicit Vertex(const std::string& name);

  virtual ~Vertex(void);

  const std::string& get_name(void) const {
    return station_name_;
  }

  Edge* get_first_edge(void) const {
    return first_edge_;
  }

  void set_first_edge(Edge* const edge) {
    first_edge_ = edge;
  }

  void set_line_transfer_time(const std::map<std::string, int>& input) {
    line_transfer_time_map_ = input;
  }

  const std::map<std::string, int>& get_line_transfer_time(void) const {
    return line_transfer_time_map_;
  }

  void ConnectEdgeToVertex(Edge* const kEdge);

 protected:
  Vertex(const Vertex& other) = delete;

  Vertex& operator=(const Vertex& other) = delete;

  Vertex(Vertex&& other) = delete;

  Vertex& operator=(Vertex&& other) = delete;

 private:
  std::string  station_name_;
  Edge*  first_edge_;
  std::map<std::string, int> line_transfer_time_map_;
};  //end of class Vertex

} //end of namespace graph_t

#endif

