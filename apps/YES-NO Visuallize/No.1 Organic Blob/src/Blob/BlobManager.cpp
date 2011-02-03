/*
 *  BlobManager.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/01/31.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "BlobManager.h"

void BlobManager::setup(int _fps, AdminPanel* _admin) {
	
	fps = _fps;
	admin = _admin;
	shader.setup("glsl");
	ofDisableArbTex();	
	
	// init metaball and voxels
	bullet = new ofxBullet();
	bullet->initPhysics(ofxVec3f(0, 0, 0), false);	
    nMetaBalls = 10;	
	int numChunk = 2;
	for (int i = 0; i < numChunk; i++) {
		MetaBallChunk* mchunk = new MetaBallChunk(nMetaBalls, i);
		mBallChunks.push_back(mchunk);
		
		vector<MyRigidBody*> spheres;
		for (int j = 0; j < nMetaBalls; j++) {
			ofxVec3f rdmPos = ofxVec3f(ofGetWidth()/2+ofRandom(-100, 100), ofGetHeight()/2+ofRandom(-100, 100), ofRandom(-100, 100));
			MyRigidBody* sph = bullet->createSphere(rdmPos,
													ofRandom(50, 50), 
													1, 
													ofxVec4f(ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), 0.7), 
													DYNAMIC_BODY);		
			spheres.push_back(sph);
		}
		sphrersForChunk.push_back(spheres);		
	}
	CMarchingCubes::BuildTables();
    boundsAvg.x = boundsAvg.y = boundsAvg.z = 0;
    boundsScaling = 1.0 / 1020.0f;	
	
	// blur the motion stiffness
	counter = 0;
	counter2 = 0;
    blurValue  = 1000;	
    maxDeceleration = 0;
	
	// settings for shadow
	screenW = ofGetWidth();
	screenH = ofGetHeight();
	nScreenPixels = screenW * screenH;
	shadowDivX = 8;
	shadowDivY = 8;
	shadowW = screenW/shadowDivX;
	shadowH = screenH/shadowDivY;
	nShadowPixels = shadowW * shadowH;
	shadowTex.allocate(shadowW,shadowH,GL_LUMINANCE_ALPHA);
	screenDepth  = new float[nScreenPixels];
	for (int i=0; i<nScreenPixels; i++){
	    screenDepth[i] = 0;
	}
	shadowPixels   = new unsigned char[nShadowPixels];
	shadowPixelsLA = new unsigned char[nShadowPixels*2];
	for (int i=0; i<nShadowPixels; i++){
        shadowPixels[i] = 255;
        shadowPixelsLA[i] = 255;
        shadowPixelsLA[i+nShadowPixels] = 255;
	}
	shadowCvImage.allocate(shadowW, shadowH);	
	
	// load imgs for texture
	bg.loadImage("vancity.jpg");
	blobTex.loadImage("sky.jpeg");
	glActiveTexture(GL_TEXTURE1);
	texSlot = 1;
	glBindTexture(GL_TEXTURE_2D, blobTex.getTextureReference().getTextureData().textureID);	
	glActiveTexture(GL_TEXTURE0);		
	
	bgCenter = ofxVec3f(ofGetWidth()/2-bg.getWidth()/2, ofGetHeight()/2-bg.getHeight()/2, 0);
	
	// move bg
	bgXTween.setParameters(easingback, ofxTween::easeOut, 0, 0, 0, 0);
	bgYTween.setParameters(easingback, ofxTween::easeOut, 0, 0, 0, 0);
	
}

void BlobManager::update() {

	if (isVidBG) {
		bgPlayer.update();
		if (bgPlayer.bLoaded && !bgPlayer.isFrameNew()) bgPlayer.play();			
	}
	if (isVidBlobTex) {
		blobTexPlayer.update();
		if (blobTexPlayer.bLoaded && !blobTexPlayer.isFrameNew()) blobTexPlayer.play();		
	}
	
	// update metaball point locations
	if (admin->TOGGLEMOTION) {
		
		bullet->stepPhysicsSimulation(admin->PHYSICSTICKFPS);			
		
		float maxVal = 0.01;
		ofxVec3f force;
		ofxVec3f crossVec;
		crossVec.set(0, 0, 0);
		crossVec.rotate(ofGetFrameNum(), ofGetFrameNum(), ofGetFrameNum());
		ofxVec3f tangentVec;	
		int rdmIdx = (int)ofRandom(0, 21);
		
		counter2 = (counter2+1)%blurValue;
		counter  = counter2 + 1;
		
		float bx,by,bz;
		float px,py,pz;
		bx = by = bz = 0.5;
		float frac = (float)counter/(float)blurValue;
		frac = 0.50 + 0.50  * cos(TWO_PI * frac);
		frac = 0.05 + 0.95 * frac;
		frac = pow(frac, 4.0f);
		float A = frac;
		float B = 1.0-A;		
		
		for (int i = 0; i < mBallChunks.size(); i++) {
			
			MetaBallChunk* mChunk = mBallChunks[i];
			mChunk->updateChunkBasePos();
			mChunk->updateBallSizes();
			mChunk->updateColor();
			vector<MyRigidBody*> spheres = sphrersForChunk[i];
			
			for (int j = 0; j < spheres.size(); j++) {
				
				// apply force
				MyRigidBody* sph = spheres[j];
				
				force.set(-sph->getBodyPos() + 
						  ofxVec3f(ofGetWidth()/2 + mChunk->chunkCurrPos.x, ofGetHeight()/2 + mChunk->chunkCurrPos.y, 0));
				force *= maxVal * 15;
				tangentVec = force.crossed(crossVec);
				tangentVec.normalize();
				tangentVec *= maxVal*100;
				force += tangentVec;
				btVector3 btImpulse(force.x, force.y, force.z);
				sph->getRigidBody()->applyCentralImpulse(btImpulse);
				
				ofxVec3f impulse;
				impulse.set(ofRandomf(), ofRandomf(), ofRandomf());
				if (i == rdmIdx) {
					impulse *= ofRandom(-250.0, 250.0);
				}else {
					impulse *= 0;
				}
				btImpulse = btVector3(impulse.x, impulse.y, impulse.z);
				sph->getRigidBody()->applyCentralImpulse(btImpulse);
				
				// compute scaled coordinates
				if ((counter > 1)){
					ofxVec3f pos = sph->getBodyPos()-ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, -400);
					bx = (pos.x - boundsAvg.x) * boundsScaling;
					by = (pos.y - boundsAvg.y) * boundsScaling;
					bz = (pos.z - boundsAvg.z) * boundsScaling;
				}
				
				// stash the previous coordinates
				px = mChunk->ballPoints[j].x;
				py = mChunk->ballPoints[j].y;
				pz = mChunk->ballPoints[j].z;
				
				// compute blurred new coordinates
				bx = A*px + B*bx;
				by = A*py + B*by;
				bz = A*pz + B*bz;
				mChunk->ballPoints[j].set(bx,by,bz);				
			}		
			
			mChunk->m_pMetaballs->UpdateBallsFromPointsAndSizes(spheres.size(), mChunk->ballPoints, mChunk->ballSizes);
			
		}
	}
}

float xidx = 0.0;
float yidx = 0.0;
void BlobManager::draw() {

	// draw BG
    glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	if (!isVidBG) {
//		xidx += ofRandom(0.01, -0.01);
//		yidx += ofRandom(0.01, -0.01);
//		float x = ofSignedNoise(xidx)*(ofGetWidth()/2-bg.getWidth()/2);
//		float y = ofSignedNoise(yidx)*(ofGetHeight()/2-bg.getHeight()/2);
//		bg.draw(bgCenter.x+x, bgCenter.y+y);
		
		bg.draw(ofGetWidth()/2-bg.getWidth()/2+bgXTween.update(), 
				ofGetHeight()/2-bg.getHeight()/2+bgYTween.update());
	}else {
		bgPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
	glEnable(GL_LIGHTING);
		
	setupGLStuff();
	
    // draw metaball
	ofEnableAlphaBlending();

		glPushMatrix();
		float w = ofGetWidth();
		float h = ofGetHeight();
		float sz = 0.75*min(w,h);
		glTranslatef(w/2,h/2,0);
		glScalef(sz,sz,sz);

		for (int i = 0; i < mBallChunks.size(); i++) {
			ofxVec4f basecol = mBallChunks[i]->chunkCurrCol;
			glColor3f(basecol.x, basecol.y, basecol.z);
			shader.begin();
			shader.setUniform1i("tex", texSlot);
			shader.setUniform1f("tex_col_mixRatio", admin->TEXCOLMIXRATIO);
			shader.setUniform1f("blob_transparency", admin->BLOBTRANSPARENCY);				
			mBallChunks[i]->m_pMetaballs->Render();
			shader.end();			

		}

		glPopMatrix();

	ofDisableAlphaBlending();
	
	
	// draw shadow
    bool bDrawDropShadow = true;
    if (bDrawDropShadow){
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glColor4f(0, 0.002, 0.005, admin->SHADOWINTENSITY);
        glEnable(GL_BLEND);
        glDisable(GL_LIGHTING);
        glDisable(GL_POLYGON_SMOOTH);
        glDisable(GL_CULL_FACE);
        glDisable(GL_COLOR_MATERIAL);
        glReadPixels(0,0,screenW,screenH, GL_DEPTH_COMPONENT, GL_FLOAT, screenDepth); // fuck slow suck
		
        int shadowIndex = 0;
        int screenIndex = 0;
        int row = 0;
		
        for (int y=0; y<shadowH; y++){
            row = y*shadowDivY*screenW;
            for (int x=0; x<shadowW; x++){
                screenIndex = row + x*shadowDivX;
                shadowPixels[shadowIndex++] = (screenDepth[screenIndex] < 1.0)? 255:0;
            }
        }
        shadowCvImage.setFromPixels(shadowPixels, shadowW, shadowH);
        shadowCvImage.blurGaussian( 21 );
        unsigned char *blurred = shadowCvImage.getPixels();
        for (int i=0; i<nShadowPixels; i++){
            shadowPixelsLA[i*2  ] = blurred[i];
            shadowPixelsLA[i*2+1] = blurred[i];
        }
		
        shadowTex.loadData(shadowPixelsLA, shadowW,shadowH, GL_LUMINANCE_ALPHA);
        glPushMatrix();
		glTranslatef(0.53*screenW+admin->SHADOWPOSX, 0.73*screenH+admin->SHADOWPOSY, 0);
		glScalef(admin->SHADOWSCALE, admin->SHADOWSCALE, 1);
		if (!(!admin->SHADOWROTX && !admin->SHADOWROTY && !admin->SHADOWROTZ))
			glRotatef(admin->SHADOWROTDEG, (admin->SHADOWROTX)?1:0, (admin->SHADOWROTY)?1:0, (admin->SHADOWROTZ)?1:0);
		glTranslatef(-shadowW/2, shadowH/2, 0);
		shadowTex.draw(0, 0, shadowW, 0-shadowH);
        glPopMatrix();
		
        glPopAttrib();
    }
	glDisable(GL_DEPTH_TEST);
	
}

void BlobManager::setupGLStuff(){
	
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
	
    glDisable(GL_BLEND);
    glPolygonMode(GL_BACK, GL_FILL );
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glShadeModel(GL_SMOOTH);
	
    //glColor3f(admin->BLOBBASECOL[0], admin->BLOBBASECOL[1], admin->BLOBBASECOL[2]);
    GLfloat on[]  = {1.0};
    GLfloat off[] = {0.0};
    glLightModelfv( GL_LIGHT_MODEL_TWO_SIDE, on);
	
    GLfloat shininess[] = {admin->BLOBMATERIALSHINENESS};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, admin->BLOBMATERIALAMBIENT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, admin->BLOBMATERIALDIFFUSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, admin->BLOBMATERIALSPECULAR);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
 
	GLfloat lightPosition[] = {admin->LIGHTX, admin->LIGHTY, admin->LIGHTZ, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, admin->LIGHTDIFFUSE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, admin->LIGHTSPECULAR);
    glLightfv(GL_LIGHT0, GL_AMBIENT, admin->LIGHTAMBIENT);		
	
}

void BlobManager::changeImgBG(string path) {
	
	bool isImg = bg.loadImage(path);
	
	if (isImg) {
		//bg.resize(ofGetWidth(), ofGetHeight());			
		isVidBG = false;
		bgPlayer.stop();
		bgPlayer.close();
		
	}else {
		bgPlayer.loadMovie(path);
		bgPlayer.play();		
		isVidBG = true;
			
	}
}

void BlobManager::changeImgBlobTex(string path) {
	
	bool isImg = blobTex.loadImage(path);
	
	if (isImg) {
		blobTex.resize(ofGetWidth(), ofGetHeight());
		glActiveTexture(GL_TEXTURE1);
		texSlot = 1;
		glBindTexture(GL_TEXTURE_2D, blobTex.getTextureReference().getTextureData().textureID);	
		glActiveTexture(GL_TEXTURE0);			
		
		isVidBlobTex = false;
		blobTexPlayer.stop();
		blobTexPlayer.close();
		
	}else {
		blobTexPlayer.loadMovie(path);
		blobTexPlayer.play();		
		isVidBlobTex = true;
		
		glActiveTexture(GL_TEXTURE1);
		texSlot = 1;
		glBindTexture(GL_TEXTURE_2D, blobTexPlayer.getTextureReference().getTextureData().textureID);	
		glActiveTexture(GL_TEXTURE0);		
	}
}

void BlobManager::moveBG() {
	
	if (bgXTween.isCompleted()) {
		
		float cur = bgXTween.getTarget(0);
		float x = ofRandom(-430, 430);
		float dur = ofRandom(6000, 8000);
		bgXTween.setParameters(easingquart, ofxTween::easeInOut, cur, x, dur, 0);
		
		cur = bgYTween.getTarget(0);
		float y = ofRandom(-130, 130);
		bgYTween.setParameters(easingquart, ofxTween::easeInOut, cur, y, dur, 0);
		
	}
	
	
}

void BlobManager::recieveSMS(UpdateInfo upInfo) {

//	for (int i = 0; i < mBallChunks.size(); i++) {
//		mBallChunks[i]->onSMSRecieved();
//	}
	mBallChunks[0]->onSMSRecieved(upInfo.ratioThisTimeYes, upInfo.ratioTotalYes);
	mBallChunks[1]->onSMSRecieved(upInfo.ratioThisTimeNo, upInfo.ratioTotalNo);
	
}






