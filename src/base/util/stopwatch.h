#ifndef UTIL_STOPWATCH
#define UTIL_STOPWATCH

#include <map>
#include <string>
#include <chrono>

#include "base/io/log.h"

struct TaggedTimePoint {
    const char* tag;
    std::chrono::high_resolution_clock::time_point start;

    TaggedTimePoint()
        : tag(nullptr)
    {
    }
    operator bool() {
        return tag;
    }
    void reset() {
        tag = nullptr;
        start = std::chrono::high_resolution_clock::time_point();
    }
};

template<typename T>
class Stopwatch {
template<typename B> friend class ElapsedTime;
public:
    Stopwatch(const char* src) {
        timePoint.tag = src;
        timePoint.start = std::chrono::high_resolution_clock::now();
        T(*this);
    }
    ~Stopwatch() {
        T();
    }

protected:
    TaggedTimePoint timePoint;
};

template<typename B>
class ElapsedTime {
public:
    ElapsedTime() {
        if (currentSw)
            measure(*currentSw);
    }
    template<class T>
    ElapsedTime(Stopwatch<T>& activeSw) {
        if (currentSw)
            measure(*currentSw);
        currentSw = &activeSw.timePoint;
    }

    static void start() {
        timeTable.clear();
        overallDuration = std::chrono::microseconds(0);
        overallStart = std::chrono::high_resolution_clock::now();
    }

    static int stop() {
        auto diff = std::chrono::high_resolution_clock::now() - overallStart;
        return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    }

    static void logDetails() {
        int callCount = 0;
        std::chrono::microseconds callDuration;
        for(auto& item : timeTable) {
            callCount += item.second.count;
            callDuration += item.second.time;
            IO::log() << item.first.c_str() << " --- " <<
                item.second.count << "/" <<
                item.second.time.count() << " us";
        }
        IO::log() << "Overall call time: " <<
            std::chrono::duration_cast<std::chrono::milliseconds>(callDuration).count();
        IO::log() << "Overall call count: " << callCount;
        IO::log() << "Overall time: " << stop();
    }

protected:
    struct item_t {
        unsigned count;
        std::chrono::microseconds time;
    };

    static TaggedTimePoint* currentSw;
    static std::chrono::microseconds overallDuration;
    static std::chrono::high_resolution_clock::time_point overallStart;
    static std::map<std::string, item_t> timeTable;

    void measure(TaggedTimePoint& ttp) {
        auto item = timeTable.find(ttp.tag);
        auto tmp = std::chrono::high_resolution_clock::now() - ttp.start;
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(tmp);
        if (item == timeTable.end())
            timeTable.insert(std::make_pair(ttp.tag, item_t{1, time}));
        else
            item->second.time += time, item->second.count++;
        overallDuration += time;
        if (currentSw)
            currentSw->reset();
        currentSw = nullptr;
    }    
};

template<typename B>
TaggedTimePoint* ElapsedTime<B>::currentSw = nullptr;
template<typename B>
std::chrono::microseconds ElapsedTime<B>::overallDuration;
template<typename B>
std::map<std::string, typename ElapsedTime<B>::item_t> ElapsedTime<B>::timeTable;
template<typename B>
std::chrono::high_resolution_clock::time_point ElapsedTime<B>::overallStart;

#endif //UTIL_STOPWATCH
