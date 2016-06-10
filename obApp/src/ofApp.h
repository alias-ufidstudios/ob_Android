#pragma once

#include "ofMain.h"
using namespace std::chrono;

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void exit(ofEventArgs & args);
        void audioIn(float * input, int bufferSize, int nChannels);

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
    ofVec2f indicator;   // relative (diff from start_point)

    ofVboMesh vPoints;
    ofVboMesh vLines;
    ofVboMesh vAudio;

    // sound
    int sampleRate;
    int bufferSize;
    int nCh;
    int currentSamplePos;
    int prevSamplePos;
    vector<float> audioData;
    ofSoundStream soundStream;
};
