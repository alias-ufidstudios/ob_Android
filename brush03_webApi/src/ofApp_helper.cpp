#include "ofApp.h"

string ofApp::getExternalCacheDir(){
    JNIEnv * env = ofGetJNIEnv();
    jobject activity = ofGetOFActivityObject();
    jclass activityClass = env->FindClass("cc/openframeworks/brush03_webApi/OFActivity");
    jmethodID method = env->GetMethodID(activityClass,"getExternalCacheDirJava","()Ljava/lang/String;");
    jstring js = (jstring)env->CallObjectMethod(activity, method);
    const char * c = env->GetStringUTFChars(js,0);
    return string(c);
}

void ofApp::openShareIntent() {

    // save
    string fileName = cacheDir + "/BrushCore.png";
    ofSaveScreen(fileName);

    // call java
    JNIEnv * env = ofGetJNIEnv();
    jobject activity = ofGetOFActivityObject();
    jclass activityClass = env->FindClass("cc/openframeworks/brush03_webApi/OFActivity");
    jmethodID method = env->GetMethodID(activityClass,"openShareIntent","(Ljava/lang/String;)V");
    jstring fileNamej = env->NewStringUTF(fileName.c_str());
    env->CallVoidMethod(activity, method, fileNamej);
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

void ofApp::exit(){
//    onDestroy does not work
//    string fileName = cacheDir + "/tmp.png";
//    ofFile::removeFile(fileName, false);
}