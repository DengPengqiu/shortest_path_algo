#include "vertex.h"
#include "edge.h"

namespace graph_t {
using namespace std;

Edge::Edge(Vertex* const kFromVertex, Vertex* const kToVertex,
           const string& kLineName, const int kWeight) {
  from_vertex_  = kFromVertex;
  to_vertex_  = kToVertex;
  weight_     = kWeight;
  next_edge_  = nullptr;

  string name{kFromVertex->get_name()};
  name += "->";
  name += kToVertex->get_name();
  edge_name_ = {name};
  line_name_ = {kLineName};
} //end of Edge::Edge(kFromVertex, kToVertex, kLineName, kWeight)

Edge::~Edge(void) {
  from_vertex_ = to_vertex_ = nullptr;
  next_edge_ = nullptr;
}

} //end of namespace graph

