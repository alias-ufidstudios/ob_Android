#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

using namespace std::chrono;

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

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

        void audioIn(float * input, int bufferSize, int nChannels);

    void draw_wave();
    void draw_bg();
    void draw_info();

    const int total_time_ms = 3 * 60 * 1000; // 3 mim
    const int targetFps = 60;

    bool bHandy;
    bool bStart;

    // visual
    ofRectangle canvas;
    float track_len;
    float track_offset;
    float indicator_speed;

    ofVec2f start_point; // absolute pix pos
    ofVec2f indicator;   // relative pix pos(dist from start_point)

    // sound
    int currentSamplePos;
    int prevSamplePos;
    vector<float> audioData;
    ofSoundStream soundStream;

    // video
    //ofVideoGrabber grabber;

};
