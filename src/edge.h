#ifndef EDGE_H
#define EDGE_H

#include <vector>
#include <string>

namespace graph_t {
class Vertex;

class Edge {
 public:
  Edge(Vertex* const kFromVertex, Vertex* const kToVertex,
       const std::string& kLineName, const int kWeight);

  virtual ~Edge(void);

  Vertex* get_from_vertex(void) const {
    return from_vertex_;
  }

  Vertex* get_to_vertex(void) const {
    return to_vertex_;
  }

  Edge* get_next_edge(void) const {
    return next_edge_;
  }

  void set_next_edge(Edge* edge)  {
    next_edge_ = edge;
  }

  int get_weight(void) const {
    return weight_;
  }

  const std::string get_name(void) const {
    return edge_name_;
  }

  const std::string& get_line_name(void) const {
    return line_name_;
  }

 protected:
  Edge(const Edge& other) = delete;

  Edge& operator=(const Edge& other) = delete;

  Edge(Edge&& other) = delete;

  Edge& operator=(Edge&& other) = delete;

 private:
  Vertex*  from_vertex_;
  Vertex*  to_vertex_;
  int      weight_;
  Edge*    next_edge_;
  std::string  edge_name_; //"da_zhong_si-xi_zhi_men"
  std::string  line_name_; //"13", "4"
};  //end of class Edge

} //end of namespace graph_t

#endif
