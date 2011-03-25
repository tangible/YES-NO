#include "ofxCamera.h"

ofxCamera::ofxCamera(){
	perspective();
	position();
	eye();
	up();
}

void ofxCamera::position(float x, float y, float z){
	posCoord.x = x;
	posCoord.y = y;
	posCoord.z = z;
}
void ofxCamera::position(ofxVec3f _pos){
	posCoord = _pos;
}
void ofxCamera::position(){
	posCoord.x = (float)w/2.0f;
	posCoord.y = (float)h/2.0f;
	float halfFov = PI * fieldOfView / 360.0f;
	float theTan = tanf(halfFov);
	posCoord.z = posCoord.y/theTan;
}

void ofxCamera::lerpPosition(float _targetX, float _targetY, float _targetZ, float _step){
	posCoord.x += (_targetX - posCoord.x) * _step;
	posCoord.y += (_targetY - posCoord.y) * _step;
	posCoord.z += (_targetZ - posCoord.z) * _step;
}

void ofxCamera::lerpPosition(ofxVec3f target, float step){
	lerpPosition(target.x, target.y, target.z, step);
}

void ofxCamera::lerpEye(float _targetX, float _targetY, float _targetZ, float _step){
	eyeCoord.x += (_targetX - eyeCoord.x) * _step;
	eyeCoord.y += (_targetY - eyeCoord.y) * _step;
	eyeCoord.z += (_targetZ - eyeCoord.z) * _step;
}

void ofxCamera::lerpEye(ofxVec3f target, float step){
	lerpEye(target.x, target.y, target.z, step);
}

void ofxCamera::slerpEye(ofxVec3f target, float time)
{
    ofxQuaternion toRot, fromRot;

    ofxVec3f view = -posCoord + eyeCoord;
    ofxVec3f final_view = -posCoord + target;

    toRot.makeRotate(view,final_view);
    fromRot.makeRotate(0,view.x,view.y,view.z);

    fromRot.slerp(time,fromRot,toRot);

    eyeCoord = posCoord + fromRot*view;

}

void ofxCamera::eye(float x, float y, float z){
	eyeCoord.x = x;
	eyeCoord.y = y;
	eyeCoord.z = z;
}

void ofxCamera::eye(ofxVec3f _pos){
	eyeCoord = _pos;
}

void ofxCamera::eye(){
	eyeCoord.x = (float)w/2.0f;
	eyeCoord.y = (float)h/2.0f;
	eyeCoord.z = 0;
}


void ofxCamera::up(float _nx, float _ny, float _nz){
	upVec.x = _nx;
	upVec.y = _ny;
	upVec.z = _nz;
}

void ofxCamera::up(ofxVec3f _up){
	upVec = _up;
}


void ofxCamera::up(){
	upVec.x = 0;
	upVec.y = 1;
	upVec.z = 0;
}

void ofxCamera::perspective(float _fov, float _aspect, float _zNear, float _zFar){
	fieldOfView = _fov;
	aspectRatio = _aspect;
	if(_zNear==0) _zNear = 0.01;
	zNear = _zNear;
	zFar = _zFar;
}

void ofxCamera::perspective(){
	fieldOfView = 65.0f;
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	aspectRatio = (float)w/(float)h;
	zNear = 100.0f;
	zFar = 10000.0f;
}

void ofxCamera::place(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspectRatio, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posCoord[0], posCoord[1], posCoord[2], eyeCoord[0], eyeCoord[1], eyeCoord[2], upVec[0], upVec[1], upVec[2]);

	glScalef(1, -1, 1);
	glTranslatef(0, -h, 0);
}


//Removes the camera, so it returns as if there was no camera
void ofxCamera::remove(){
	ofSetupScreen();
}



void ofxCamera::moveLocal(float _x, float _y, float _z){
	moveLocal(ofxVec3f(_x, _y, _z));
}

void ofxCamera::moveLocal(ofxVec3f move){
	ofxVec3f dir =  getDir().normalized();
	posCoord += dir.rescaled(move.z);
	eyeCoord += dir.rescaled(move.z);

	posCoord += upVec.rescaled(move.y);
	eyeCoord += upVec.rescaled(move.y);

	posCoord += dir.cross(upVec).rescaled(move.x);
	eyeCoord += dir.cross(upVec).rescaled(move.x);
}

void ofxCamera::moveGlobal(float _x, float _y, float _z){
	posCoord.x += _x;
	posCoord.y += _y;
	posCoord.z += _z;
	eyeCoord.x += _x;
	eyeCoord.y += _y;
	eyeCoord.z += _z;
}
void ofxCamera::moveGlobal(ofxVec3f move){
	posCoord += move;
	eyeCoord += move;
}

void ofxCamera::orbitAround(ofxVec3f target, ofxVec3f axis, float value){
	ofxVec3f r = posCoord-target;
	posCoord = target + r.rotated(value, axis);
}

void ofxCamera::rotate(ofxVec3f axis, float value){
	ofxVec3f r = -posCoord+eyeCoord;
	eyeCoord = posCoord + r.rotated(value, axis);
}

void ofxCamera::qorbitAround(ofxVec3f target, ofxVec3f axis, float value)
{
    ofxQuaternion rot, view, result;

    //positive quaternion rotation is counter-clockwise so multiply by -1 to get clockwise
    float angle = (float)(-value*DEG_TO_RAD);

    ofxVec3f r = posCoord-target;
    view.set(r.x,r.y,r.z,0);

    rot.makeRotate(angle, axis.x, axis.y, axis.x);

    result = (rot*view)*rot.conj();

    posCoord = target + result.asVec3();
}

void ofxCamera::qrotate(ofxVec3f axis, float value)
{
    ofxQuaternion rot, view, result;

    //positive quaternion rotation is counter-clockwise so multiply by -1 to get clockwise
    float angle = (float)(-value*DEG_TO_RAD);

    ofxVec3f r = -posCoord+eyeCoord;
    view.set(r.x,r.y,r.z,0);

    rot.makeRotate(angle, axis.x, axis.y, axis.x);

    result = (rot*view)*rot.conj();

    eyeCoord = posCoord + result.asVec3();
}

void ofxCamera::setViewByMouse(int x, int y)
{
    ofxVec2f mouseDelta;
    float MouseSensitivity = 10.0f;
    float MiddleX = ofGetWidth()/2;
    float MiddleY = ofGetHeight()/2;

    if((x == MiddleX) && (y == MiddleY))
        return;

    // otherwise move the mouse back to the middle of the screen
    glutWarpPointer(MiddleX, MiddleY);

    mouseDelta.x = (MiddleX - x)/MouseSensitivity;
    mouseDelta.y = (MiddleY - y)/MouseSensitivity;

    // get the axis to rotate around the x-axis.
    ofxVec3f axis = eyeCoord - posCoord;
    axis.cross(upVec);
    // To be able to use the quaternion conjugate, the axis to
    // rotate around must be normalized.
    axis.normalize();

    // Rotate around the x axis
    qrotate(axis,mouseDelta.y);
    // Rotate around the y axis
    qrotate(ofxVec3f(0, 1, 0),mouseDelta.x);
}

//
//Getters
//
ofxVec3f ofxCamera::getDir(){
	return eyeCoord-posCoord;
}

ofxVec3f ofxCamera::getPosition(){
	return posCoord;
}

ofxVec3f ofxCamera::getEye(){
	return eyeCoord;
}

ofxVec3f ofxCamera::getUp(){
	return upVec;
}


