/*
 *  BlobManager.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/01/31.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "BlobManager.h"

#define FPS 40

void BlobManager::setup() {

	counter = 0;
	counter2 = 0;
	motionBlur = 0;
	
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
	
    materialAmbient  = new float[4];
    materialDiffuse  = new float[4];
    materialSpecular = new float[4];
    for (int i=0; i<4; i++){
        materialAmbient[i]  = 1.0f;
        materialDiffuse[i]  = 20.0f;
        materialSpecular[i] = 1.0f;
    }
	
    blurValue  = 1000;
    nMetaBalls = 20;
	nPoints  = nMetaBalls;
	
	ballPoints     = new ofPoint[nPoints];
	ballPointsPrev = new ofPoint[nPoints];
	ballPointsPrev2= new ofPoint[nPoints];
	ballMasses     = new float[nPoints];
	ballLevels     = new int[nPoints];
	for (int i=0; i<nPoints; i++){
	    ballPoints[i].set(0.0,0.0,0.0);
	    ballPointsPrev[i].set(0.0,0.0,0.0);
	    ballPointsPrev2[i].set(0.0,0.0,0.0);
	    ballMasses[i] = 1.0;
	    ballLevels[i] = 0;
	}
	// levels = 0,1,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3
	
	//-----------------------
	m_pMetaballs = new CMetaballs (nPoints);
	m_pMetaballs->SetGridSize(100);
	CMarchingCubes::BuildTables();
	
    maxDeceleration = 0;
    decelerationThreshPct = 0.50;
    decelerationEventCount = 0;
	
	boundsMin.x = boundsMin.y = boundsMin.z =  9999999;
    boundsMax.x = boundsMax.y = boundsMax.z = -9999999;
    boundsAvg.x = boundsAvg.y = boundsAvg.z = 0;
	
    boundsScaling = 1.0 / 1020.0f;
	
	shader.setup("shaders/glsl");
	ofDisableArbTex();
	//	player.loadMovie("movies/foces60fps.mov");
	//	player.play();
	img.loadImage("imgs/b.jpeg");
	img.resize(ofGetWidth(), ofGetHeight());
	//	tex.allocate(img.getWidth(), img.getHeight(), GL_RGB);
	//	tex.loadData(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGB);
	glActiveTexture(GL_TEXTURE1);
	texSlot = 1;
	//	glBindTexture(GL_TEXTURE_2D, player.getTextureReference().getTextureData().textureID);	
	glBindTexture(GL_TEXTURE_2D, img.getTextureReference().getTextureData().textureID);	
	//	glBindTexture(GL_TEXTURE_2D, tex.getTextureData().textureID);		
	glActiveTexture(GL_TEXTURE0);		
	
	bullet = new ofxBullet();
	bullet->initPhysics(ofxVec3f(0, 0, 0), false);
	
	for (int i = 0; i < nMetaBalls; i++) {
		
		ofxVec3f rdmPos = ofxVec3f(ofGetWidth()/2+ofRandom(-100, 100), ofGetHeight()/2+ofRandom(-100, 100), ofRandom(-100, 100));
		
		MyRigidBody* sph = bullet->createSphere(rdmPos,
												ofRandom(50, 50), 
												1, 
												ofxVec4f(ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), 0.7), 
												DYNAMIC_BODY);		
		spheres.push_back(sph);
	}	
	
}

void BlobManager::update() {

	counter2 = (counter2+1)%blurValue;
	counter  = counter2 + 1;
	
	bullet->stepPhysicsSimulation(FPS);	
	float maxVal = 0.01;
	ofxVec3f force;
	ofxVec3f crossVec;
	crossVec.set(0, 0, 0);
	crossVec.rotate(ofGetFrameNum(), ofGetFrameNum(), ofGetFrameNum());
	ofxVec3f tangentVec;	
	int rdmIdx = (int)ofRandom(0, 21);
	for (int i = 0; i < spheres.size(); i++) {
		force.set(-spheres[i]->getBodyPos()+ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));
		force *= maxVal * 15;
		tangentVec = force.crossed(crossVec);
		tangentVec.normalize();
		tangentVec *= maxVal*100;
		force += tangentVec;
		btVector3 btImpulse(force.x, force.y, force.z);
		spheres[i]->getRigidBody()->applyCentralImpulse(btImpulse);
		
		ofxVec3f impulse;
		//impulse.set(ofRandomuf(), ofRandomuf(), ofRandomuf());
		impulse.set(ofRandomf(), ofRandomf(), ofRandomf());
		//impulse.rotate(ofRandomf()*360, ofRandomf()*360, ofRandomf()*360);
		//impulse *= ofRandom(-4.0, 4.0);
		if (i == rdmIdx) {
			impulse *= ofRandom(-250.0, 250.0);
		}else {
			impulse *= 0;
		}
		btImpulse = btVector3(impulse.x, impulse.y, impulse.z);
		spheres[i]->getRigidBody()->applyCentralImpulse(btImpulse);
	}		
	
}

void BlobManager::draw() {


//    glDisable(GL_LIGHTING);
//	glColor3f(1.0, 1.0, 1.0);
//	img.draw(0, 0);
	
	
    //---------------------
    // update metaball point locations
    float bx,by,bz;
    float px,py,pz;
    bx = by = bz = 0.5;
	
    float frac = (float)counter/(float)blurValue;
    frac = 0.50 + 0.50  * cos(TWO_PI * frac);
    frac = 0.05 + 0.95 * frac;
    frac = pow(frac, 4.0f);
    float A = frac;
    float B = 1.0-A;
    motionBlur = A;
	
    float massBase = 0;
    for (int i=0; i<nMetaBalls; i++){
        // compute scaled coordinates
        if ((counter > 1)){
			ofxVec3f pos = spheres[i]->getBodyPos()-ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, -400);
			bx = (pos.x - boundsAvg.x) * boundsScaling;
			by = (pos.y - boundsAvg.y) * boundsScaling;
			bz = (pos.z - boundsAvg.z) * boundsScaling;
        }
        // stash the previous coordinates
        px = ballPoints[i].x;
        py = ballPoints[i].y;
        pz = ballPoints[i].z;
		
        // compute blurred new coordinates
        bx = A*px + B*bx;
        by = A*py + B*by;
        bz = A*pz + B*bz;
        ballPoints[i].set(bx,by,bz);
		
        // compute masses
        massBase           = 0.147; //0.135;
        float massBaseSin  = 0.035 * sin(ofGetElapsedTimeMillis()/4000.0);
        float massLevelSin = 0.010 * sin(ofGetElapsedTimeMillis()/1300.0 - ballLevels[i]);
        ballMasses[i] = massBase + massBaseSin + massLevelSin;
    }

    m_pMetaballs->UpdateBallsFromPointsAndMasses (nPoints, ballPoints, ballMasses);
	
	setupForNoTexturing();
	//	setupForTexturing();
	
    // Actually draw them
	//	shader.begin();
	//	shader.setUniform1i("tex", texSlot);
	//ofSetColor(170, 170, 170);
    glPushMatrix();
	float w = ofGetWidth();
	float h = ofGetHeight();
	float sz = 0.75*min(w,h);
	glTranslatef(w/2,h/2,0);
	glScalef(sz,sz,sz);
	
	m_pMetaballs->Render();
    glPopMatrix();
	//ofSetColor(255, 255, 255);
	//	shader.end();
	
	
    //---------------------
    bool bDrawDropShadow = true;
    if (bDrawDropShadow){
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glColor4f(0,0.002,0.005, 0.17);
        glEnable(GL_BLEND);
        glDisable(GL_LIGHTING);
        glDisable(GL_POLYGON_SMOOTH);
        glDisable(GL_CULL_FACE);
        glDisable(GL_COLOR_MATERIAL);
        glReadPixels(0,0,screenW,screenH, GL_DEPTH_COMPONENT, GL_FLOAT, screenDepth);
        huntForBlendFunc(1000,1,5);//0,2);
		
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
            shadowPixelsLA[i*2  ] = blurred[i];//(i/shadowW)%255;
            shadowPixelsLA[i*2+1] = blurred[i];
        }
        shadowTex.loadData(shadowPixelsLA, shadowW,shadowH, GL_LUMINANCE_ALPHA);
        glPushMatrix();
		glTranslatef(0,0,-200);//-200);
		glTranslatef(0.53*screenW, 0.73*screenH, 0);
		glScalef(12,12,1);
		glRotatef(60.0, 1,0,0);
		glTranslatef(-shadowW/2,shadowH/2,0);//-200);
		shadowTex.draw(0, 0, shadowW, 0-shadowH);// shadowW,0-shadowH);
		//shadowTex.draw(0, mouseY, shadowW*3,0-shadowH*3);
        glPopMatrix();
        glPopAttrib();
    }
	
	for (int i = 0; i < spheres.size(); i++) {
		//spheres[i]->render(bullet->getWorld());
	}
	
}

//======================================================================================
void BlobManager::huntForBlendFunc(int period, int defaultSid, int defaultDid){
	// sets all possible combinations of blend functions,
	// changing modes every [period] milliseconds.
	
	int sfact[] = {
		GL_ZERO,
		GL_ONE,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_SRC_ALPHA_SATURATE
	};
	
	int dfact[] = {
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA
	};
	
	glEnable(GL_BLEND);
	
	if ((defaultSid == -1) && (defaultDid == -1)) {
		
		int sid =  (ofGetElapsedTimeMillis()/(8*period))%9;
		int did =  (ofGetElapsedTimeMillis()/period)%8;
		glBlendFunc(sfact[sid], dfact[did]);
		printf("SRC %d	DST %d\n", sid, did);
		
	} else if (defaultDid == -1){
		
		int did =  (ofGetElapsedTimeMillis()/period)%8;
		glBlendFunc(sfact[defaultSid], dfact[did]);
		printf("SRC %d	DST %d\n", defaultSid, did);
		
	} else if (defaultSid == -1){
		
		int sid =  (ofGetElapsedTimeMillis()/(8*period))%9;
		glBlendFunc(sfact[sid], dfact[defaultDid]);
		printf("SRC %d	DST %d\n", sid, defaultDid);
		
	} else {
		
		glBlendFunc(sfact[defaultSid], dfact[defaultDid]);
		
	}
}



//--------------------------------------------------------------
void BlobManager::setupForTexturing(){
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glColor3f(0.9, 0.9, 0.9);
}

void BlobManager::setupForNoTexturing(){
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
	
    glColor3f(0.5, 0.5, 1.0);
    GLfloat on[]  = {1.0};
    GLfloat off[] = {0.0};
    glLightModelfv( GL_LIGHT_MODEL_TWO_SIDE,on);
	
	
    GLfloat shininess[] = {100};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,  materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,  materialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
	
	
    GLfloat lightPosition[] = { 0.0f, 0.0f, -100.0, 0.0f };
    GLfloat lightDiffuse[]  = { 1.00, 0.99, 0.98, 1.0};
    GLfloat lightSpecular[] = { 0.10, 0.10, 0.10, 1.0};
    GLfloat lightAmbient[]  = { 0.45, 0.43, 0.44, 1.0};
    glLightfv(GL_LIGHT0,GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0,GL_AMBIENT,  lightAmbient);
	
}