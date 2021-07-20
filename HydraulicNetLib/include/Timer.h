
#ifndef HYDRAULICNETWORK_TIMER_H
#define HYDRAULICNETWORK_TIMER_H
#include <chrono>
#include <cassert>

using std::cerr;
namespace HydraulicNetwork {
    struct Timer {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        std::chrono::duration<float> duration;

        Timer() {
            start = std::chrono::high_resolution_clock::now();
        }
        ~Timer() {
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;

            float ms = duration.count() * 1000.0f;
            float s = duration.count();
            //  std::cerr << "Timer took in ms " << ms << "ms" << "\n";
            std::cerr << "Timer took in s " << s << "s"
                      << "\n";
        }
    };
}
#endif//HYDRAULICNETWORK_TIMER_H
