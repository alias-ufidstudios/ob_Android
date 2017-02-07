#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxJSON.h"

class ofApp : public ofxAndroidApp{
	
	public:
		
		void setup();
		void draw();


        ofxJSONElement result;
};
