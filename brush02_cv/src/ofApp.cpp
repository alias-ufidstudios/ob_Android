#include "ofApp.h"
#include "obGeoDrawer.hpp"
#include "obStats.hpp"

//
//  if n<0 -1
//  if n=0  0
//  if n>0  1
//
template<typename T>
inline int signval(T n){
    return (n > 0) - (n < 0);
}

void ofApp::setup(){
    ofSetFrameRate(target_fps);
    ofSetVerticalSync(true);
    //ofSetWindowShape(1920, 1080);
    ofSetWindowPosition(0, 0);
    ofEnableAlphaBlending();
    ofDisableAntiAliasing();
    ofDisableSmoothing();
    ofSetOrientation( OF_ORIENTATION_DEFAULT );
    ofSetLogLevel(OF_LOG_VERBOSE);

    bNeedSaveImg = false;

    // video
    grbW = 320;
    grbH = 240;
    grabber.listDevices();
    grabber.setDeviceID(1);
    grabber.setUseTexture(false);
    grabber.setPixelFormat( OF_PIXELS_RGB );
    //grabber.setPixelFormat(OF_PIXELS_MONO);

    grabber.setDesiredFrameRate( target_fps );
    grabber.setup( grbW, grbH );

    colorImg.allocate( grbW, grbH );
    grayImg.allocate( grbW, grbH );

    float width = ofGetWindowWidth();
    float height = ofGetWindowHeight();
    float scaleW = width/grbH;
    float scaleH = height/grbW;
    canvas_scale = min(scaleW, scaleH);
    canvas.x = width * canvas_scale;
    canvas.y = height * canvas_scale;
}

void ofApp::videoPreProcess(){

    grabber.update();

    if( grabber.isFrameNew() ){

        if (grabber.getPixels().getData()!=NULL) {

            colorImg.setFromPixels( grabber.getPixels().getData(), grbW, grbH  );
            grayImg = colorImg;
            grayImg.threshold(70);

            unsigned char * data = grayImg.getPixels().getData();
            size_t step = sizeof(unsigned char) * grbW;
            cv::Mat mat( grbH, grbW, CV_8UC1, data, step );
            vector<cv::KeyPoint> keys;

            auto detector = cv::ORB(500, 1.2f, 16, 0, 0, 4);
            //auto detector = cv::BRISK(10, 1, 1.f);
            //auto detector = cv::MSER( 5, 60, 14400);
            //auto detector = cv::FastFeatureDetector( 30, true, cv::FastFeatureDetector::TYPE_9_16 );
            detector.detect(mat, keys);

            feat.clear();
            for( int i=0; i<keys.size(); i++ ){
                cv::Point2f & p = keys[i].pt;
                feat.push_back( ofVec2f(p.x, p.y) );
            }

            // put point on frame
            if(0){
                float row = 10;
                float col = 10;
                float addW = grbH/col;
                float addH = grbW/row;

                for( unsigned int i=0; i<col+1; i++ ){
                    float x = 0 + addW*i;
                    feat.push_back(ofVec2f(x, 0));
                    feat.push_back(ofVec2f(x,grbH));
                }

                for( unsigned int i=0; i<row+1; i++ ){
                    float y = 0 + addH*i;
                    feat.push_back(ofVec2f(0,y));
                    feat.push_back(ofVec2f(grbW,y));
                }
            }

            // put grid
            if(1){
                float row = 10;
                float col = 10;
                float addW = canvas.x/col;
                float addH = canvas.y/row;

                for( unsigned int i=0; i<col+1; i++ ){
                    for( unsigned int j=0; j<row+1; j++ ){
                        float x = 0 + addW*i;
                        float y = 0 + addH*j;
                        feat.push_back(ofVec2f(x, y));
                    }
                }
            }

            // put random
            for( unsigned int i=0; i<keys.size()/10; i++ ){
                feat.push_back(ofVec2f(grbW*ofRandomuf(), grbH*ofRandomuf() ));
            }
        }
    }
}

void ofApp::update(){
    videoPreProcess();

}

void ofApp::draw(){

    ofSetLineWidth(1);
    glPointSize(1);
    ofBackground(255);

    ofSetColor(255,0,0);
    ofPushMatrix();
    {
       ofScale( canvas_scale, canvas_scale );
       ofRotateZ(-90);
       ofTranslate(-grbW, 0 );

       //ofTranslate( )
       draw_vid();
       draw_feat();
    }ofPopMatrix();

    draw_info();

}

void ofApp::draw_vid(){

    ofSetColor(255,150);
    grayImg.draw(0, 0);
}

void ofApp::draw_feat(){

    ofPushMatrix();{

        vector<ofVec2f> lines;
        int size = feat.size();
        for( int i=0; i<size-1; i++){
            for( int j=i+1; j<size; j++){
                //int id1 = ofRandom(0, size);
                //int id2 = ofRandom(0, size);
                int id1 = i;
                int id2 = j;
                const ofVec2f & v1 = feat[id1];
                const ofVec2f & v2 = feat[id2];
                float dist = v1.distance(v2);
                float max = 10;
                float noise = ofNoise(i*j+ofRandomf());
                if( noise>0.78 ) max = ofGetWindowHeight()*0.7f;

                if( 1<dist && dist<max ){
                    lines.push_back(v1);
                    lines.push_back(v2);
                }
            }
        }

        ofSetLineWidth(1);
        ofSetColor(40, 45);
        ob::drawLines(lines);

        glPointSize(2);
        ofSetColor(55,0,0, 255);
        ob::drawDots(feat);

    }
    ofPopMatrix();

}

void ofApp::draw_info(){
    int y = 10;
    int x = 10;
    int os = 170;
    ofSetColor(0);
    ofDrawBitmapString("fps        " + ofToString(ofGetFrameRate()), x, y);
}


void ofApp::keyPressed  (int key){}
void ofApp::keyReleased(int key){}
void ofApp::windowResized(int w, int h){}
void ofApp::touchDown(int x, int y, int id){}
void ofApp::touchMoved(int x, int y, int id){}
void ofApp::touchUp(int x, int y, int id){}

void ofApp::touchDoubleTap(int x, int y, int id){
    //string fileName = "screenshot/" + ofGetTimestampString() + ".png";
    string fileName= "/sdcard/DCIM/myFolder/ob"
    				+ ofToString(ofGetYear()) + ofToString(ofGetMonth())
    				+ ofToString(ofGetDay()) + ofToString(ofGetHours())
    				+ ofToString(ofGetMinutes()) + ofToString(ofGetSeconds())
    				+ ".png";
    ofSaveScreen(fileName);
}

void ofApp::touchCancelled(int x, int y, int id){}
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){}
void ofApp::pause(){}
void ofApp::stop(){}
void ofApp::resume(){}
void ofApp::reloadTextures(){}
bool ofApp::backPressed(){
	return false;
}

void ofApp::okPressed(){}
void ofApp::cancelPressed(){}
