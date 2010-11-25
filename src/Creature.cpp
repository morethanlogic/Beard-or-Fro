/*
 *  Creature.cpp
 *
 *  Created by Hugues Bruyere on 10-11-16.
 *  Copyright 2010 more than logic. All rights reserved.
 *
 */

#include "Creature.h"

//--------------------------------------------------------------
ofImage Creature::overlay;

//--------------------------------------------------------------
Creature::Creature(int _id, b2World* _world, float _x, float _y, float _radius) {
    id    = _id;
    world = _world;
    
    // create a body and add it to the world
    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position.Set(PIX2M(_x), PIX2M(_y));
    bd.angle = 0;
    
    body = world->CreateBody(&bd);
    
    // add collision shapes to that body
    radius = _radius;
    b2CircleShape m_circle;
    m_circle.m_radius = PIX2M(radius);
    
    b2FixtureDef fd;
    fd.shape       = &m_circle;
    fd.density     = 0;
    fd.friction    = .1f;
    fd.restitution = .1f;
    
    body->CreateFixture(&fd);
    
    // set attraction parameters
    range    = PIX2M(ofMap(radius, kMinCreatureRadius, kMaxCreatureRadius, kMinCreatureRange, kMaxCreatureRange));
    strength = PIX2M(radius * kCreatureStrength);
    
    // set a random color
    col.set(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
}

//--------------------------------------------------------------
Creature::~Creature() {
    if (world == NULL) {
        ofLog(OF_LOG_WARNING, "Creature::~Creature() Must have a valid b2World");
        return;
    } else if (!body) {
        ofLog(OF_LOG_WARNING, "Creature::~Creature() NULL body");
        return;
    }
    
    world->DestroyBody(body);
    body = NULL;
}

//--------------------------------------------------------------
void Creature::update(float _x, float _y, float _radius) {
    world->DestroyBody(body);
    
    // create a body and add it to the world
    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position.Set(PIX2M(_x), PIX2M(_y));
    bd.angle = 0;
    
    body = world->CreateBody(&bd);
    
    // add collision shapes to that body
    radius = _radius;
    b2CircleShape m_circle;
    m_circle.m_radius = PIX2M(radius);
    
    b2FixtureDef fd;
    fd.shape       = &m_circle;
    fd.density     = 0;
    fd.friction    = .1f;
    fd.restitution = .1f;
    
    body->CreateFixture(&fd);
    
    // set attraction parameters
    range    = PIX2M(ofMap(radius, kMinCreatureRadius, kMaxCreatureRadius, kMinCreatureRange, kMaxCreatureRange));
    strength = PIX2M(radius * kCreatureStrength);
    
    
//    body->DestroyFixture(fixture);
//    
//    if (ABS(radius - _radius) > kRadiusThreshold) {
//        radius = _radius;
//        cs.m_radius = PIX2M(radius);
//        fixture = body->CreateFixture(&fd);
//        
//        // set attraction parameters
//        range    = PIX2M(ofMap(radius, kMinCreatureRadius, kMaxCreatureRadius, kMinCreatureRange, kMaxCreatureRange));
//        strength = PIX2M(radius * kCreatureStrength);
//    }
    
//	if (!motionTween.isRunning()) {
//		motionTween.setParameters(quad, ofxTween::easeOut, 
//                                  pos().x, PIX2M(ofRandom(0, ofGetWidth())), 
//                                  ofRandom(2000, 10000), ofRandom(0, 500));
//		motionTween.addValue(pos().y, PIX2M(ofRandom(0, ofGetHeight())));
//		motionTween.start();
//	} else {
//        body->SetTransform(b2Vec2(motionTween.update(), motionTween.getTarget(1)), 0);
//    }
	
//	if (!radiusTween.isRunning()) {
//		int factor = ofMap(Creature::nbOfBibittes, 1, 25, 1, 7);
//		float newRadius = ofRandom(10/factor, 100/factor);
//		attractors[0]->range = ofMap(newRadius, 10, 100, 5, 9); //ofRandom(1, 3);
//		attractors[0]->strength = (newRadius/30)*10;
//		
//		radiusTween.setParameters(quad,ofxTween::easeOut, attractors[0]->radius, newRadius, ofRandom(500, 1000), ofRandom(2000, 10000));
//		
//		radiusTween.start();
//	}
//	
//	b2Vec2 currPosition = attractors[0]->obstacle->GetPosition();
//	b2Vec2 dbPosition =  centerPt - currPosition;
////	cout << "********** " << dbPosition.x << " ------ "<< dbPosition.y << endl;
	
//	currPosition.x = motionTween.update();
//	currPosition.y = motionTween.getTarget(1);
//	
//	attractors[0]->radius = radiusTween.update();
//	
//	attractors[0]->pos.Set(currPosition.x, currPosition.y);
//	attractors[0]->obstacle->SetXForm(currPosition, 0);
}

//--------------------------------------------------------------
void Creature::draw() {
    ofSetColor(col);
    overlay.draw(M2PIX(pos().x), M2PIX(pos().y), radius*2, radius*2);
}

//--------------------------------------------------------------
const b2Vec2& Creature::pos() {
    return body->GetPosition();
}
