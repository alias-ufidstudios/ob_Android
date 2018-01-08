#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include <unordered_map>

namespace ob{
namespace plot{

enum class TYPE{
    
    HOUR,
    DAY,
    WEEK,
    MONTH,
    YEAR
};
}
}

namespace std {
    template<> struct hash<ob::plot::TYPE> {
        size_t operator() (const ob::plot::TYPE &t) const { return size_t(t); }
    };
}

class PlotData{
    
public:
    PlotData(){}
    
    glm::vec2 stPos;
    glm::vec2 endPos;
    
    float stAngle;
    float endAngle;
    float radius;    // dist from center
    float size;      // circle size, arc thickness, etc
    int level;       // override level
    ofColor color;
};

class BrushData{
    
public:
    BrushData(){};
    int pressureCount;
    int pressureTime;
    int duration;
    
    std::tm start;
    std::tm end;

    unordered_map<ob::plot::TYPE, PlotData> plot =
    {
        {ob::plot::TYPE::HOUR,  PlotData()},
        {ob::plot::TYPE::DAY,   PlotData()},
        {ob::plot::TYPE::WEEK,  PlotData()},
        {ob::plot::TYPE::MONTH, PlotData()},
        {ob::plot::TYPE::YEAR,  PlotData()}
    };

    static void createData(ofxJSONElement & elem, vector<BrushData> & data);
    
    void print(){
        //cout << start << " - " << end << ", dur " << duration << ", pcnt " << pressureCount << ", pressureTIme " << pressureTIme << '\n';
    }
};
