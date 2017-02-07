#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    std::string file = "test.json";
    bool parsingSuccessful = result.open(file);

    if (parsingSuccessful){
        ofLogNotice("ofApp::setup") << result.getRawString();
     }else{
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
        ofExit();
    }
}

void ofApp::draw(){
    ofBackground(0);
    ofSetHexColor(0x00FF00);

    std::stringstream ss;

    Json::Value::iterator itrA = result.begin();

    for( ; itrA!=result.end(); itrA++){

        Json::Value::iterator itrB = (*itrA).begin();

        for( ; itrB!=(*itrA).end(); itrB++){
            string name = itrB.memberName();
            string data = (*itrB).asString();
            ss << name << " : " << data << "\n";
        }
        ss << "\n\n";
    }

    ofDrawBitmapString(ss.str(), 20, 20);
}
