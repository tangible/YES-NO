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
int maxSphereSize = 125;
void BlobManager::setup(int _fps, AdminPanel* _admin, ofxCamera* cam, QuestionImage* _qImage, StateText* _sText) {
	
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

		for (int i = 0; i < inSMSs.size(); i++) {
			if (inSMSs[i]->chunkID == 0) {
				inSMSs[i]->update(centroidYes);
			}else if (inSMSs[i]->chunkID == 1) {
				inSMSs[i]->update(centroidNo);
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
						  ofxVec3f(ofGetScreenWidth()/2 + mChunk->chunkCurrPos.x, ofGetScreenHeight()/2 + mChunk->chunkCurrPos.y, mChunk->chunkCurrPos.z));
//				float fFactor = (i==YES) ? ofMap(yesDiff, 0.0, 1.0, 40, 30) : ofMap(noDiff, 0.0, 1.0, 40, 30); // min=10,max=60
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
					impulse *= (i==YES) ? ofMap(yesDiff, 0.0, 1.0, 30, 190) : ofMap(noDiff, 0.0, 1.0, 30, 190); // 80;
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
		bg.draw(ofGetScreenWidth()/2-bg.getWidth()/2+bgXTween.update(), 
				ofGetScreenHeight()/2-bg.getHeight()/2+200+bgYTween.update());
		
		ofPushMatrix();
		ofSetupScreen();		
		glDisable(GL_DEPTH_TEST);
		ofEnableSmoothing(); 	
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255);
		qImage->draw();	
		glDisable(GL_CULL_FACE);	
		ofEnableSmoothing(); 	
		sText->draw(upInfo);
		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		ofPopMatrix();
		ofDisableAlphaBlending();
		
		setupGLStuff();		
		
		ofEnableAlphaBlending();		
			
		for (int i = 0; i < inSMSs.size(); i++) {
			ofxVec3f pos = inSMSs[i]->body->getBodyPos();
			ofColor col = inSMSs[i]->actualCol;
			ofSetColor(col.r, col.g, col.b);
			float size = inSMSs[i]->size;
			ofxSphere(pos.x, pos.y, pos.z, size);
		}			
		
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

	if (0 == _chunkID) {
		randomImpulsSMSRecievedYes = ofMap(yesDiff, 0.0, 1.0, baseImpulse, 150);
	}else if (1 == _chunkID) {
		randomImpulsSMSRecievedNo = ofMap(noDiff, 0.0, 1.0, baseImpulse, 150);
	}
	
}

void BlobManager::onBallGetSMSrep(int& chunkID) {
	
	cout << "got event, chunkID=" + ofToString(chunkID) << endl;
	ofRemoveListener(inSMSs[inSMSs.size()-1]->onBallGetSMSrepEvent, this, &BlobManager::onBallGetSMSrep);
    delete inSMSs[inSMSs.size()-1];
	inSMSs.pop_back();
	
	float totalRatioYes = upInfo.ratioTotalYes;
	float totalRatioNo = upInfo.ratioTotalNo;
	float ratioYes = upInfo.ratioThisTimeYes;
	float ratioNo = upInfo.ratioThisTimeNo;
	
	float sd = scaleDiffMax;
	int yesdiff = upInfo.numDiffYes;
	float diffForYes = ofClamp(yesdiff, -sd, sd);
	yesDiff = ofMap(diffForYes, -sd, sd, 0.0, 1.0);	
	diffForYes = ofMap(diffForYes, -sd, sd, minScale, maxScale);		
	mBallChunks[0]->onSMSRecievedChangeMetaballSize(diffForYes, totalRatioYes);	
	sd = scaleDiffMax;
	int nodiff = upInfo.numDiffNo;
	float diffForNo = ofClamp(nodiff, -sd, sd);
	noDiff = ofMap(diffForNo, -sd, sd, 0.0, 1.0);
	diffForNo = ofMap(diffForNo, -sd, sd, minScale, maxScale);		
	mBallChunks[1]->onSMSRecievedChangeMetaballSize(diffForNo, totalRatioNo);
	
	if (upInfo.numTotalYes != 0) 
		onSMSRecievedChangeSphereSize(0, diffForYes, diffForNo);
	if (upInfo.numTotalNo != 0) 	
		onSMSRecievedChangeSphereSize(1, diffForYes, diffForNo);
	

	float rdmCol = ofRandomuf();
	yesColAng = rdmCol;
	chunkCol.setColorScale(1.0);
	chunkCol.setColorRadius(1.0);	
	chunkCol.setColorAngle(rdmCol);
	chunkCol.update();
	ofColor yesCol = chunkCol.getColor();
	yesCol.r = yesCol.r/255.0; 	yesCol.g = yesCol.g/255.0; 	yesCol.b = yesCol.b/255.0;
	noColAng = rdmCol+0.5;
	chunkCol.setColorScale(1.0);
	chunkCol.setColorRadius(1.0);	
	chunkCol.setColorAngle(rdmCol+0.5);
	chunkCol.update();
	ofColor noCol = chunkCol.getColor();
	noCol.r = noCol.r/255.0; noCol.g = noCol.g/255.0; noCol.b = noCol.b/255.0;
	
	if (chunkID == 0) {
		onSMSRecievedImpulseForSphere(0);
		mBallChunks[0]->onSMSRecievedChangeCol(0, totalRatioYes, yesCol);
	}else {
		onSMSRecievedImpulseForSphere(1);		
		mBallChunks[1]->onSMSRecievedChangeCol(0, totalRatioNo, noCol);	
	}
}

void BlobManager::onBallGetSMSrepComplete(int& _chunkID) {
	
	int baseImpulse = 50;
	int maxImpulse = 300;
	if (0 == _chunkID) {
		randomImpulsSMSRecievedYes = ofMap(yesDiff, 0.0, 1.0, baseImpulse, maxImpulse);		
	}else if (1 == _chunkID) {
		randomImpulsSMSRecievedNo = ofMap(noDiff, 0.0, 1.0, baseImpulse, maxImpulse);		
	}
	
}

void BlobManager::recieveSMS(UpdateInfo _upInfo) {

	upInfo = _upInfo;
	float ratioYes = upInfo.ratioThisTimeYes;
	float ratioNo = upInfo.ratioThisTimeNo;	
	int yes = ratioYes*10;
	int no = ratioNo*10;
	
	for (int i = 0; i < 2; i++) {
		if (i == 0 && upInfo.numYes != 0) {
			IncomingSMS* sms = new IncomingSMS();
			sms->setup(bullet, 0, yesColAng);
			ofAddListener(sms->onBallGetSMSrepEvent, this, &BlobManager::onBallGetSMSrep);			
			inSMSs.push_back(sms);
		}else if (i == 1 && upInfo.numNo != 0) {
			IncomingSMS* sms = new IncomingSMS();
			sms->setup(bullet, 1, noColAng);
			ofAddListener(sms->onBallGetSMSrepEvent, this, &BlobManager::onBallGetSMSrep);			
			inSMSs.push_back(sms);			
		}
	}	
	
}

void BlobManager::onClearBG(int& i) {
	bg.clear();
}



