#pragma once 
#include "CRenderer.h"
#include <chrono>
#include <thread>
#include <ratio>

//constexpr int fps{60}; // desired frame rate 
//tp = time point 
template <std::intmax_t FPS>
class frame_rater{
public: 
    frame_rater() :    //initialize the object keeping the pace 
    time_between_frames{1}, 
    tp{std::chrono::steady_clock::now()} {}

    void sleep(){
        tp += time_between_frames; // addition to time point 

        std::this_thread::sleep_until(tp); // and sleep util the next tp session
    }
private:
    std::chrono::duration<double, std::ratio<1, FPS>> time_between_frames;
    std::chrono::time_point<std::chrono::steady_clock, decltype(time_between_frames)> tp;
};
/*class frame_rater {
public:
    frame_rater(){};
    frame_limiter(){

        next_frame += std::chrono::milliseconds(100); 
        std::cout << std::time(0) << '\n'; 
        std::this_thread::sleep_until(next_frame);
    }
private:
    //std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    // std::chrono::system_clock::time_point lastFrame = std::chrono::system_clock::now();
    using clock = std::chrono::steady_clock;
    auto next_frame = clock::now();

};*/