#include "ofApp.h"
#include "obGeoDrawer.hpp"

void ofApp::setup(){

    ofSetFrameRate(targetFps);
    ofSetVerticalSync(true);
    ofSetWindowShape(1920, 1080);
    ofSetWindowPosition(0, 0);
    ofEnableAlphaBlending();
    ofDisableAntiAliasing();
    ofDisableSmoothing();

    bStart = true;

    // sound
    int bufferSize = 1920;
    int nCh = 1;
    int sampleRate = 48000;
    currentSamplePos = 0;
    prevSamplePos = 0;

    soundStream.setup(this, 0, nCh, sampleRate, bufferSize, 4);
    audioData.assign( soundStream.getBufferSize(), 0.0f );

    // visual
    int w = ofGetWindowWidth();
    int h = ofGetWindowHeight();
    bHandy = w<h;

    float longside = bHandy ? h:w;
    float shortside = bHandy ? w:h;
    canvas.set(0, 0, longside, shortside);

    track_len = longside * 0.9f;
    track_offset = longside * 0.05;
    start_point.x = track_offset;
    start_point.y = shortside/2;
    indicator.set(0, 0);

    // set update speed
    float speed_ms = track_len / (float)total_time_ms;
    indicator_speed = speed_ms * 1000.0f/(float)targetFps;

}


void ofApp::audioIn(float * input, int bufferSize, int nChannels){

    if( !bStart ) return;

    audioData.clear();
    for(int i=0; i<bufferSize; i++){
        float val = input[i];
        audioData.push_back( val * 1.8);
        currentSamplePos++;
    }
}

void ofApp::update(){

    if( !bStart ) return;

    float sampleRate = soundStream.getSampleRate();
    int totalSampleNum = total_time_ms/1000 * sampleRate;
    indicator.x = (float)currentSamplePos/totalSampleNum * track_len;

    if (indicator.x >= track_len) {
        cout << "tracking finished : " << ofGetElapsedTimef() << endl;
        //bStart = false;
    }

    prevSamplePos = currentSamplePos;
}

void ofApp::draw(){

    ofSetLineWidth(1);
    glPointSize(1);
    ofBackground(255);
    ofPushMatrix();{

        if(bHandy){
            ofRotateZ(90.0);
            ofTranslate(0, -ofGetWindowWidth());
        }

        ofTranslate(start_point);

        draw_bg();

        draw_wave();

    }ofPopMatrix();

    int y = 10;
    int os = 15;
    ofSetColor(0);
    ofDrawBitmapString("fps        " + ofToString(ofGetFrameRate()), 10, y);
    ofDrawBitmapString("nChannels  " + ofToString(soundStream.getNumInputChannels() ), 10, y+=os);
    ofDrawBitmapString("bufferSize " + ofToString(soundStream.getBufferSize()), 10, y+=os);
    ofDrawBitmapString("sampleRate " + ofToString(soundStream.getSampleRate()), 10, y+=os);

}

void ofApp::draw_wave(){
    // make vbo from audio
    if(audioData.size()!=0){

        ob::settings s;
        s.app = this;
        s.indicator = indicator;
        s.data = &audioData;
        s.track_len = track_len;
        s.bufferSize = soundStream.getBufferSize();
        s.xrate = track_len/s.bufferSize;

        int start = 0;
        const int end = s.bufferSize;
        float amp = canvas.height/2 * 0.8;
        bool loop = true;

        while( loop ){

            float n1 = ofNoise( ofGetDay(), ofGetElapsedTimef(), start );
            int type_max = 7;
            int type = round(n1 * type_max);

            float n2 = ofNoise( ofGetHours() , ofGetFrameNum()*2.0, start );
            n2 = pow(n2, 8) * ofRandom(1.0f,10.0f);

            int num_min = s.bufferSize * 0.01;
            int num_max = s.bufferSize * 0.05;
            int num = num_min + n2*num_max;

            if(type == 3) num*=0.25;

            if((start+num)>=end){
                num =  end-start-1;
                loop = false;
            }

            switch (type) {
                case 0:
                case 1: ob::draw_dot_wave(s, start, num, amp); break;
                case 2: ob::draw_prep_line(s, start, num, amp); break;
                case 3: ob::draw_circle(s, start, num, amp); break;
                case 4: ob::draw_rect(s, start, num, amp); break;
                case 5: ob::draw_log_wave(s, start, num, amp*0.01); break;
                case 6: ob::draw_arc(s, start, num, amp*0.5); break;
                case 7: ob::draw_prep_line_inv(s, start, num, amp/3); break;

                default: break;
            }
            start += num;
        }
    }
}

void ofApp::draw_bg(){

    ofSetColor(0, 0, 0);
    float yy = canvas.height/2*0.8;
    ofDrawLine(indicator.x, +yy+10, indicator.x, +yy);
    ofDrawLine(indicator.x, -yy-10, indicator.x, -yy);
    ofDrawLine(0, +yy+10, 0, +yy);
    ofDrawLine(0, -yy-10, 0, -yy);

    ofDrawLine(          0,    -5,         0,      +5);
    ofDrawLine(  track_len,    -5, track_len,      +5);

    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(255,255,0,255);
    ofFill();
    ofDrawRectangle(0, -yy, indicator.x, yy*2);

    // text sec
    ofSetColor(0);
    ofDrawBitmapString(ofToString(ofGetElapsedTimef()), indicator.x, yy+40);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){
    //string fileName = "screenshot/" + ofGetTimestampString() + ".png";
    string fileName= "/sdcard/DCIM/myFolder/ob"
    				+ ofToString(ofGetYear()) + ofToString(ofGetMonth())
    				+ ofToString(ofGetDay()) + ofToString(ofGetHours())
    				+ ofToString(ofGetMinutes()) + ofToString(ofGetSeconds())
    				+ ".png";
    ofSaveScreen(fileName);
}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}
