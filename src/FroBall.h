#pragma once

/*
 *  FroBall.h
 *
 *  Created by Elie Zananiri on 10-11-19.
 *  Copyright 2010 silentlyCrashing::net. All rights reserved.
 *
 */

#include "ofMain.h"
#include "mtlBox2d.h"

#define kMinFroRadius   2
#define kMaxFroRadius  20

//========================================================================
class FroBall {
	
    public:
        static ofImage  overlay;
        
                        FroBall(int _id, b2World* _world, float _x, float _y);
                        ~FroBall();
        
        void            applyForce(const b2Vec2& _force);
        void            draw();
    
        const b2Vec2&   pos();
        
        b2World         *world;
        b2Body          *body;
        
        int             id;
        float           radius;
        ofColor         col;
	
};
