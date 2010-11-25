/*
 *  Person.cpp
 *  FroNect
 *
 *  Created by Elie Zananiri on 10-11-25.
 *  Copyright 2010 silentlyCrashing::net. All rights reserved.
 *
 */

#include "Person.h"
#include "testApp.h"

//--------------------------------------------------------------
int Person::s_radiusThreshold   =   5;
int Person::s_minRadius         =  30;
int Person::s_maxRadius         =  60;
int Person::s_minRange          = 100;
int Person::s_maxRange          = 600;
int Person::s_strengthScalar    =  10;

//--------------------------------------------------------------
Person::Person(b2World* _world, float _x, float _y, float _radius) {
    setup(_world, _x, _y, _radius, 0, true);
    
    // set attraction parameters
    range    = ofMap(radius, s_minRadius, s_maxRadius, s_minRange, s_maxRange);
    strength = radius * s_strengthScalar;
}

//--------------------------------------------------------------
void Person::update(float _x, float _y, float _radius) {
    setPosition(b2Vec2(_x, _y));
    if (ABS(_radius - radius) > s_radiusThreshold) {
        setRadius(_radius);
        
        // set attraction parameters
        range    = ofMap(radius, s_minRadius, s_maxRadius, s_minRange, s_maxRange);
        strength = radius * s_strengthScalar;
    }
}

//--------------------------------------------------------------
void Person::attract(HairBall* _hairball) {
    b2Vec2  acc = getPosition() - _hairball->getPosition();
    float32 dst = acc.Normalize();
    if (dst < range) {
        // attract the FroBall
        _hairball->applyForce(strength * acc);
    }
}
