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
    static ofApp & get();
    ofApp(){}

    void setup();
    void update();
    void draw();
    void makeVisual();

    //
    // ofApp_helper.cpp
    //
    string getExternalCacheDir();
    void openShareIntent();

    void touchDown(int x, int y, int id);
    void touchMoved(int x, int y, int id);
    void touchUp(int x, int y, int id);
    void touchDoubleTap(int x, int y, int id);
    void touchCancelled(int x, int y, int id);
    void swipe(ofxAndroidSwipeDir swipeDir, int id);
    void pause();
    void stop();
    void resume();
    void reloadTextures();
    bool backPressed();
    void okPressed();
    void cancelPressed();
    void exit();


    ofxAndroidWebView webView;
    vector<BrushData> data;
    BrushDataHandler handler;
    CircularVisualizer viz;
    Voro voro;
    Np np;
    
    bool bTakePhoto = false;
    string userToken = "aaa";
    string cacheDir = "unknown";
};


