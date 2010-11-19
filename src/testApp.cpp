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
    if (!Creature::overlay.loadImage("eyea.png")) cout << "Error loading Creature texture" << endl;
    if (!FroBall::overlay.loadImage("particle.png")) cout << "Error loading FroBall texture" << endl;
    
    srcBlend = GL_SRC_ALPHA;
    dstBlend = GL_ONE;
    
    // init the physics world
    world = physics.getWorld();
    physics.setGravity(b2Vec2(0, -10));
    physics.createBounds(0, 0, ofGetWidth(), ofGetHeight());
    
    // build the FroBalls
    numFroBalls = 300;
    for (int i = 0; i < numFroBalls; i++) {
        froBalls[i] = new FroBall(i, world, PIX2M(ofRandom(10, ofGetWidth() - 10)), PIX2M(ofRandom(10, ofGetHeight() - 10)));
	}
    
    creatures.push_back(new Creature(0, world, ofRandom(10, ofGetWidth() - 10), ofRandom(10, ofGetHeight() - 10), 50));
    
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
            face.width = face.height = MAX(face.width, face.height);
            colorIn.setROI(face);
            creatures[0]->update(face.getCenter().x * kCaptureScale, face.getCenter().y * kCaptureScale, face.width * kCaptureScale);
        }
        
//        for (int i = 0; i < faceTracker.blobs.size(); i++) {
//            ofRectangle face = faceTracker.blobs[i].boundingRect;
//            ofEllipse(face.getCenter().x, face.getCenter().y, face.width, face.height);
//        }
    }
    
    // go through all the FroBalls
    for (int i = 0; i < numFroBalls; i++) { 
        // go through all the Creatures
        for (int j = 0; j < creatures.size(); j++) {
            b2Vec2  acc = creatures[j]->pos() - froBalls[i]->pos();
            float32 dst = acc.Normalize();
            if (dst < creatures[j]->range) {
                // attract the FroBall
                froBalls[i]->applyForce(creatures[j]->strength * acc);
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(255, 255, 255);
    
    ofSetColor(255, 255, 255);
    colorIn.drawROI(face.getCenter().x * kCaptureScale, face.getCenter().y * kCaptureScale, face.width * kCaptureScale, face.height * kCaptureScale);
    
    glEnable(GL_BLEND);
    glBlendFunc(BLEND_MODES[srcBlend], BLEND_MODES[dstBlend]);
	
    // draw all the FroBalls
    ofSetColor(0, 0, 0);
    for (int i = 0; i < numFroBalls; i++) {
        froBalls[i]->draw();
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
    
    if (info) {
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 2) + 
                           "\nCREATURES: " + ofToString((int)creatures.size()) + 
                           "\nFROBALLS: " + ofToString(numFroBalls), 10, 20);        
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
