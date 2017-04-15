
#include "../constants.h"
#include <iostream>
#include <utility>
#include "utils.h"

#include <chrono>
using namespace std::chrono;

long epoch_millis() {
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

double millis_diff_to_fps(long millis) {
  return 1000./millis;
}

int snap_to_chunk(int x){
  int mod;
  if (x < 0){
    mod = abs(x+1) % CHUNK_SIZE;
    mod = CHUNK_SIZE - mod - 1;
  } else {
    mod = x % CHUNK_SIZE;
  }
  return x - mod;
}
