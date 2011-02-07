/*
 *  BlobManager.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/01/31.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "BlobManager.h"
int baseSphereSize = 30;
int maxSphereSize = 150;
void BlobManager::setup(int _fps, AdminPanel* _admin, QuestionImage* _qImage, StateText* _sText) {
	
	for (int i = 0; i < 2; i++) {
		Ray* r = new Ray();
		if (i == 0) r->setup(centroidYes);
		if (i == 1) r->setup(centroidNo);
		rays.push_back(r);
	}	
	
	fps = _fps;
	admin = _admin;
	qImage = _qImage;
	sText = _sText;
	shader.setup("glsl");
	ofDisableArbTex();	
	
	// init sms
	for (int i = 0; i < 2; i++) {
		Flock* f = new Flock();
		f->flockID = i;
		flocks.push_back(f);

	}		
	
	// init metaball and voxels
	bullet = new ofxBullet();
	bullet->initPhysics(ofxVec3f(0, 0, 0), false);	
    nMetaBalls = 10;	
	int numChunk = 2;
	for (int i = 0; i < numChunk; i++) {
		// zero = YES, one = NO.
		MetaBallChunk* mchunk = new MetaBallChunk(nMetaBalls, i);
		mBallChunks.push_back(mchunk);
		
		vector<MyRigidBody*> spheres;
		vector<float> baseSizes;
		for (int j = 0; j < nMetaBalls; j++) {
			
			ofxVec3f rdmPos;
			if (i == 0) {
				rdmPos = ofxVec3f(ofGetWidth()/2+ofRandom(500, 200), ofGetHeight()/2+ofRandom(-200, 200), ofRandom(-500, 0));
			} else {
				rdmPos = ofxVec3f(ofGetWidth()/2+ofRandom(-500, -200), ofGetHeight()/2+ofRandom(-200, 200), ofRandom(-500, 0));				
			}

			baseSizes.push_back(baseSphereSize);
			MyRigidBody* sph = bullet->createSphere(rdmPos,
													baseSphereSize, 
													1, 
													ofxVec4f(ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), 0.7), 
													DYNAMIC_BODY);		
			spheres.push_back(sph);
		}
		sphrersForChunk.push_back(spheres);	
		sphrersForChunkBaseSize.push_back(baseSizes);
	}
	CMarchingCubes::BuildTables();
    boundsAvg.x = boundsAvg.y = boundsAvg.z = 0;
    boundsScaling = 1.0 / 1020.0f;	
	
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
	bg.loadImage("vancityWBBlur.jpg");
	blobTex.loadImage("sky.jpeg");
	glActiveTexture(GL_TEXTURE1);
	texSlot = 1;
	glBindTexture(GL_TEXTURE_2D, blobTex.getTextureReference().getTextureData().textureID);	
	glActiveTexture(GL_TEXTURE0);		
	
	bgCenter = ofxVec3f(ofGetWidth()/2-bg.getWidth()/2, ofGetHeight()/2-bg.getHeight()/2, 0);
	
	// move bg
	bgXTween.setParameters(easingback, ofxTween::easeOut, 0, 0, 0, 0);
	bgYTween.setParameters(easingback, ofxTween::easeOut, 0, 0, 0, 0);

	// sms param
	randomImpulsSMSRecievedYes = 0;
	randomImpulsSMSRecievedNo = 0;
	smsYes = NULL;
	smsNo = NULL;
	
	upInfo.ratioTotalYes = 0;
	upInfo.ratioTotalNo = 0;
	upInfo.ratioThisTimeYes = 0;
	upInfo.ratioThisTimeNo = 0;
	upInfo.numTotalYes = 0;
	upInfo.numTotalNo = 0;
	upInfo.numYes = 0;
	upInfo.numNo = 0;
	upInfo.requesttime = "";
	
	ofAddListener(flocks[0]->onBallGetSMSrepEvent, this, &BlobManager::onBallGetSMSrep);
	ofAddListener(flocks[1]->onBallGetSMSrepEvent, this, &BlobManager::onBallGetSMSrep);	
	
	
	timg.loadImage("blob.png");
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
		
		for (int i = 0; i < flocks.size(); i++) {
			Flock* f = flocks[i];
			int flockID = f->flockID;
			if (flockID == 0) {
				f->z = centroidYes.z;
				f->update(centroidYes.x, centroidYes.y, centroidYes.z);
			}else if (flockID == 1) {
				f->z = centroidNo.z;				
				f->update(centroidNo.x, centroidNo.y, centroidNo.z);
			}
		}
				
		bool a = false;
		bool b = false;		
		for (int i = 0; i < rays.size(); i++) {
			if (i == 0) {
				bool near = rays[0]->update(centroidYes);
//				if (near) {
//					Ray* r = rays[0];
//					delete r;
//					a = true;
//					rays.erase(rays.begin()+0);
//				}
			}else if (i == 1) {
				bool near = rays[1]->update(centroidNo);
//				if (near) {
//					Ray* r = rays[1];
//					delete r;
//					b = true;
//					rays.erase(rays.begin()+1);					
//				}				
			}
		}

		
		bullet->stepPhysicsSimulation(admin->PHYSICSTICKFPS);	
		
		// get centroid
		for (int i = 0; i < sphrersForChunk.size(); i++) {
			vector<MyRigidBody*> spheres = sphrersForChunk[i];
			float xxx = 0.0; 
			float yyy = 0.0;
			float zzz = 0.0;
			
			for (int j = 0; j < nMetaBalls; j++) {
				MyRigidBody* sph = spheres[j];
				ofxVec3f pos = sph->getBodyPos();
				xxx += pos.x;
				yyy += pos.y;
				zzz += pos.z;
				
			}

			float xx = 0.0;
			float yy = 0.0;
			float zz = 0.0;
			xx = xxx/nMetaBalls;
			yy = yyy/nMetaBalls;
			zz = zzz/nMetaBalls;
			
			if (i == 0) {
				centroidYes = ofxVec3f(xx, yy, zz);	
			}else if (i == 1) {
				centroidNo = ofxVec3f(xx, yy, zz);
			}
		}
		
		
		float maxVal = 0.01;
		ofxVec3f force;
		ofxVec3f crossVec;
		crossVec.set(0, 0, 0);
		crossVec.rotate(ofGetFrameNum(), ofGetFrameNum(), ofGetFrameNum());
		ofxVec3f tangentVec;	
		int rdmIdx = (int)ofRandom(0, 21);	
		
		for (int i = 0; i < mBallChunks.size(); i++) {
			
			MetaBallChunk* mChunk = mBallChunks[i];
			mChunk->updateChunkBasePos();
			mChunk->updateBallSizes();
			mChunk->updateColor();
			vector<MyRigidBody*> spheres = sphrersForChunk[i];
			
			// position and size process
			for (int j = 0; j < nMetaBalls; j++) {
				
				// apply force
				MyRigidBody* sph = spheres[j];
				
				force.set(-sph->getBodyPos() + 
						  ofxVec3f(ofGetWidth()/2 + mChunk->chunkCurrPos.x, ofGetHeight()/2 + mChunk->chunkCurrPos.y, mChunk->chunkCurrPos.z));
				force *= maxVal * 40;
				tangentVec = force.crossed(crossVec);
				tangentVec.normalize();
				tangentVec *= maxVal*100;
				force += tangentVec;
				btVector3 btImpulse(force.x, force.y, force.z);
				sph->getRigidBody()->applyCentralImpulse(btImpulse);
				
				ofxVec3f impulse;
				impulse.set(ofRandomf(), ofRandomf(), ofRandomf());
				
				if (i == 0 && randomImpulsSMSRecievedYes != 0) {
					impulse *= 1100 + randomImpulsSMSRecievedYes;
				}else if (i == 1 && randomImpulsSMSRecievedNo != 0) {
					impulse *= 1100 + randomImpulsSMSRecievedNo;
				}else {
					impulse *= 80;
				}

				btImpulse = btVector3(impulse.x, impulse.y, impulse.z);
				sph->getRigidBody()->applyCentralImpulse(btImpulse);
				
				ofxVec3f pos = sph->getBodyPos()-ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, -400);
				float bx = (pos.x - boundsAvg.x) * boundsScaling;
				float by = (pos.y - boundsAvg.y) * boundsScaling;
				float bz = (pos.z - boundsAvg.z) * boundsScaling;
				
				mChunk->ballPoints[j].set(bx,by,bz);							
			}

			mChunk->m_pMetaballs->UpdateBallsFromPointsAndSizes(nMetaBalls, mChunk->ballPoints, mChunk->ballSizes);
			
		}
		if (0 != randomImpulsSMSRecievedYes) randomImpulsSMSRecievedYes = 0; // once enough
		if (0 != randomImpulsSMSRecievedNo) randomImpulsSMSRecievedNo = 0;
		
//		if (smsYes) {
//			for (int i = 0; i < smsYesBody.size(); i++) {
//				
//				// apply force
//				MyRigidBody* sph = smsYesBody[i];		
//				
//				ofxVec3f flockpos = mBallChunks[0]->chunkCurrPos;
//				
//				force.set(-sph->getBodyPos() + 
//						  ofxVec3f(ofGetWidth()/2 + flockpos.x, ofGetHeight()/2 + flockpos.y, flockpos.z));
//				force *= maxVal * 40;	
//				btVector3 btImpulse(force.x, force.y, force.z);
//				sph->getRigidBody()->applyCentralImpulse(btImpulse);				
//				
//				ofxVec3f impulse;
//				impulse.set(ofRandomf(), ofRandomf(), ofRandomf());
//				impulse *= 80;
//				btImpulse = btVector3(impulse.x, impulse.y, impulse.z);
//				sph->getRigidBody()->applyCentralImpulse(btImpulse);
//				
//				ofxVec3f pos = sph->getBodyPos()-ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, -400);
//				float bx = (pos.x - boundsAvg.x) * boundsScaling;
//				float by = (pos.y - boundsAvg.y) * boundsScaling;
//				float bz = (pos.z - boundsAvg.z) * boundsScaling;
//				
//				smsYes->ballPoints[i].set(bx,by,bz);				
//
//			}
//			smsYes->m_pMetaballs->UpdateBallsFromPointsAndSizes(smsYesBody.size(), smsYes->ballPoints, smsYes->ballSizes);
//		}
	}
}

void BlobManager::draw() {
	
	if (!admin->DRAWDEBUG) {
		
		// draw BG, qimage and statetext
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 1.0);
		if (!isVidBG) {
			bg.draw(ofGetWidth()/2-bg.getWidth()/2+bgXTween.update(), 
					ofGetHeight()/2-bg.getHeight()/2+200+bgYTween.update());
		}else {
			bgPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
		}
		
		qImage->draw();

		glEnable(GL_LIGHTING);	
		
//		for (int i = 0; i < rays.size(); i++) {
//			rays[i]->draw();
//		}
		
		
		setupGLStuff();		
		
		ofEnableAlphaBlending();		
			
			// draw metaball
			ofPushMatrix();
			float w = ofGetWidth();
			float h = ofGetHeight();
			float sz = 0.75*min(w,h);
			ofTranslate(w/2, h/2, 0);
			ofScale(sz,sz,sz);

			for (int i = 0; i < mBallChunks.size(); i++) {
				ofxVec4f basecol = mBallChunks[i]->chunkCurrCol;
				glColor3f(basecol.x, basecol.y, basecol.z);
//				shader.begin();
//				shader.setUniform1i("tex", texSlot);
//				shader.setUniform1f("tex_col_mixRatio", admin->TEXCOLMIXRATIO);
//				shader.setUniform1f("blob_transparency", admin->BLOBTRANSPARENCY);				
				mBallChunks[i]->m_pMetaballs->Render();
//				shader.end();			

			}
		
			ofPopMatrix();		
		
			// draw sms
			for (int i = 0; i < flocks.size(); i++) {
				Flock* f = flocks[i];
				ofxVec3f pos = f->draw();
//				ofPushMatrix();
//				ofTranslate(pos.x, pos.y, pos.z);
//				glDisable(GL_LIGHTING);
//				timg.draw(0, 0);
//				glEnable(GL_LIGHTING);
//				ofPopMatrix();
			}		
		
		ofDisableAlphaBlending();

	// debug	
	}else {
	
		ofSetColor(255, 0, 0);
		ofxSphere(centroidYes.x, centroidYes.y, centroidYes.z, 30);
		ofxSphere(centroidNo.x, centroidNo.y, centroidNo.z, 30);
		ofSetColor(255, 255, 255);	
		
		for (int i = 0; i < sphrersForChunk.size(); i++) {
			vector<MyRigidBody*> spheres = sphrersForChunk[i];
			
			for (int j = 0; j < spheres.size(); j++) {
				MyRigidBody* mrb = spheres[j];
				mrb->render(bullet->getWorld());
			}
		}

	}
	
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
	
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	sText->draw(upInfo, centroidYes, centroidNo);
	glDisable(GL_LIGHTING);
	
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
	
	if (bgYTween.isCompleted()) {
		
		float cur = bgXTween.getTarget(0);
		float x = ofRandom(-230, 230);
		float dur = ofRandom(10000, 12000);
		bgXTween.setParameters(easingquart, ofxTween::easeInOut, cur, x, dur, 0);
		
		cur = bgYTween.getTarget(0);
		float y = ofRandom(-230, 230);
		bgYTween.setParameters(easingquart, ofxTween::easeInOut, cur, y, dur, 0);
		
	}
	
	
}

void BlobManager::onSMSRecievedChangeSphereSize(int _chunkID, float totalRatioYes, float totalRatioNo) {
	
	for (int i = 0; i < mBallChunks.size(); i++) {
		if (i == _chunkID) {
			MetaBallChunk* mChunk = mBallChunks[i];
			vector<MyRigidBody*> spheres = sphrersForChunk[i];		
			vector<MyRigidBody*> swapSpheres;
			vector<float> baseSizes = sphrersForChunkBaseSize[i];
			for (int j = 0; j < nMetaBalls; j++) {
				MyRigidBody* sph = spheres[j];
				ofxVec3f pos = sph->getBodyPos();
				float baseSize = baseSizes[j];
				
				float radius = 0.0;
				if (i == 0) {
					thisSizeY = ofMap(totalRatioYes, 0.0, 1.0, baseSphereSize, maxSphereSize);
					radius = thisSizeY;
				}else if (i == 1) {
					thisSizeN = ofMap(totalRatioNo, 0.0, 1.0, baseSphereSize, maxSphereSize);				
					radius = thisSizeN;
				}
				
				MyRigidBody* swapSph = bullet->createSphere(pos,
															radius, 
															1, 
															ofxVec4f(0.5,0.5,0.5,0.7), 
															DYNAMIC_BODY);
				swapSpheres.push_back(swapSph);
				sph->remove(bullet->getWorld());
			}
			sphrersForChunk[i].swap(swapSpheres);
		}
	}		
}

void BlobManager::onSMSRecievedImpulseForSphere(int _chunkID) {
	
	float totalRatioYes = upInfo.ratioTotalYes;
	float totalRatioNo = upInfo.ratioTotalNo;
	float ratioYes = upInfo.ratioThisTimeYes;
	float ratioNo = upInfo.ratioThisTimeNo;	
	float factAY = 1.0-totalRatioYes;
	float factAN = 1.0-totalRatioNo;
	float factBY = 1.0-ratioYes;
	float factBN = 1.0-ratioNo;
	factAY = (factAY<=0.1)?0.1:factAY;
	factBY = (factBY<=0.1)?0.1:factBY;
	factAN = (factAN<=0.1)?0.1:factAN;
	factBN = (factBN<=0.1)?0.1:factBN;	
	
	int baseImpulse = 50;
//	float inpulseDivY = thisSizeY/factAY+thisSizeY/factBY;
//	float inpulseDivN = thisSizeN/factAN+thisSizeN/factBN;
	float inpulseDivY = thisSizeY/ratioYes;
	float inpulseDivN = thisSizeN/ratioNo;
	
	if (0 == _chunkID) {
		randomImpulsSMSRecievedYes = ofMap(ratioYes, 0.0, 1.0, baseImpulse, inpulseDivY);
	}else if (1 == _chunkID) {
		randomImpulsSMSRecievedNo = ofMap(ratioNo, 0.0, 1.0, baseImpulse, inpulseDivN);
	}
	
}

void BlobManager::onBallGetSMSrep(int& chunkID) {
	
	float totalRatioYes = upInfo.ratioTotalYes;
	float totalRatioNo = upInfo.ratioTotalNo;
	float ratioYes = upInfo.ratioThisTimeYes;
	float ratioNo = upInfo.ratioThisTimeNo;
	
	ofColor testcol;
	testcol.r = ofRandom(0.0, 1.0);
	testcol.g = ofRandom(0.0, 1.0);
	testcol.b = ofRandom(0.0, 1.0);
	
	if (chunkID == 0) {
		mBallChunks[0]->onSMSRecievedChangeCol(ratioYes, totalRatioYes, testcol);
		mBallChunks[0]->onSMSRecievedChangeMetaballSize(ratioYes, totalRatioYes);	
		onSMSRecievedImpulseForSphere(0);
		sText->onSMSReceivedUpdate(0, upInfo);
		
		//if (totalRatioYes > 0.5)
			onSMSRecievedChangeSphereSize(0, totalRatioYes, totalRatioNo);		
		
	}else if (chunkID == 1) {
		mBallChunks[1]->onSMSRecievedChangeCol(ratioNo, totalRatioNo, testcol);	
		mBallChunks[1]->onSMSRecievedChangeMetaballSize(ratioNo, totalRatioNo);		
		onSMSRecievedImpulseForSphere(1);
		sText->onSMSReceivedUpdate(1, upInfo);

		//if (totalRatioNo > 0.5)		
			onSMSRecievedChangeSphereSize(1, totalRatioYes, totalRatioNo);		
	}
	
	
}

void BlobManager::onBallGetSMSrepComplete(int& _chunkID) {
	
	float totalRatioYes = upInfo.ratioTotalYes;
	float totalRatioNo = upInfo.ratioTotalNo;
	float ratioYes = upInfo.ratioThisTimeYes;
	float ratioNo = upInfo.ratioThisTimeNo;	
	float factAY = 1.0-totalRatioYes;
	float factAN = 1.0-totalRatioNo;
	float factBY = 1.0-ratioYes;
	float factBN = 1.0-ratioNo;
	factAY = (factAY<=0.1)?0.1:factAY;
	factBY = (factBY<=0.1)?0.1:factBY;
	factAN = (factAN<=0.1)?0.1:factAN;
	factBN = (factBN<=0.1)?0.1:factBN;	
	
	int baseImpulse = 50;
	float inpulseDivY = thisSizeY/factAY+thisSizeY/factBY;
	float inpulseDivN = thisSizeN/factAN+thisSizeN/factBN;
	
	if (0 == _chunkID) {
		randomImpulsSMSRecievedYes = ofMap(ratioYes, 0.0, 1.0, baseImpulse, inpulseDivY);
	}else if (1 == _chunkID) {
		randomImpulsSMSRecievedNo = ofMap(ratioNo, 0.0, 1.0, baseImpulse, inpulseDivN);
	}
	
}

void BlobManager::recieveSMS(UpdateInfo _upInfo) {

	upInfo = _upInfo;
	float ratioYes = upInfo.ratioThisTimeYes;
	float ratioNo = upInfo.ratioThisTimeNo;	
	int yes = ratioYes*10;
	int no = ratioNo*10;
	
	for (int i = 0; i < 2; i++) {
		Flock* f = flocks[i];
		if (i == 0 && upInfo.numYes != 0) {
			f->addBoid(80+ofRandomuf()*20, 4+ofRandomuf()*10);
		}else if (i == 1 && upInfo.numNo != 0) {
			f->addBoid(80+ofRandomuf()*20, 4+ofRandomuf()*10);
		}
//		if (i == 0) {
//			f->addBoid(20/ofRandomuf(), 1.0/ofRandomuf());
//		}else if (i == 1) {
//			f->addBoid(20/ofRandomuf(), 1.0/ofRandomuf());
//		}		
	}	
	

	

	
}





