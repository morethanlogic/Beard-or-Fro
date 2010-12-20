#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofSeedRandom();
    ofSetFrameRate(kTargetFPS);
    ofEnableAlphaBlending();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetBackgroundAuto(false);
	ofSetLogLevel(OF_LOG_NOTICE);
    ofSetWindowTitle("Beard or Fro");
#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/");
#endif
#ifdef TARGET_WIN32
	HWND handleWindow;
	AllocConsole();
	handleWindow = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(handleWindow, 0);
#endif
    
    // load the textures
    if (!HairBall::s_tex.loadImage("hair.png")) {
        cout << "Error loading HairBall texture" << endl;
    }
    
    srcBlend = GL_SRC_ALPHA;
    dstBlend = GL_ONE;
    
    // init the physics world
    world = physics.getWorld();
    physics.createBounds(0, 0, ofGetWidth(), ofGetHeight());
    
    doHair = true;
    toggleHairOrBeard();
    
    // build the HairBalls
    numHairBalls = 40;
    for (int i = 0; i < numHairBalls; i++) {
        hairBalls[i] = new HairBall(world, ofRandom(10, ofGetWidth() - 10), ofRandom(10, ofGetHeight() - 10), ofRandom(HairBall::s_minRadius, HairBall::s_maxRadius));
    }
    
    // init the face tracker
    capture.initGrabber(kCaptureWidth, kCaptureHeight);
    colorIn.allocate(kCaptureWidth, kCaptureHeight);
    grayIn.allocate(kCaptureWidth, kCaptureHeight);
    faceTracker.setup("haarcascade_frontalface_default.xml");
    
    debug = false;
    info  = false;
    titleIndex = 0;
}

//--------------------------------------------------------------
void testApp::update() {
    physics.update();
    
    if (doHair && HairBall::s_tint > 0) {
        HairBall::s_tint = ofLerp(HairBall::s_tint, 0, .1f);
    } else if (!doHair && HairBall::s_tint < 255) {
        HairBall::s_tint = ofLerp(HairBall::s_tint, 255, .1f);
    }
    
    capture.grabFrame();
    if (capture.isFrameNew()) {
        colorIn.resetROI();
        colorIn.setFromPixels(capture.getPixels(), kCaptureWidth, kCaptureHeight);
        grayIn = colorIn;
        faceTracker.findHaarObjects(grayIn);
        
        // create as many Persons as faces
        int upTo = 0;
        if (faceTracker.blobs.size() > persons.size()) {
            // add new Persons
            upTo = persons.size();
            for (int i = upTo; i < faceTracker.blobs.size(); i++) {
                face = faceTracker.blobs[i].boundingRect;
                face.width = face.height = MIN(face.width, face.height);
                persons.push_back(new Person(world, face.getCenter().x * kCaptureScale, face.getCenter().y * kCaptureScale, face.width * .4f * kCaptureScale));
            }
        } else if (faceTracker.blobs.size() < persons.size()) {
            // remove dead Persons
            while (faceTracker.blobs.size() < persons.size()) {
                Person* p = persons.back();
                delete p;
                persons.pop_back();
            }
            upTo = persons.size();
        } else {
            upTo = persons.size();
        }
        
        // update old Persons
        for (int i=0; i < upTo; i++) {
            face = faceTracker.blobs[i].boundingRect;
            face.width = face.height = MIN(face.width, face.height);
            persons[i]->update(face.getCenter().x * kCaptureScale, face.getCenter().y * kCaptureScale, face.width * .4f * kCaptureScale);
        }
    }
    
    // go through all the HairBalls
    for (int i = 0; i < numHairBalls; i++) { 
        // go through all the Persons
        for (int j = 0; j < persons.size(); j++) {
            persons[j]->attract(hairBalls[i]);
        }
    }
    
    // update the window title every 2 seconds
    if (ofGetFrameNum()%(kTargetFPS * 2) == 0) {
        titleIndex++;
        if (titleIndex%3 == 0) {
            ofSetWindowTitle("Beard or Fro");
        } else if (titleIndex%3 == 1) {
            ofSetWindowTitle("Click to toggle between a beard or a fro");
        } else {
            ofSetWindowTitle("Press the spacebar to save a screenshot");
        }
    }
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(255, 255, 255);
    
    if (debug) {
        physics.debug();
    } else {
        ofSetColor(255, 255, 255);
        
        glEnable(GL_BLEND);
        glBlendFunc(BLEND_MODES[srcBlend], BLEND_MODES[dstBlend]);
        
        colorIn.draw(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
        
        // draw all the HairBalls
        ofSetColor(HairBall::s_tint, HairBall::s_tint, HairBall::s_tint);
        for (int i = 0; i < numHairBalls; i++) {
            hairBalls[i]->draw();
        }
        
        ofEnableAlphaBlending();
    }
    
    if (info) {
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 2) + 
                           "\nPERSONS: " + ofToString((int)persons.size()) + 
                           "\nHAIRBALLS: " + ofToString(numHairBalls), 10, 20);        
    }
}

//--------------------------------------------------------------
void testApp::toggleHairOrBeard() {
    doHair = !doHair;
    if (doHair) {
        // gravity up
        physics.setGravityB2(b2Vec2(0, -300));
    } else {
        // gravity down
        physics.setGravityB2(b2Vec2(0, 300));
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) { 
	switch (key) {
        case 'd':
            debug = !debug;
            break;
        case 'i':
            info = !info;
            break;
            
        case ' ':
#ifdef TARGET_OSX
            ofSaveScreen("../../../BeardOrFro_" + ofToString(ofGetYear()) + ofToString(ofGetMonth()) + ofToString(ofGetDay()) + "_" + ofToString(ofGetFrameNum()) + ".jpg");
#else
            ofSaveScreen("../BeardOrFro_" + ofToString(ofGetYear()) + ofToString(ofGetMonth()) + ofToString(ofGetDay()) + "_" + ofToString(ofGetFrameNum()) + ".jpg");
#endif
            break;
            
        case OF_KEY_UP:
            srcBlend = (srcBlend+1)%kNumBlendModes;
            break;
        case OF_KEY_DOWN:
            srcBlend = (srcBlend-1)%kNumBlendModes;
            break;
            
        case OF_KEY_RIGHT:
            dstBlend = (dstBlend+1)%kNumBlendModes;
            break;
        case OF_KEY_LEFT:
            dstBlend = (dstBlend-1)%kNumBlendModes;
            break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) { 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    toggleHairOrBeard();
}

//--------------------------------------------------------------
void testApp::mouseReleased() {
}
