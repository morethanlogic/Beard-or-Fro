/*
 *  HairBall.cpp
 *  FroNect
 *
 *  Created by Elie Zananiri on 10-11-25.
 *  Copyright 2010 silentlyCrashing::net. All rights reserved.
 *
 */

#include "HairBall.h"

//--------------------------------------------------------------
ofImage HairBall::tex;

int     HairBall::s_minRadius   =   10;
int     HairBall::s_maxRadius   =   30;
float   HairBall::s_drawScale   =    3.f;

//--------------------------------------------------------------
HairBall::HairBall(b2World* _world, float _x, float _y, float _radius) {
    setup(_world, _x, _y, _radius);
}

//--------------------------------------------------------------
void HairBall::draw() {
    pos = getPosition();
    tex.draw(pos.x, pos.y, radius*s_drawScale, radius*s_drawScale);
}

//--------------------------------------------------------------
void HairBall::applyForce(const b2Vec2& _force) {
    body->ApplyForce(_force, body->GetWorldPoint(b2Vec2(0, 0)));
}
