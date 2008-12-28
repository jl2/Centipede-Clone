#include "utils.h"

#include <cstdlib>

double randDouble(double min, double max) {
  return (double(rand())/double(RAND_MAX))*(max-min)+min;
}
