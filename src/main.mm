#include "ofMain.h"
#include "testApp.h"
#include "ofxCocoa.h"

//========================================================================
int main() {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	MSA::ofxCocoa::InitSettings			initSettings;
    initSettings.numFSAASamples			= 4;
	initSettings.initRect.size.width	= kCaptureWidth*kCaptureScale;
	initSettings.initRect.size.height	= kCaptureHeight*kCaptureScale;
	initSettings.windowMode				= OF_WINDOW;
	//initSettings.windowStyle			= NSBorderlessWindowMask;
	MSA::ofxCocoa::AppWindow window(initSettings);
    
    ofSetupOpenGL(&window, kCaptureWidth*kCaptureScale, kCaptureHeight*kCaptureScale, OF_WINDOW);
	ofRunApp(new testApp());
    
    [pool drain];
}
