/*
 *  BlobManager.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/01/31.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "BlobManager.h"
int baseSphereSize = 10;
int maxSphereSize = 95;//125;
void BlobManager::setup(int _fps, AdminPanel* _admin, ofxCamera* cam, QuestionImage* _qImage, StateText* _sText) {
	
	bgPlayer = new ofVideoPlayer();	
	
	fps = _fps;
	admin = _admin;
	qImage = _qImage;
	sText = _sText;
	
	shader.setup("glsl");
	ofDisableArbTex();	
	
	// init metaball and voxels
	bullet = new ofxBullet();
	bullet->initPhysics(ofxVec3f(0, 0, 0), false);	

	nMetaBalls = 18;	
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
				rdmPos = ofxVec3f(ofGetScreenWidth()/2+ofRandom(500, 200), ofGetScreenHeight()/2+ofRandom(-200, 200), ofRandom(-500, 0));
			} else {
				rdmPos = ofxVec3f(ofGetScreenWidth()/2+ofRandom(-500, -200), ofGetScreenHeight()/2+ofRandom(-200, 200), ofRandom(-500, 0));				
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
	screenW = ofGetScreenWidth();
	screenH = ofGetScreenHeight();
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
//	bg.loadImage("vancityWBBlur.jpg");
//	blobTex.loadImage("sky.jpeg");
	glActiveTexture(GL_TEXTURE1);
	texSlot = 1;
	//glBindTexture(GL_TEXTURE_2D, blobTex.getTextureReference().getTextureData().textureID);	
	glActiveTexture(GL_TEXTURE0);		
	
	bgCenter = ofxVec3f(ofGetScreenWidth()/2-bg.getWidth()/2, ofGetScreenHeight()/2-bg.getHeight()/2, 0);
	
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
	yesDiff = 0.0;
	noDiff = 0.0;
	
	ofAddListener(admin->onClearBG, this, &BlobManager::onClearBG);	
	
	soundYes.loadSound("key_omin_a.aif");
	soundNo.loadSound("A_kick.aif");
	soundYes.setMultiPlay(true);
	soundNo.setMultiPlay(true);
	
	//prevColAngle = ofRandomuf();
	ofColor iniYCol;
	iniYCol.r = 0.0; iniYCol.g = 0.5; iniYCol.b = 0.5;
	chunkCol.setColor(iniYCol);
	chunkCol.update();
	nextYesColAngle = chunkCol.getColorAngle()+0.08;
	prevColAngle = chunkCol.getColorAngle();
	ofColor iniNCol;
	iniNCol.r = 0.5; iniNCol.g = 0.0; iniNCol.b = 0.0;
	chunkCol.setColor(iniNCol);
	chunkCol.update();
	nextNoColAngle = nextYesColAngle+0.5;//chunkCol.getColorAngle()+0.08;

}

void BlobManager::update() {

	if (isVidBG) {
		bgPlayer->update();
		if (bgPlayer->bLoaded && !bgPlayer->isFrameNew()) bgPlayer->play();			
	}
	if (isVidBlobTex) {
		blobTexPlayer.update();
		if (blobTexPlayer.bLoaded && !blobTexPlayer.isFrameNew()) blobTexPlayer.play();		
	}
	
	// update metaball point locations
	if (admin->TOGGLEMOTION) {

		float minRad = 330;
		float maxRad = 330;
		float maxRadFactor = 1.2;
		int maxSMSnum = 5000;
		for (int i = 0; i < inSMSs.size(); i++) {
			if (inSMSs[i]->chunkID == 0) {
				float total = ofClamp(upInfo.numTotalYes, 0, maxSMSnum);
				float tmpSize = yesDiff*total;
				float maxSizeParam = 1.0*maxSMSnum;
				float radBase = ofMap(tmpSize, 0.0, maxSizeParam, minRad, maxRad);
				float diffFactor = ofMap(yesDiff, 0.0, 1.0, 0.5, maxRadFactor);
				radBase *= diffFactor;				
				
				inSMSs[i]->update(centroidYes, radBase);
			}else if (inSMSs[i]->chunkID == 1) {
				float total = ofClamp(upInfo.numTotalNo, 0, maxSMSnum);
				float tmpSize = noDiff*total;
				float maxSizeParam = 1.0*maxSMSnum;
				float radBase = ofMap(tmpSize, 0.0, maxSizeParam, minRad, maxRad);
				float diffFactor = ofMap(noDiff, 0.0, 1.0, 0.5, maxRadFactor);
				radBase *= diffFactor;					
				
				inSMSs[i]->update(centroidNo, radBase);
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
		
		// get radius
//		for (int i = 0; i < sphrersForChunk.size(); i++) {
//			float radAccum = 0.0;			
//			vector<MyRigidBody*> spheres = sphrersForChunk[i];
//			
//			for (int j = 0; j < nMetaBalls; j++) {
//				MyRigidBody* sph = spheres[j];
//				ofxVec3f pos = sph->getBodyPos();
//				float rad = pos.distance((i==0)?centroidYes:centroidNo);
//				radAccum += rad;
//			}
//			radAccum /= nMetaBalls;
//			(i==0)?radiusYes:radiusNo = radAccum;
//		}
		
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
				
				ofxVec3f ofForce = -sph->getBodyPos()+ofxVec3f(ofGetScreenWidth()/2 + mChunk->chunkCurrPos.x, ofGetScreenHeight()/2 + mChunk->chunkCurrPos.y, mChunk->chunkCurrPos.z);
//				if (0 == ofGetFrameNum()/ofRandom(100, 200)) {
//					ofForce += ofxVec3f(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 0);
//				}
				force.set(ofForce);

				force *= maxVal * 27;
				tangentVec = force.crossed(crossVec);
				tangentVec.normalize();
				tangentVec *= maxVal * 40;
				force += tangentVec;
				btVector3 btImpulse(force.x, force.y, force.z);
				sph->getRigidBody()->applyCentralImpulse(btImpulse);
				
				ofxVec3f impulse;
				impulse.set(ofRandomf(), ofRandomf(), ofRandomf());
				
				if (i == 0 && randomImpulsSMSRecievedYes != 0) {
					impulse *= 500 + randomImpulsSMSRecievedYes;
				}else if (i == 1 && randomImpulsSMSRecievedNo != 0) {
					impulse *= 500 + randomImpulsSMSRecievedNo;
				}else {
					int minImpulse = 20;
					int maxImpulse = 120;//80;
					int maxSMSnum = 5000;		
					float maxImpulseParam = 1.0*maxSMSnum;
					if (i == 0) {
						float total = upInfo.numTotalYes;
						total = ofClamp(total, 0, maxSMSnum);
						float tmpImpulse = yesDiff*total;
						float impulseY = ofMap(tmpImpulse, 0.0, maxImpulseParam, minImpulse, maxImpulse);
						//cout << "impulse Yes = "+ofToString(impulseY) << endl;
						impulse *= impulseY * 1.2;
					}else if (i == 1) {
						float total = upInfo.numTotalNo;
						total = ofClamp(total, 0, maxSMSnum);				
						float tmpImpulse = noDiff*total;				
						float impulseN = ofMap(tmpImpulse, 0.0, maxImpulseParam, minImpulse, maxImpulse);
						//cout << "impulse No = "+ofToString(impulseN) << endl;						
						impulse *= impulseN * 1.2;
					}					
				}

				btImpulse = btVector3(impulse.x, impulse.y, impulse.z);
				sph->getRigidBody()->applyCentralImpulse(btImpulse);
				
				ofxVec3f pos = sph->getBodyPos()-ofxVec3f(ofGetScreenWidth()/2, ofGetScreenHeight()/2, -400);
				float bx = (pos.x - boundsAvg.x) * boundsScaling;
				float by = (pos.y - boundsAvg.y) * boundsScaling;
				float bz = (pos.z - boundsAvg.z) * boundsScaling;
				
				mChunk->ballPoints[j].set(bx,by,bz);							
			}

			mChunk->m_pMetaballs->UpdateBallsFromPointsAndSizes(nMetaBalls, mChunk->ballPoints, mChunk->ballSizes);
			
		}
		if (0 != randomImpulsSMSRecievedYes) randomImpulsSMSRecievedYes = 0; // once enough
		if (0 != randomImpulsSMSRecievedNo) randomImpulsSMSRecievedNo = 0;
		
	}
}

void BlobManager::draw() {
	
	if (!admin->DRAWDEBUG) {
		
		// draw BG, qimage and statetext
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 1.0);
		
		ofPushMatrix();
		ofSetupScreen();
		if (isVidBG) {
			bgPlayer->draw(ofGetScreenWidth()/2-bgPlayer->getWidth()/2, 
						  ofGetScreenHeight()/2-bgPlayer->getHeight()/2);
		}else {
			bg.draw(ofGetScreenWidth()/2-bg.getWidth()/2, 
					ofGetScreenHeight()/2-bg.getHeight()/2);		
		}
		ofPopMatrix();
		
		
		ofPushMatrix();
		ofSetupScreen();		
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);		
		ofEnableSmoothing(); 	
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255);
		qImage->draw();	
		glDisable(GL_CULL_FACE);	
		ofEnableSmoothing(); 	
		//		sText->draw(upInfo);
		//		ofColor yCol;
		//		yCol.r = mBallChunks[0]->chunkCurrCol.x*255.0; yCol.g = mBallChunks[0]->chunkCurrCol.y*255.0; yCol.b = mBallChunks[0]->chunkCurrCol.z*255.0;
		//		ofColor nCol;
		//		nCol.r = mBallChunks[1]->chunkCurrCol.x*255.0; nCol.g = mBallChunks[1]->chunkCurrCol.y*255.0; nCol.b = mBallChunks[1]->chunkCurrCol.z*255.0;		
		//		sText->drawWithNoScale(upInfo, yCol, nCol);
		//		sText->drawWithNoScale(upInfo, sTextColYes, sTextColNo);
		sText->drawWithNoScale(upInfo);
		ofPopMatrix();
		ofDisableAlphaBlending();			
		
		
		setupGLStuff();		
		
		ofEnableAlphaBlending();		
			
		ofxSetSphereResolution(60);
		glDisable(GL_CULL_FACE);
		GLfloat lightPosition[] = {admin->LIGHTX, -admin->LIGHTY, admin->LIGHTZ, 0.0f};
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);		
		for (int i = 0; i < inSMSs.size(); i++) {
			ofxVec3f pos = inSMSs[i]->body->getBodyPos();
			ofColor col = inSMSs[i]->actualCol;
			ofSetColor(col.r, col.g, col.b);
			float size = inSMSs[i]->size;
			ofxSphere(pos.x, pos.y, pos.z, size);
		}			
		glEnable(GL_CULL_FACE);
		GLfloat lightPosition2[] = {admin->LIGHTX, admin->LIGHTY, admin->LIGHTZ, 0.0f};
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition2);		
		
		// draw metaball
		ofPushMatrix();
		float w = ofGetScreenWidth();
		float h = ofGetScreenHeight();
		float sz = 0.75*min(w,h);
		ofTranslate(w/2, h/2, 0);
		ofScale(sz,sz,sz);
		for (int i = 0; i < mBallChunks.size(); i++) {
			ofEnableSmoothing(); 	
			ofxVec4f basecol = mBallChunks[i]->chunkCurrCol;
			glColor3f(basecol.x, basecol.y, basecol.z);				
			mBallChunks[i]->m_pMetaballs->Render();
		}
		ofPopMatrix();		
		
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
//	ofEnableAlphaBlending();
    bool bDrawDropShadow = false;
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
		bgPlayer->stop();
		bgPlayer->close();
		bgPlayer->stop();
		bgPlayer->close();
		delete bgPlayer;
		bgPlayer = new ofVideoPlayer();		
		
	}else {
		bgPlayer->stop();
		bgPlayer->close();
		delete bgPlayer;
		bgPlayer = new ofVideoPlayer();		
		bgPlayer->loadMovie(path);
		bgPlayer->play();		
		isVidBG = true;
			
	}
}

void BlobManager::changeImgBlobTex(string path) {
	
	bool isImg = blobTex.loadImage(path);
	
	if (isImg) {
		blobTex.resize(ofGetScreenWidth(), ofGetScreenHeight());
		glActiveTexture(GL_TEXTURE1);
		texSlot = 1;
		//glBindTexture(GL_TEXTURE_2D, blobTex.getTextureReference().getTextureData().textureID);	
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
		//glBindTexture(GL_TEXTURE_2D, blobTexPlayer.getTextureReference().getTextureData().textureID);	
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
				
				int maxSMSnum = 5000;
				float maxSizeParam = 1.0*maxSMSnum;					
				float radius = 0.0;
				if (_chunkID == 0) {
					float total = upInfo.numTotalYes;
					total = ofClamp(total, 0, maxSMSnum);
					float tmpSize = yesDiff*total;
					thisSizeY = ofMap(tmpSize, 0.0, maxSizeParam, baseSphereSize, maxSphereSize);
					//cout << "radius Yes = "+ofToString(thisSizeY) << endl;
					radius = thisSizeY;
				}else if (_chunkID == 1) {
					float total = upInfo.numTotalNo;
					total = ofClamp(total, 0, maxSMSnum);				
					float tmpSize = noDiff*total;				
					thisSizeN = ofMap(tmpSize, 0.0, maxSizeParam, baseSphereSize, maxSphereSize);				
					//cout << "radius No = "+ofToString(thisSizeN) << endl;					
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

	if (0 == _chunkID) {
		randomImpulsSMSRecievedYes = ofMap(yesDiff, 0.0, 1.0, baseImpulse, 150);
	}else if (1 == _chunkID) {
		randomImpulsSMSRecievedNo = ofMap(noDiff, 0.0, 1.0, baseImpulse, 150);
	}
	
}

void BlobManager::onBallGetSMSrep(int& chunkID) {
	cout << "got event, chunkID=" + ofToString(chunkID) << endl;
	int preservedChunkID = chunkID;
	ofRemoveListener(inSMSs[inSMSs.size()-1]->onBallGetSMSrepEvent, this, &BlobManager::onBallGetSMSrep);
    delete inSMSs[inSMSs.size()-1];
	inSMSs.pop_back();
	
	float totalRatioYes = upInfo.ratioTotalYes;
	float totalRatioNo = upInfo.ratioTotalNo;
	float ratioYes = upInfo.ratioThisTimeYes;
	float ratioNo = upInfo.ratioThisTimeNo;
	float totalYes = upInfo.numTotalYes;
	float totalNo = upInfo.numTotalNo;
	
	float sd = scaleDiffMax;
	float diffForYes = ofClamp(upInfo.numDiffYes, -sd, sd);
	yesDiff = ofMap(diffForYes, -sd, sd, 0.1, 1.0);	
	diffForYes = ofMap(diffForYes, -sd, sd, minScale, maxScale);	
	if (preservedChunkID == 0 || totalYes < totalNo) 
		mBallChunks[0]->onSMSRecievedChangeMetaballSize(preservedChunkID, yesDiff, totalYes);
	
	sd = scaleDiffMax;
	float diffForNo = ofClamp(upInfo.numDiffNo, -sd, sd);
	noDiff = ofMap(diffForNo, -sd, sd, 0.1, 1.0);
	diffForNo = ofMap(diffForNo, -sd, sd, minScale, maxScale);		
	if (preservedChunkID == 1 || totalNo < totalYes) 	
		mBallChunks[1]->onSMSRecievedChangeMetaballSize(preservedChunkID, noDiff, totalNo);
	
	if (upInfo.numTotalYes != 0) 
		onSMSRecievedChangeSphereSize(0, diffForYes, diffForNo);
	if (upInfo.numTotalNo != 0) 	
		onSMSRecievedChangeSphereSize(1, diffForYes, diffForNo);
	

	prevColAngle += 0.08;
	chunkCol.setColorScale(1.0);
	chunkCol.setColorRadius(1.0);	
	chunkCol.setColorAngle(prevColAngle);
	chunkCol.update();
	nextYesColAngle = prevColAngle+0.08;
	ofColor yesCol = chunkCol.getColor();
	yesCol.r = yesCol.r/255.0; 	yesCol.g = yesCol.g/255.0; 	yesCol.b = yesCol.b/255.0;
	chunkCol.setColorScale(1.0);
	chunkCol.setColorRadius(1.0);	
	chunkCol.setColorAngle(prevColAngle+0.5);
	nextNoColAngle = nextYesColAngle+0.5;
	chunkCol.update();
	ofColor noCol = chunkCol.getColor();
	noCol.r = noCol.r/255.0; noCol.g = noCol.g/255.0; noCol.b = noCol.b/255.0;

	
	if (preservedChunkID == 0) {
		onSMSRecievedImpulseForSphere(0);
		mBallChunks[0]->onSMSRecievedChangeCol(preservedChunkID, 0, totalRatioYes, yesCol);
		soundYes.play();
		sText->startFadeToDefaultColorYes();
	}else if(preservedChunkID == 1) {
		onSMSRecievedImpulseForSphere(1);		
		mBallChunks[1]->onSMSRecievedChangeCol(preservedChunkID, 0, totalRatioNo, noCol);			
		soundNo.play();
		sText->startFadeToDefaultColorNo();		
	}
}

//void BlobManager::onBallGetSMSrepComplete(int& _chunkID) {
//	
//	int baseImpulse = 50;
//	int maxImpulse = 300;
//	if (0 == _chunkID) {
//		randomImpulsSMSRecievedYes = ofMap(yesDiff, 0.0, 1.0, baseImpulse, maxImpulse);		
//	}else if (1 == _chunkID) {
//		randomImpulsSMSRecievedNo = ofMap(noDiff, 0.0, 1.0, baseImpulse, maxImpulse);		
//	}
//	
//}

void BlobManager::recieveSMS(UpdateInfo _upInfo) {

	upInfo = _upInfo;
	float ratioYes = upInfo.ratioThisTimeYes;
	float ratioNo = upInfo.ratioThisTimeNo;	
	int yes = ratioYes*10;
	int no = ratioNo*10;
	
	for (int i = 0; i < 2; i++) {
		if (i == 0 && upInfo.numYes != 0) {
			IncomingSMS* sms = new IncomingSMS();
			sms->setup(bullet, 0, nextYesColAngle);
			ofAddListener(sms->onBallGetSMSrepEvent, this, &BlobManager::onBallGetSMSrep);
			inSMSs.push_back(sms);
			sText->updateColorYes(nextYesColAngle);
		}else if (i == 1 && upInfo.numNo != 0) {
			IncomingSMS* sms = new IncomingSMS();
			sms->setup(bullet, 1, nextNoColAngle);
			ofAddListener(sms->onBallGetSMSrepEvent, this, &BlobManager::onBallGetSMSrep);			
			inSMSs.push_back(sms);			
			sText->updateColorNo(nextNoColAngle);			
		}
	}	
	//updateStextCol();
}

void BlobManager::onClearBG(int& i) {
	if (isVidBG) {
		bgPlayer->stop();
		bgPlayer->close();
		delete bgPlayer;
		bgPlayer = new ofVideoPlayer();
		isVidBG = false;
		bg.clear();
	}else {
		bg.clear();
	}
}

