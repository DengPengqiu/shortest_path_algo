#include "vertex.h"
#include "edge.h"

namespace graph_t {
using namespace std;

Vertex::Vertex(const string& name) {
  station_name_ = name;
  first_edge_ = nullptr;
}

Vertex::~Vertex(void) {
  first_edge_ = nullptr;
}

void Vertex::ConnectEdgeToVertex(Edge* const kEdge) {
  if (get_first_edge() == nullptr) {
    set_first_edge(kEdge);
  } else {
    Edge* edge = get_first_edge();
    while (edge->get_next_edge() != nullptr) {
      edge = edge->get_next_edge();
    }
    edge->set_next_edge(kEdge);
  }
} // end of void Vertex::ConnectEdgeToVertex(kEdge)

} // end of namespace graph

