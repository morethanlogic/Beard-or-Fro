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
    
    white    = true;
    srcBlend = GL_SRC_ALPHA;
    dstBlend = GL_ONE;
    
    // init the physics world
    world = physics.getWorld();
    physics.setGravity(b2Vec2(0, 0));
    physics.createBounds(0, 0, ofGetWidth(), ofGetHeight());
    
    // build the FroBalls
    numFroBalls = 1000;
    for (int i = 0; i < numFroBalls; i++) {
        froBalls[i] = new FroBall(i, world, PIX2M(ofRandom(10, ofGetWidth() - 10)), PIX2M(ofRandom(10, ofGetHeight() - 10)));
	}
}

//--------------------------------------------------------------
void testApp::update() {
    physics.update();
    
    // update all the creatures
    ofSetColor(255, 255, 255);
    for (int i = 0; i < creatures.size(); i++) {
        creatures[i]->update();
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
    ofBackground(0, 0, 0);
    if (!white) {
        ofEnableAlphaBlending();
    }
    
    // draw the creatures
    ofSetColor(255, 255, 255);
    for (int i = 0; i < creatures.size(); i++) {
        creatures[i]->draw();
    }
    
    if (white) {
        glEnable(GL_BLEND);
		glBlendFunc(BLEND_MODES[srcBlend], BLEND_MODES[dstBlend]);
		ofSetColor(255, 255, 255);
	} else {
		ofSetColor(0, 0, 0);
	}
    
    // draw all the FroBalls
    for (int i = 0; i < numFroBalls; i++) {
        froBalls[i]->draw();
    }
    
    if (white) {
		glDisable(GL_BLEND);
		ofEnableAlphaBlending();
	}
    
    // draw the creatures
    // TODO: Find out why we're doing this again here
    ofSetColor(255, 255, 255);
    for (int i = 0; i < creatures.size(); i++) {
        creatures[i]->draw();
    }
    
    if (!white) {
		ofDisableAlphaBlending();
	}
    
    ofSetColor(255, 255, 255);
	ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 2) + "\nCREATURES: " + ofToString((int)creatures.size()) + "\nFROBALLS: " + ofToString(numFroBalls), 10, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) { 
	switch (key) {
        case 'w':
            white = !white;
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
    creatures.push_back(new Creature(creatures.size(), world, PIX2M(x), PIX2M(y)));
}

//--------------------------------------------------------------
void testApp::mouseReleased() {
}
