/*
 *  Effect.h
 *  Effects
 *
 *  Created by Makira on 11/02/05.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once;

#include "ofMain.h"
#include "ofxTween.h"
#include "ofxVectorMath.h"

class Effect {
	
public:
	ofImage img;
	
	ofxVec4f rot;
	ofxTween rotTween;
	ofxVec2f scale;
	ofxTween scaleTween;
	ofxVec3f pos;
	ofxTween posTween;
	ofxVec4f col;
	ofxTween colTween;
	
	// tweens
	ofxEasingBack		easingback;
	ofxEasingBounce 	easingbounce;
	ofxEasingCirc		easingcirc;
	ofxEasingCubic		easingcubic;
	ofxEasingElastic	easingelastic;
	ofxEasingExpo		easingexpo;
	ofxEasingLinear 	easinglinear;
	ofxEasingQuad		easingquad;
	ofxEasingQuart		easingquart;
	ofxEasingQuint		easingquint;
	ofxEasingSine		easingsine;		
	
};