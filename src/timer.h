#include <time.h>

class Timer {
    public:
        Timer(){
            startTime = clock();
        }

        float end(){
            return (clock() - startTime) / CLOCKS_PER_SEC;
        }

    private:
        float startTime;
};
