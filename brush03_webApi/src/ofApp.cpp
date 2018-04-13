//#define USE_DUMMY_DATA

#include "ofApp.h"

ofApp & ofApp::get(){
    static ofApp app;
    return app;
}

void ofApp::setup() {

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetCircleResolution(360);
    ofLogNotice("setup") << "width : " << ofGetWidth() << ", height : " << ofGetHeight();

    cacheDir = getExternalCacheDir();
    ofLogNotice() << "External File directory is -> " << cacheDir;
}

void ofApp::getData(){

#ifdef USE_DUMMY_DATA
    ofxJSONElement json = handler.getDataFromDummyFile("sessionExample.json");
    BrushData::createData(json, data);
    makeVisual();
    bDataok = true;
    bAccessingToCloud = false;
#else
    if(userToken=="aaa") {
        handler.getDataFromServer();
    }
#endif

}

void ofApp::update(){

    if(!bDataok){
        bNetok = ofxAndroidIsWifiOnline();// || ofxAndroidIsMobileOnline();
        if(bNetok && !bAccessingToCloud){
            getData();
            bAccessingToCloud = true;
        }
    }
}

void ofApp::draw() {

    ofEnableAntiAliasing();
    ofBackground(255);

    if(bDataok){
        float x = ofGetWidth() / 2;
        float y = ofGetHeight() / 2;
        float rad = ofGetWidth() / 2;
        int num = ofGetFrameNum() * 0.25f;
        num = MIN(data.size(), num);

        ofPushMatrix();
        {
            ofTranslate(x, y);

            ofFill();
            //viz.draw_hour   (rad * 0.4, data, num);
            viz.draw_day(rad * 0.6, data, num);
            viz.draw_week(rad * 0.7, data, num);
            viz.draw_month(rad * 0.8, data, num);
            viz.draw_year(rad * 0.9, data, num);

            voro.draw();
            np.draw();

            ofSetColor(150, 200);
            ofSetLineWidth(1);
            ofNoFill();
            ofDrawCircle(0, 0, rad * 0.4);

        }
        ofPopMatrix();

        if (bTakePhoto) {
            bTakePhoto = false;
            openShareIntent();
        }

        ofSetColor(0);
        ofDrawBitmapString(userToken, 50, 50);
        ofDrawBitmapString(ofGetFrameRate(), 50, 75);

    }else{
        string msg = "No Internet";
        ofSetColor(255,0,0);
        ofDrawBitmapString(msg, ofGetWidth()/2-50, 100);
        ofDrawLine(0,0, ofGetWidth(), ofGetHeight());
        ofDrawLine(ofGetWidth(), 0, 0, ofGetHeight());
    }
}

