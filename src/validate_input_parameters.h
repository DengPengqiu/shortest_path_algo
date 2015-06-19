#ifndef VALIDATE_INPUT_PARAMETERS_H
#define VALIDATE_INPUT_PARAMETERS_H

#include <string>
#include <cstdint>
#include "graph.h"

namespace validate {

bool ValidateInputParameters(const int kArgc, const char** kArgv,
                             graph_t::Graph::station_pair_t* station_pair);

} //end of namespace validate

#endif

