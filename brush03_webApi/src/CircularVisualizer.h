#pragma once

#include "ofMain.h"
#include "BrushData.h"

class CircularVisualizer{
    
public:
    
    CircularVisualizer();
    
    void composePlotData(vector<BrushData> & data);

    void draw_hour(float radius, const vector<BrushData> & data, int num);
    void draw_day(float radius, const vector<BrushData> & data, int num);
    void draw_week(float radius, const vector<BrushData> & data, int num);
    void draw_month(float radius, const vector<BrushData> & data, int num);
    void draw_year(float radius, const vector<BrushData> & data, int num);

};
