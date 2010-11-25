#pragma once

/*
 *  Person.h
 *  FroNect
 *
 *  Created by Elie Zananiri on 10-11-25.
 *  Copyright 2010 silentlyCrashing::net. All rights reserved.
 *
 */

#include "mtlBox2dCircle.h"
#include "HairBall.h"

//========================================================================
class Person : public mtlBox2dCircle {
  
public:
            Person(b2World* _world, float _x, float _y, float _radius);
    
    void    update(float _x, float _y, float _radius);
    void    attract(HairBall* _hairball);
    
    float   range;
    float   strength;
    
    static  int s_radiusThreshold;
    static  int s_minRadius;
    static  int s_maxRadius;
    static  int s_minRange;
    static  int s_maxRange;
    static  int s_strengthScalar;
    
};