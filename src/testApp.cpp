#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofSeedRandom();
    //ofSetFrameRate(FPS);
    ofEnableAlphaBlending();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetBackgroundAuto(false);
	ofSetLogLevel(OF_LOG_NOTICE);
    
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
    
    persons.push_back(new Person(world, ofRandom(10, ofGetWidth() - 10), ofRandom(10, ofGetHeight() - 10), ofRandom(Person::s_minRadius, Person::s_maxRadius)));
    
    // init the face tracker
    capture.initGrabber(kCaptureWidth, kCaptureHeight);
    colorIn.allocate(kCaptureWidth, kCaptureHeight);
    grayIn.allocate(kCaptureWidth, kCaptureHeight);
    faceTracker.setup("haarcascade_frontalface_default.xml");
    
    debug = false;
    info  = false;
    
//    tweaks.init("Tweaks", 200, 400, 200, 200, 200, 100);
//	tweaks.enable();
//    
//    tweaks.addSeparator("Person");
//	tweaks.addParam("Radius Threshold", &Person::s_radiusThreshold, " min=0   max=10   step=1 ");
//	tweaks.addParam("Min Radius",       &Person::s_minRadius,       " min=10  max=50   step=1 ");
//	tweaks.addParam("Max Radius",       &Person::s_maxRadius,       " min=50  max=150  step=1 ");
//	tweaks.addParam("Min Range",        &Person::s_minRange,        " min=0   max=200  step=1 ");
//	tweaks.addParam("Max Range",        &Person::s_maxRange,        " min=300 max=800  step=1 ");
//	tweaks.addParam("Strength Scalar",  &Person::s_strengthScalar,  " min=1   max=20   step=1 ");
//    
//    tweaks.addSeparator("HairBall");
//	tweaks.addParam("Min Radius",       &Person::s_minRadius,       " min=1   max=20   step=1 ");
//	tweaks.addParam("Max Radius",       &Person::s_maxRadius,       " min=20  max=50   step=1 ");
//	tweaks.addParam("Draw Scale",       &Person::s_minRange,        " min=0.5 max=5.0  step=0.5 ");   
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
        
        if (faceTracker.blobs.size() > 0) {
            face = faceTracker.blobs[0].boundingRect;
            face.width = face.height = MIN(face.width, face.height);
            //colorIn.setROI(face);
            //persons[0]->update(face.getCenter().x * kCaptureScale, face.getCenter().y * kCaptureScale, face.width * .5f * kCaptureScale);
        }
        
//        for (int i = 0; i < faceTracker.blobs.size(); i++) {
//            ofRectangle face = faceTracker.blobs[i].boundingRect;
//            ofEllipse(face.getCenter().x, face.getCenter().y, face.width, face.height);
//        }
    }
    
    // go through all the HairBalls
    for (int i = 0; i < numHairBalls; i++) { 
        // go through all the Persons
        for (int j = 0; j < persons.size(); j++) {
            persons[j]->attract(hairBalls[i]);
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
        
        //    if (faceTracker.blobs.size() > 0) {
        //        colorIn.drawROI(face.getCenter().x * kCaptureScale, face.getCenter().y * kCaptureScale, face.width * kCaptureScale, face.height * kCaptureScale);
        //    }
        
        ofCircle(persons[0]->getPosition().x, persons[0]->getPosition().y, persons[0]->getRadius());
        
        // draw all the HairBalls
        ofSetColor(HairBall::s_tint, HairBall::s_tint, HairBall::s_tint);
        for (int i = 0; i < numHairBalls; i++) {
            hairBalls[i]->draw();
        }
        
        glDisable(GL_BLEND);
        ofEnableAlphaBlending();
        
        // draw the Persons
        //    for (int i = 0; i < creatures.size(); i++) {
        //        creatures[i]->draw();
        //    }
        
        //    for (int i = 0; i < faceTracker.blobs.size(); i++) {
        //        ofRectangle face = faceTracker.blobs[i].boundingRect;
        //        ofEllipse(face.getCenter().x, face.getCenter().y, face.width, face.height);
        //    }
    }
    
    if (info) {
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 2) + 
                           "\nPERSONS: " + ofToString((int)persons.size()) + 
                           "\nHAIRBALLS: " + ofToString(numHairBalls), 10, 20);        
    }
//    
//    tweaks.draw();
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
            toggleHairOrBeard();
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
    persons[0]->update(x, y, 100);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    //creatures.push_back(new Creature(creatures.size(), world, PIX2M(x), PIX2M(y)));
}

//--------------------------------------------------------------
void testApp::mouseReleased() {
}
