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
    soundStream.printDeviceList();
    bufferSize = 1920;
    nCh = 1;
    sampleRate = 48000;
    currentSamplePos = 0;
    prevSamplePos = 0;
    soundStream.setup(this, 0, nCh, sampleRate, bufferSize, 4);
    audioData.assign( bufferSize, 0.0f );

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

    // vbo
    vPoints.setUsage(GL_DYNAMIC_DRAW);
    vLines.setUsage(GL_DYNAMIC_DRAW);
    vPoints.setMode(OF_PRIMITIVE_POINTS);
    vLines.setMode(OF_PRIMITIVE_LINES);

    vAudio.setUsage(GL_DYNAMIC_DRAW);
    vAudio.setMode(OF_PRIMITIVE_POINTS);

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

    int totalSampleNum = total_time_ms/1000 * sampleRate;
    indicator.x = (float)currentSamplePos/totalSampleNum * track_len;

    // vbo clear
    vPoints.clearVertices();
    vPoints.clearColors();
    vLines.clearVertices();
    vLines.clearColors();

    // make vbo from audio
    if(audioData.size()!=0){

        ob::settings s;
        s.app = this;
        s.indicator = indicator;
        s.data = &audioData;
        s.vp = &vPoints;
        s.vl = &vLines;
        s.track_len = track_len;
        s.bufferSize = bufferSize;
        s.xrate = track_len/bufferSize;

        int start = 0;
        const int end = bufferSize;
        float amp = canvas.height/2 * 0.8;
        bool loop = true;

        while( loop ){

            float n1 = ofNoise( ofGetDay(), ofGetElapsedTimef(), start );
            int type_max = 7;
            int type = round(n1 * type_max);

            float n2 = ofNoise( ofGetHours() , ofGetFrameNum()*2.0, start );
            n2 = pow(n2, 8) * ofRandom(1.0f,10.0f);

            int num_min = bufferSize * 0.01;
            int num_max = bufferSize * 0.05;
            int num = num_min + n2*num_max;

            if(type == 3) num*=0.25;

            if((start+num)>=end){
                num =  end-start-1;
                loop = false;
            }
            switch (type) {
                case 0: ob::draw_line_wave(s, start, num, amp); break;
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


    if (indicator.x >= track_len) {
        cout << "tracking finished : " << ofGetElapsedTimef() << endl;
        //bStart = false;
    }

    prevSamplePos = currentSamplePos;
}


void ofApp::draw(){

    ofSetLineWidth(1);
    //glPointSize(1);

    ofBackground(255);
    ofPushMatrix();

    if(bHandy){
        ofRotateZ(90.0);
        ofTranslate(0, -ofGetWindowWidth());
    }

    ofTranslate(start_point);

    /*
     OF_BLENDMODE_DISABLED = 0,
     OF_BLENDMODE_ALPHA 	  = 1,
     OF_BLENDMODE_ADD 	  = 2,
     OF_BLENDMODE_SUBTRACT = 3,
     OF_BLENDMODE_MULTIPLY = 4,
     OF_BLENDMODE_SCREEN   = 5
     */
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

    vPoints.draw();
    vLines.draw();
    vAudio.draw();

    ofSetColor(0);
    ofDrawBitmapString(ofToString(ofGetElapsedTimef()), indicator.x, yy+40);

    ofPopMatrix();

    int y = 10;
    int os = 15;
    ofSetColor(0);
    ofDrawBitmapString("fps  " + ofToString(ofGetFrameRate()), 10, y);
    ofDrawBitmapString("time " + ofToString(ofGetElapsedTimef()), 10, y+=os);
    ofDrawBitmapString("vPoints numVertices " + ofToString(vPoints.getNumVertices()), 10, y+=os);
    ofDrawBitmapString("vLines  numVertices " + ofToString(vLines.getNumVertices()), 10, y+=os);
    //ofDrawBitmapString("vAudio  numVertices " + ofToString(vAudio.getNumVertices()), 10, y+=os);
 }

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit( ofEventArgs& e){

}
