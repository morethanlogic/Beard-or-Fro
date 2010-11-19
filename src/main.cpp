#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main() {
    ofAppGlutWindow window;
    window.setGlutDisplayString("rgb alpha double samples depth");
    ofSetupOpenGL(&window, 854, 480, OF_WINDOW);
	ofRunApp(new testApp);
}
