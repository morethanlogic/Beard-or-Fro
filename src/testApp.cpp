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
    if (!HairBall::tex.loadImage("particle.png")) {
        cout << "Error loading HairBall texture" << endl;
    }
    
    srcBlend = GL_SRC_ALPHA;
    dstBlend = GL_ONE;
    
    // init the physics world
    world = physics.getWorld();
    physics.setGravityB2(b2Vec2(0, -9.8));
    physics.createBounds(0, 0, ofGetWidth(), ofGetHeight());
    
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
}

//--------------------------------------------------------------
void testApp::update() {
    physics.update();
    
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
    
    // go through all the FroBalls
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
        
        // draw all the FroBalls
        ofSetColor(255, 255, 255);
        for (int i = 0; i < numHairBalls; i++) {
            hairBalls[i]->draw();
        }
        
        glDisable(GL_BLEND);
        ofEnableAlphaBlending();
        
        // draw the creatures
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
