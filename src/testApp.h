#pragma once

#include "ofMain.h"
#include "mtlBox2d.h"
#include "ofxCvHaarFinder.h"

#include "HairBall.h"
#include "Person.h"

#define kCaptureWidth     320
#define kCaptureHeight    240
#define kCaptureScale       2

#define kMaxHairBalls    1024

#define kNumBlendModes     10

//========================================================================
const GLenum BLEND_MODES[] = { 
	GL_ZERO, 
	GL_ONE, 
	GL_SRC_COLOR, 
	GL_ONE_MINUS_SRC_COLOR, 
	GL_DST_COLOR, 
	GL_ONE_MINUS_DST_COLOR, 
	GL_SRC_ALPHA, 
	GL_ONE_MINUS_SRC_ALPHA, 
	GL_DST_ALPHA, 
	GL_ONE_MINUS_DST_ALPHA 
};

//========================================================================
class testApp : public ofBaseApp {
	
	public:
		void                setup();
		void                update();
		void                draw();
		
		void                keyPressed(int key);
		void                keyReleased(int key);
		void                mouseMoved(int x, int y);
		void                mouseDragged(int x, int y, int button);
		void                mousePressed(int x, int y, int button);
		void                mouseReleased();
    
        mtlBox2d            physics;
        b2World*            world;
        
        vector<Person*>     persons;
    
        int                 numHairBalls; 
        HairBall*           hairBalls[kMaxHairBalls];
        
        bool                white;
        int                 srcBlend;
        int                 dstBlend;
    
        ofVideoGrabber      capture;
        ofxCvColorImage     colorIn;
        ofxCvGrayscaleImage grayIn;
        ofxCvHaarFinder     faceTracker;
        ofRectangle         face;
    
        bool                debug;
        bool                info;
		
};
