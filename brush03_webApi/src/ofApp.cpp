//#define USE_DUMMY_DATA

#include "ofApp.h"
#include <jni.h>

extern "C" {
JNIEXPORT void JNICALL Java_cc_openframeworks_brush03_1webApi_OFActivity_redirectFromWebAuth
        (JNIEnv *env, jobject obj, jstring token) {

    jboolean isCopy = false;
    const char *c = env->GetStringUTFChars(token, &isCopy);
    string ut = ofApp::get().userToken = string(c);
    ofLogNotice("redirectFromWebAuth : ") << ut;
    ofApp::get().handler.requestSessionData(ut);
}
}

void ofApp::setup() {

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetCircleResolution(360);
    ofLogNotice("setup") << "width : " << ofGetWidth() << ", height : " << ofGetHeight();

#ifdef USE_DUMMY_DATA
    ofxJSONElement json = handler.getDataFromDummyFile("sessionExample.json");
    BrushData::createData(json, data);
    makeVisual();
#else
    if(userToken=="aaa") {
        ofRegisterURLNotification(this);
        handler.getDataFromServer();
    }
#endif
}

void ofApp::makeVisual(){

    viz.composePlotData(data);

    float minRad, maxRad;
    minRad = 0;
    maxRad = TWO_PI;
    voro.addVertices(data, ob::plot::TYPE::HOUR, minRad, maxRad);
    voro.addVertices(data, ob::plot::TYPE::DAY,  minRad, maxRad);
    voro.addVertices(data, ob::plot::TYPE::WEEK, minRad, maxRad);
    voro.addVertices(data, ob::plot::TYPE::MONTH,minRad, maxRad);
    voro.addVertices(data, ob::plot::TYPE::YEAR, minRad, maxRad);

    voro.vPs.push_back(vPoint(0,0)); // put a vertex at center
    voro.create();
    
    minRad = 0;
    maxRad = TWO_PI;
    np.addVertices(data, ob::plot::TYPE::HOUR,  minRad, maxRad);
    np.addVertices(data, ob::plot::TYPE::DAY,   minRad, maxRad);
    np.addVertices(data, ob::plot::TYPE::WEEK,  minRad, maxRad);
    np.addVertices(data, ob::plot::TYPE::MONTH, minRad, maxRad);
    np.addVertices(data, ob::plot::TYPE::YEAR,  minRad, maxRad);
    np.create(2);
    
}

void ofApp::update(){
    num = ofGetFrameNum()*0.5f;
    num = MIN(data.size(), num);
}

void ofApp::draw(){
    ofBackground(255);

    ofSetColor(0);
    ofDrawBitmapString(userToken, 50, 50);

    float x = ofGetWidth()/2;
    float y = ofGetHeight()/2;
    float rad = ofGetWidth()/2;

    ofPushMatrix();{
        ofTranslate(x, y);

        ofFill();
        //viz.draw_hour   (rad * 0.4, data, num);
        viz.draw_day    (rad * 0.6, data, num);
        viz.draw_week   (rad * 0.7, data, num);
        viz.draw_month  (rad * 0.8, data, num);
        viz.draw_year   (rad * 0.9, data, num);

        voro.draw();
        np.draw();

        ofSetColor(150,200);
        ofSetLineWidth(1);
        ofNoFill();
        ofDrawCircle(0, 0, rad*0.4);

    }ofPopMatrix();
}

void ofApp::urlResponse(ofHttpResponse & response){
    string name = response.request.name;
    
    if(response.status==200){
        ofxJSONElement json;
        ofBuffer buf = response.data;
        string raw = ofToString(buf);
        json.parse(raw);
        
        if(name == "session data"){
            BrushData::createData(json, data);
            makeVisual();
        }
    }
}

void ofApp::onResume(){
    ofLogNotice("onResume") << "yay";
}

void ofApp::exit() {
    ofUnregisterURLNotification(this);
}
