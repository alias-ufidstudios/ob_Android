#include "ofMain.h"
#include "ofApp.h"

int main(){
    ofSetupOpenGL(1024,768, OF_WINDOW);
    ofRunApp( &ofApp::get() );
    return 0;
}

#ifdef TARGET_ANDROID
void ofAndroidApplicationInit()
{
    //application scope init
}

void ofAndroidActivityInit()
{
    //activity scope init
    main();
}
#endif
