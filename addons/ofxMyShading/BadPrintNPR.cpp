/*
 *  BadPrintNPR.cpp
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/11.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#include "BadPrintNPR.h"

void BadPrintNPR::setup(ofxVec3f _LightPos,
						ofxVec3f _Scale,
						ofxVec3f _Offset,
						ofxVec3f _Register,
						ofxVec3f _Size) {

	ofxMyShading::setDataPathRoot();
	shader.setup("fluxus/badprint");
	ofxMyShading::resetDataPathRoot();
	
	LightPos = _LightPos;
	Scale = _Scale;
	Offset = _Offset;
	Register = _Register;
	Size = _Size;

}

void BadPrintNPR::beginRender() {

	shader.begin();
	shader.setUniform3f("LightPos", LightPos.x, LightPos.y, LightPos.z);
	shader.setUniform3f("Scale", Scale.x, Scale.y, Scale.z);
	shader.setUniform3f("Offset", Offset.x, Offset.y, Offset.z);
	shader.setUniform3f("Register", Register.x, Register.y, Register.z);
	shader.setUniform3f("Size", Size.x, Size.y, Size.z);

}

void BadPrintNPR::endRender() {

	shader.end();

}