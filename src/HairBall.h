#pragma once

/*
 *  HairBall.h
 *  FroNect
 *
 *  Created by Elie Zananiri on 10-11-25.
 *  Copyright 2010 silentlyCrashing::net. All rights reserved.
 *
 */

#include "mtlBox2dCircle.h"

//========================================================================
class HairBall : public mtlBox2dCircle {
    
public:
            HairBall(b2World* _world, float _x, float _y, float _radius);
    
    void    draw();
    void    applyForce(const b2Vec2& _force);
    
    static  ofImage s_tex;
    static  float   s_tint;
    
    static  int     s_minRadius;
    static  int     s_maxRadius;
    static  float   s_drawScale;
    
};