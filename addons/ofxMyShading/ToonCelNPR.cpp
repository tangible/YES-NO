/*
 *  ToonCelNPR.cpp
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/11.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#include "ToonCelNPR.h"

void ToonCelNPR::setup(ofxVec3f _LightPos,
					   ofxVec4f _HighlightColour,
					   ofxVec4f _MidColour,
					   ofxVec4f _ShadowColour,
					   float _HighlightSize,
					   float _ShadowSize,
					   float _OutlineWidth,
					   ofxVec4f _OutlineColor) {

	ofxMyShading::setDataPathRoot();
	shader.setup("fluxus/toon");
	ofxMyShading::resetDataPathRoot();
	
	LightPos = _LightPos;
	HighlightColour = _HighlightColour;
	MidColour = _MidColour;
	ShadowColour = _ShadowColour;
	HighlightSize = _HighlightSize;
	ShadowSize = _ShadowSize;
	OutlineWidth = _OutlineWidth;
	OutlineColor = _OutlineColor;
	
}

void ToonCelNPR::beginRender() {

	shader.begin();
	shader.setUniform3f("LightPos", LightPos.x, LightPos.y, LightPos.z);
	shader.setUniform4f("HighlightColour", HighlightColour.x, HighlightColour.y, HighlightColour.z, HighlightColour.w);
	shader.setUniform4f("MidColour", MidColour.x, MidColour.y, MidColour.z, MidColour.w);
	shader.setUniform4f("ShadowColour", ShadowColour.x, ShadowColour.y, ShadowColour.z, ShadowColour.w);
	shader.setUniform1f("HighlightSize", HighlightSize);
	shader.setUniform1f("ShadowSize", ShadowSize);
	shader.setUniform1f("OutlineWidth", OutlineWidth);
	shader.setUniform4f("OutlineColor", OutlineColor.x, OutlineColor.y, OutlineColor.z,	OutlineColor.w);
	
}

void ToonCelNPR::endRender() {

	shader.end();

}