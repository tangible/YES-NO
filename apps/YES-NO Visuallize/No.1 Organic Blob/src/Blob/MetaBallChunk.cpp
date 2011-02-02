/*
 *  MetaBallChunk.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/02.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "MetaBallChunk.h"

MetaBallChunk::MetaBallChunk() {
	
	chunkCurrPos = ofxVec3f(0.0, 0.0, 0.0);
	chunkDestPos = ofxVec3f(10.0, 10.0, 0.0);
	deceleration = 0.99;
	sizeBase = 0.247;
	
}

void MetaBallChunk::updateChunkPos() {
	
	if (1.0 < chunkCurrPos.distance(chunkDestPos)) {
		float ampx = chunkDestPos.x - chunkCurrPos.x;
		float ampy = chunkDestPos.y - chunkCurrPos.y;
		float ampz = chunkDestPos.z - chunkCurrPos.z;
		float tmpampx = ampx * deceleration;
		float tmpampy = ampy * deceleration;
		float tmpampz = ampz * deceleration;
		chunkCurrPos.x += ampx - tmpampx;
		chunkCurrPos.y += ampy - tmpampy;
		chunkCurrPos.z += ampz - tmpampz;
	}else {
		chunkDestPos = ofxVec3f(ofRandom(-300, 300), 
								ofRandom(-300, 300), 
								ofRandom(-300, 300));
		
	}

//	cout << "distance = " + ofToString(chunkCurrPos.distance(chunkDestPos)) << endl;
//	cout << "chunk curr x = " + ofToString(chunkCurrPos.x) << endl;
//	cout << "chunk curr y = " + ofToString(chunkCurrPos.y) << endl;
//	cout << "chunk dest x = " + ofToString(chunkDestPos.x) << endl;
//	cout << "chunk dest y = " + ofToString(chunkDestPos.y) << endl;
//	cout << " " << endl;
	
}