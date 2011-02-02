/*
 *  MetaBallChunk.h
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/02.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "Metaballs.h"
#include "ofxVectorMath.h"

class MetaBallChunk {
	
public:
	
	MetaBallChunk();
	void updateChunkPos();
	void updateBallSizes();
	
	// fields for each metaballs
	CMetaballs*				m_pMetaballs;
	ofPoint*				ballPoints;
	ofPoint*				ballPointsPrev;
	ofPoint*				ballPointsPrev2;	
	float*					ballSizes;
	
	// fields for this chunk
	int						chunkID;
	ofxVec3f				chunkCurrPos;
	ofxVec3f				chunkDestPos;
	float					deceleration;
	float					sizeBase;
	
};