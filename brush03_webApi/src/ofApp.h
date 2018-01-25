#pragma once

#include "ofxAndroidApp.h"
#include "CircularVisualizer.h"
#include "BrushDataHandler.h"
#include "BrushData.h"
#include "Voro.h"
#include "Np.h"
#include "ofxAndroidWebView.h"

class ofApp : public ofxAndroidApp{
    
public:
     ofApp(){ cout << "construct ofApp" << endl; }
    //~ofApp(){ cout << "destruct  ofApp" << endl; }

    void setup();
    void update();
    void draw();
    void exit();
    void makeVisual();
    void onResume();

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

    string userToken = "aaa";

    ofxAndroidWebView webView;
};


