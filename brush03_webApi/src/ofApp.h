#pragma once

//#include "ofxiOS.h"
#include "ofxAndroidApp.h"
#include "CircularVisualizer.h"
#include "BrushDataHandler.h"
#include "BrushData.h"
#include "Voro.h"
#include "Np.h"

class ofApp : public ofxAndroidApp{ //ofxiOSApp{
    
public:
     ofApp(){ cout << "construct ofApp" << endl; }
    ~ofApp(){ cout << "destruct  ofApp" << endl; }

    void setup();
    void update();
    void draw();
    void exit();
    void urlResponse(ofHttpResponse & response);
    void makeVisual();
    static ofApp & get(){
        static ofApp app;
        return app;
    }
      
    vector<BrushData> data;
    BrushDataHandler handler;

    CircularVisualizer viz;
    Voro voro;
    Np np;
    
    int num = 0;
};


