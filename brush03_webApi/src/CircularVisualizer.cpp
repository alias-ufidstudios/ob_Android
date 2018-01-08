#include "CircularVisualizer.h"
#include "ofApp.h"
#include "BrushColor.h"
#include "ofRange.h"

CircularVisualizer::CircularVisualizer(){
    
}

void CircularVisualizer::composePlotData(vector<BrushData> & data){
    
    vector<ofRange> ranges;
    
    // Hour
    for(int i=0; i<data.size(); i++){
        BrushData & s = data[i];
        
        float start = s.start.tm_min + s.start.tm_sec/60.0f;
        float end   = s.end.tm_min + s.end.tm_sec/60.0f;
        float startDeg = start/60.0f * 360.0f - 90.0f;
        float endDeg = end/60.0f * 360.0f - 90.0f;
        float startRad = ofDegToRad(startDeg);
        float endRad = ofDegToRad(endDeg);
        
        ofRange range(startDeg, endDeg);
        int intersect = 0;
        if(range.span() < 5.0f/60.0f*360.0f){
            for(int j=0; j<ranges.size(); j++){
                if(ranges[j].intersects(range)){
                    intersect++;
                }
            }
        }
        
        PlotData & p = s.plot[ob::plot::TYPE::HOUR];
        p.radius     = 0.3 * ofGetWidth()/2;
        p.size       = MAX(p.radius*0.01f, 4);  // thickness
        p.stAngle    = startDeg;
        p.endAngle   = endDeg;
        p.stPos.x    = p.radius * cos(startRad);
        p.stPos.y    = p.radius * sin(startRad);
        p.endPos.x   = p.radius * cos(endRad);
        p.endPos.y   = p.radius * sin(endRad);
        p.level      = intersect;
        p.color      = ob::color::hour;
        ranges.push_back(range);
    }
    
    // day
    for(int i=0; i<data.size(); i++){
        BrushData & s = data[i];
        PlotData & p = s.plot[ob::plot::TYPE::DAY];
        p.radius = 0.5 * ofGetWidth()/2;
        float start = s.start.tm_hour + s.start.tm_min/60.0f;
        p.stAngle   = p.endAngle = start/24.0f * 360.0f -90.0f;
        p.stPos.x   = p.radius * cos(ofDegToRad(p.stAngle));
        p.stPos.y   = p.radius * sin(ofDegToRad(p.stAngle));
        p.size      = ofGetWidth()*0.003f * s.duration/120.0f;
        p.color     = ob::color::day;
    }
    
    // week
    vector<int> wdayCnt;
    wdayCnt.assign(7, 0);
    
    for(int i=0; i<data.size(); i++){
        BrushData & s = data[i];
        PlotData & p = s.plot[ob::plot::TYPE::WEEK];
        int wday = s.start.tm_wday;
        float start = wday + s.start.tm_hour/23.0f;
        float startDeg = start/7.0f * 360.0f - 90.0;
        float startRad = ofDegToRad(startDeg);
        float baseRadius = 0.6 * ofGetWidth()/2;
        p.level = wdayCnt[wday];
        p.radius = baseRadius + 2.0f * wdayCnt[wday];
        
        p.stAngle   = p.endAngle = startDeg;
        p.stPos.x   = p.radius * cos(startRad);
        p.stPos.y   = p.radius * sin(startRad);
        p.size      = ofGetWidth()*0.003f * s.duration/120.0f;
        p.color     = ob::color::week;
        
        wdayCnt[wday]++;
    }
    
    // month
    vector<int> mdayCnt;
    mdayCnt.assign(31, 0);
    
    for(int i=0; i<data.size(); i++){
        BrushData & s = data[i];
        PlotData & p = s.plot[ob::plot::TYPE::MONTH];
        
        int mday = s.start.tm_mday - 1; // 0 - 30 days
        float start = mday + s.start.tm_hour/23.0f;
        float startDeg = start/31.0f * 360.0f - 90.0;
        float startRad = ofDegToRad(startDeg);
        float baseRadius = 0.7 * ofGetWidth()/2;
        p.radius = baseRadius + 3.0f * mdayCnt[mday];
        p.stAngle   = p.endAngle = startDeg;
        p.stPos.x   = p.radius * cos(startRad);
        p.stPos.y   = p.radius * sin(startRad);
        p.size      = ofGetWidth()*0.003f * s.duration/120.0f;
        p.color     = ob::color::month;
    }
    
    // year
    vector<int> ydayCnt;
    ydayCnt.assign(365, 0);
    
    for(int i=0; i<data.size(); i++){
        BrushData & s = data[i];
        PlotData & p = s.plot[ob::plot::TYPE::YEAR];
        
        int yday = s.start.tm_yday;
        float start = yday + s.start.tm_hour/23.0f;
        float startDeg = start/365.0f*360.0f - 90.0;
        float startRad = ofDegToRad(startDeg);
        float baseRadius = 0.8 * ofGetWidth()/2;
        p.radius = baseRadius + 4.0f * ydayCnt[yday];
        
        p.stAngle   = p.endAngle = startDeg;
        p.stPos.x   = p.radius * cos(startRad);
        p.stPos.y   = p.radius * sin(startRad);
        p.size      = ofGetWidth()*0.003f * s.duration/120.0f;
        p.color     = ob::color::year;
    }
}

void CircularVisualizer::draw_hour(float scale, const vector<BrushData> & data, int num){
    
    for(int i=0; i<num; i++){
        const BrushData & s = data[i];
        const PlotData & p = s.plot.at(ob::plot::TYPE::HOUR);
        float startDeg  = p.stAngle;
        float endDeg    = p.endAngle;
        
        int intersect   = p.level;
        float size      = p.size;
        float r         = p.radius + (2+size)*intersect;
        
        ofPath arc;
        arc.arc(0, 0, r, r, startDeg, endDeg);
        arc.arcNegative(0, 0, r+size, r+size, endDeg, startDeg);
        arc.close();
        arc.setCircleResolution(360);
        arc.setColor(p.color);
        arc.draw();
    }
}

void CircularVisualizer::draw_day(float scale, const vector<BrushData> & data, int num){
    
    for(int i=0; i<num; i++){
        const BrushData & s = data[i];
        const PlotData & p = s.plot.at(ob::plot::TYPE::DAY);
        float posx = p.stPos.x;
        float posy = p.stPos.y;
        float size = p.size;
        
        ofSetColor(p.color);
        ofDrawCircle(posx, posy, size);
    }
}

void CircularVisualizer::draw_week(float scale, const vector<BrushData> & data, int num){
    
    for(int i=0; i<num; i++){
        const BrushData & s = data[i];
        const PlotData & p = s.plot.at(ob::plot::TYPE::WEEK);
        float posx = p.stPos.x;
        float posy = p.stPos.y;
        float size = p.size;
        
        ofSetColor(p.color);
        ofDrawCircle(posx, posy, size);
    }
}

void CircularVisualizer::draw_month(float scale, const vector<BrushData> & data, int num){
    
    for(int i=0; i<num; i++){
        const BrushData & s = data[i];
        const PlotData & p = s.plot.at(ob::plot::TYPE::MONTH);
        float posx = p.stPos.x;
        float posy = p.stPos.y;
        float size = p.size;
        
        ofSetColor(p.color);
        ofDrawCircle(posx, posy, size);
    }
}

void CircularVisualizer::draw_year(float scale, const vector<BrushData> & data, int num){

    for(int i=0; i<num; i++){
        const BrushData & s = data[i];
        const PlotData & p = s.plot.at(ob::plot::TYPE::YEAR);
        float posx = p.stPos.x;
        float posy = p.stPos.y;
        float size = p.size;
        
        ofSetColor(p.color);
        ofDrawCircle(posx, posy, size);
    }
}
