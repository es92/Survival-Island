
#include <tuple>

typedef std::tuple<int, int, int> XYZ;

long epoch_millis();
double millis_diff_to_fps(long millis);

int snap_to_chunk(int x);
