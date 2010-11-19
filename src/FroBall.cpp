/*
 *  FroBall.cpp
 *  Box2DAttraction
 *
 *  Created by Elie Zananiri on 10-11-19.
 *  Copyright 2010 silentlyCrashing::net. All rights reserved.
 *
 */

#include "FroBall.h"

//--------------------------------------------------------------
ofImage FroBall::overlay;

//--------------------------------------------------------------
FroBall::FroBall(int _id, b2World* _world, float _x, float _y) {
    id    = _id;
    world = _world;
    
    // create a body and add it to the world
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(_x, _y);
    bd.angle = 0;
    
    body = world->CreateBody(&bd);
    
    // add collision shapes to that body
    radius = ofRandom(kMinFroRadius, kMaxFroRadius);
    b2CircleShape m_circle;
    m_circle.m_radius = PIX2M(radius);
    
    b2FixtureDef fd;
    fd.shape       = &m_circle;
    fd.density     = 0;
    fd.friction    = .1f;
    fd.restitution = .1f;
    
    body->CreateFixture(&fd);
}

//--------------------------------------------------------------
FroBall::~FroBall() {
    if (world == NULL) {
        ofLog(OF_LOG_WARNING, "FroBall::~FroBall() Must have a valid b2World");
        return;
    } else if (!body) {
        ofLog(OF_LOG_WARNING, "FroBall::~FroBall() NULL body");
        return;
    }
    
    world->DestroyBody(body);
    body = NULL;
}

//--------------------------------------------------------------
void FroBall::applyForce(const b2Vec2& _force) {
    body->ApplyForce(_force, body->GetWorldPoint(b2Vec2(0, 0)));
}

//--------------------------------------------------------------
void FroBall::draw() {
    // draw them twice as big as they actually are
    overlay.draw(M2PIX(pos().x), M2PIX(pos().y), radius*4, radius*4);
}

//--------------------------------------------------------------
const b2Vec2& FroBall::pos() {
    return body->GetPosition();
}
