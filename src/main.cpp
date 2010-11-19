#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main() {
    ofAppGlutWindow window;
    window.setGlutDisplayString("rgb alpha double samples depth");
    ofSetupOpenGL(&window, kCaptureWidth*kCaptureScale, kCaptureHeight*kCaptureScale, OF_WINDOW);
	ofRunApp(new testApp);
}
