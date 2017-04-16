#ifndef UTILS_
#define UTILS_

#include <tuple>
#include <iostream>
#include <utility>

typedef std::tuple<int, int, int> XYZ;
typedef std::tuple<int, int> XZ;

long epoch_millis();
double millis_diff_to_fps(long millis);

int snap_to_chunk(int x);

template <typename Arg, typename... Args>
void print(Arg&& arg, Args&&... args)
{
    std::ostream& out = std::cout;
    out << std::forward<Arg>(arg);
    using expander = int[];
    (void)expander{0, (void(out << ", " << std::forward<Args>(args)),0)...};
    out << std::endl;
}

inline void wait_on_enter(){
    std::string dummy;
    std::cout << "Enter to continue..." << std::endl;
    std::getline(std::cin, dummy);
}

#endif
