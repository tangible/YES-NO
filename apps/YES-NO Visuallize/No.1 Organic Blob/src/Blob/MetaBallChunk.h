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

class MetaBallChunk {
	
public:
	// fields for each metaballs
	CMetaballs*				m_pMetaballs;
	ofPoint*				ballPoints;
	ofPoint*				ballPointsPrev;
	ofPoint*				ballPointsPrev2;	
	float*					ballSizes;
	
	// fields for this chunk
	int						chunkID;
	ofPoint					chunkBasePos;
	
};