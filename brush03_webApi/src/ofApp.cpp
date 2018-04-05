#define USE_DUMMY_DATA

#include "ofApp.h"

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
        handler.getDataFromServer();
    }
#endif

    JNIEnv * env = ofGetJNIEnv();
    jobject activity = ofGetOFActivityObject();
    jclass activityClass = env->FindClass("cc/openframeworks/brush03_webApi/OFActivity");

    // init
    jmethodID initBufferMethod = env->GetMethodID(activityClass,"initBuffer","(III)V");
    env->CallVoidMethod(activity,initBufferMethod, ofGetWidth(), ofGetHeight(), 4);
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
    ofEnableAntiAliasing();
    ofBackground(255);

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

    if(bTakePhoto){
        bTakePhoto = false;

        bool bSaveToStorage = false;
        if(bSaveToStorage) {
            string fileName = "/sdcard/DCIM/BrushCore/ob"
                              + ofToString(ofGetYear()) + ofToString(ofGetMonth())
                              + ofToString(ofGetDay()) + ofToString(ofGetHours())
                              + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds())
                              + ".png";
            ofSaveScreen(fileName);
        }else{
            JNIEnv * env = ofGetJNIEnv();
            jobject activity = ofGetOFActivityObject();
            jclass activityClass = env->FindClass("cc/openframeworks/brush03_webApi/OFActivity");
            jmethodID method = env->GetMethodID(activityClass,"startShare","()V");
            env->CallVoidMethod(activity, method);
        }
    }

    ofSetColor(0);
    ofDrawBitmapString(userToken, 50, 50);
    ofDrawBitmapString(ofGetFrameRate(),50, 75);

}

void ofApp::exit() {
}

void ofApp::shareFB() {

}

void ofApp::touchDown(int x, int y, int id){
    bTakePhoto = true;
}

void ofApp::touchMoved(int x, int y, int id){
}

void ofApp::touchUp(int x, int y, int id){
}

void ofApp::touchDoubleTap(int x, int y, int id){
}

void ofApp::touchCancelled(int x, int y, int id){
}

void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){
}

void ofApp::pause(){
}

void ofApp::stop(){
}

void ofApp::resume(){
}

void ofApp::reloadTextures(){

}

bool ofApp::backPressed(){
    return false;
}

void ofApp::okPressed(){

}

void ofApp::cancelPressed(){

}

extern "C" {
void
Java_cc_openframeworks_brush03_1webApi_OFActivity_fillBuffer(JNIEnv *env, jobject thiz, jbyteArray buffer) {

    jobject activity = ofGetOFActivityObject();
    jclass activityClass = env->FindClass("cc/openframeworks/brush03_webApi/OFActivity");

    ofApp &app = ofApp::get();

    // screenshot on C++
    ofPixels pix;
    //pix.allocate(ofGetScreenWidth(), ofGetScreenHeight(), 3);

    ofBaseRenderer * ren = ofGetCurrentRenderer().get();
    ofBaseGLRenderer * pren = static_cast<ofBaseGLRenderer*>(ren);
    pren->saveScreen(0,0,ofGetWindowWidth(),ofGetWindowHeight(), pix);

    // Get and Fill java buffer
    jboolean isCopy;
    unsigned char * frame =  (unsigned char*)env->GetByteArrayElements(buffer, &isCopy);
    {
        ofLogNotice() << "Is copy " << (isCopy ? "TRUE" : "FALSE");

        int ch = pix.getNumChannels();
        int w = ofGetWidth();
        int h = ofGetHeight();
        unsigned char *pixData = pix.getData();

        for(int i=0; i<h; i++) {
            for(int j=0; j<w; j++) {

                int id = (j+i*w) * ch;

                // write ARGB
                frame[id+0] = (jbyte)pixData[id+3]-125;
                frame[id+1] = (jbyte)pixData[id+0]-125;
                frame[id+2] = (jbyte)pixData[id+1]-125;
                frame[id+3] = (jbyte)pixData[id+2]-125;
            }
        }

    } env->ReleaseByteArrayElements(buffer, (jbyte*)frame, 0);

    jmethodID shareImageFB = env->GetMethodID(activityClass,"shareImageFacebook","()V");
    env->CallVoidMethod(activity,shareImageFB);
}

}
