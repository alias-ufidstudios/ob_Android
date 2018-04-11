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
    void makeVisual();
    void openShareIntent();
    void exit();

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

    bool bTakePhoto = false;

    string cacheDir = "unknown";
};


