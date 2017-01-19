

#include <chrono>
using namespace std::chrono;

long epoch_millis() {
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

double millis_diff_to_fps(long millis) {
  return 1000./millis;

}
