#pragma once

/*
 *  Creature.h
 *
 *  Created by Hugues Bruyere on 10-11-16.
 *  Copyright 2010 more than logic. All rights reserved.
 *
 */

#include "ofMain.h"
#include "mtlBox2d.h"

#include "ofxTween.h"
#include "ofxEasingQuad.h"

#define kMinCreatureRadius   30
#define kMaxCreatureRadius   60
#define kMinCreatureRange   100
#define kMaxCreatureRange   300
#define kCreatureStrength    10

//========================================================================
class Creature {
	
    public:
        static ofImage  overlay;
    
                        Creature(int _id, b2World* _world, float _x, float _y);
                        ~Creature();
        
        void            update();
        void            draw();
    
        const b2Vec2&   pos();
        
        b2World         *world;
        b2Body          *body;
        
        int             id;
        float           radius;
        ofColor         col;
        
        float32         range;
        float32         strength;
        
        ofxTween        motionTween;
        ofxTween        radiusTween;
        ofxEasingQuad   quad;
	
};

