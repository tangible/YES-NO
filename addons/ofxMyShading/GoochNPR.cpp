/*
 *  GoochNPR.cpp
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/11.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#include "GoochNPR.h"

void GoochNPR::setup(ofxVec3f _LightPos,
					 ofxVec3f _WarmColour,
					 ofxVec3f _CoolColour,
					 ofxVec3f _SurfaceColour,
					 float _OutlineWidth) {
	
	ofxMyShading::setDataPathRoot();
	shader.setup("shaders/gooch");
	ofxMyShading::resetDataPathRoot();

	LightPos = _LightPos;
	WarmColour = _WarmColour;
	CoolColour = _CoolColour;
	SurfaceColour = _SurfaceColour;
	OutlineWidth = _OutlineWidth;
	
}

void GoochNPR::beginRender() {
	
	glDisable(GL_CULL_FACE);

	shader.begin();
	shader.setUniform3f("LightPos", LightPos.x, LightPos.y, LightPos.z);
	shader.setUniform3f("WarmColour", WarmColour.x, WarmColour.y, WarmColour.z);
	shader.setUniform3f("CoolColour", CoolColour.x, CoolColour.y, CoolColour.z);
	shader.setUniform3f("SurfaceColour", SurfaceColour.x, SurfaceColour.y, SurfaceColour.z);
	shader.setUniform1f("OutlineWidth", OutlineWidth);
	
}

void GoochNPR::endRender() {

	shader.end();

}