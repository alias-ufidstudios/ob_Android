#pragma once

//#define USE_GRABBER

#include "ofMain.h"
#include "ofxAndroid.h"
//#include "ofxOpenCv.h"


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

        void audioIn(float * input, int bufferSize, int nChannels) override;

        void audioPreProcess();
        void videoPreProcess();
        void draw_wave();
        void draw_bg();
        void draw_info();
        void draw_audioStats();
        void draw_vid();

        // app
        const int       total_time_ms = 3 * 60 * 1000; // 3 mim
        const int       target_fps = 60;

        bool            bHandy;
        bool            bStart;
        bool            bLog;

        // visual
        ofRectangle     canvas;
        float           track_len;
        float           track_offset;
        float           indicator_speed;

        ofVec2f         start_point; // absolute pix pos
        ofVec2f         indicator;   // relative pix pos(dist from start_point)

        // sound
        int             currentSamplePos;
        int             prevSamplePos;
        float *         audioIn_raw;
        vector<float>   audioIn_data;
        ofSoundStream   sound_stream;

#ifdef USE_GRABBER
        // video
        ofVideoGrabber  grabber;
        int             grbW;
        int             grbH;

        // cv
        vector<ofVec2f>     feat;
        ofxCvColorImage     colorImg;
        ofxCvGrayscaleImage grayImg;
#endif

        bool bNeedSaveImg;

};
