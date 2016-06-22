#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxOpenCv.h"


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

        void videoPreProcess();
        void draw_info();
        void draw_vid();
        void draw_feat();

        int target_fps = 60;
        ofVec2f canvas;
        float canvas_scale = -1;
        // video
        ofVideoGrabber  grabber;
        int             grbW;
        int             grbH;

        // cv
        vector<ofVec2f>     feat;
        ofxCvColorImage colorImg;
        ofxCvGrayscaleImage grayImg;

        bool bNeedSaveImg;

};
