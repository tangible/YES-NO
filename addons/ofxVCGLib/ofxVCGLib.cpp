/*
 *  ofxVCGLib.cpp
 *
 *  Created by Makira on 11/03/03.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "ofxVCGLib.h"

void ofxVCGLib::reconstructFaceBunny() {

	float radius = 0.22f;
	float clustering = 0.05;
	
	for (int i = 0; i < BUNNY_NUM_VERTICES; i++) {
		float x = gVerticesBunny[i*3];
		float y = gVerticesBunny[i*3+1];
		float z = gVerticesBunny[i*3+2];
		CVertex mv;
		mv.P() = Point3f(x,y,z);
		m.vert.push_back(mv);
		m.vn++;
//		cout << ofToString(x)+" "+ofToString(y)+" "+ofToString(z) << endl;
	}
//	cout << "total vertices = " + ofToString(m.vn) << endl;
	
	vcg::tri::UpdateBounding<CMesh>::Box(m);
	vcg::tri::UpdateNormals<CMesh>::PerFace(m);	
	tri::BallPivoting<CMesh> pivot(m, radius, clustering); 	
	pivot.BuildMesh();

}

void ofxVCGLib::reconstructFacePointCloud(vector<float> point_cloud, 
										  float _radius, 
										  float _clustering,
										  float _angle) {
	m.face.clear();
	m.fn = 0;	
	m.Clear();
	radius = _radius;
	mindist = _clustering;
	crease = _angle;
	
	for (int i = 0; i < point_cloud.size()/3; i++) {
		float x = point_cloud[i*3];
		float y = point_cloud[i*3+1];
		float z = point_cloud[i*3+2];
		CVertex mv;
		mv.P() = Point3f(x,y,z);
		m.vert.push_back(mv);
		m.vn++;
//		cout << ofToString(x)+" "+ofToString(y)+" "+ofToString(z) << endl;
	}
//	cout << "point cloud size = " + ofToString((int)point_cloud.size()) << endl;
//	cout << "total vertices = " + ofToString(m.vn) << endl;
	
	vcg::tri::UpdateBounding<CMesh>::Box(m);
	vcg::tri::UpdateNormals<CMesh>::PerFace(m);	
	tri::BallPivoting<CMesh> pivot(m, _radius, _clustering); 	
	pivot.BuildMesh();
	
	//NormalExtrapolation<vector<CVertex> >::ExtrapolateNormals(m.vert.begin(), m.vert.end(), 10);	
	
	box = Box3f();  
	for(int i = 0; i < m.vert.size(); i++)
		box.Add(m.vert[i].P());
	
	box.Offset(4*radius);
	grid.Set(m.vert.begin(), m.vert.end(), box);
	nb.clear();
	nb.resize(m.vert.size(), 0);
	for(int i = 0; i < m.vert.size(); i++) 
		m.vert[i].ClearFlags();
	
//	for(int i = 0; i < m.vert.size();i ++) {
//		seed(true, i);
//	}
}

/* expand the front adding 1 face. Return false on failure (id when
 all Hinges are dead  returns:
 1: added a face
 0: added nothing
 -1: finished */
int ofxVCGLib::addFace() {
	//We try to seed again
	if(!m.face.size()) {
        for(int i = 0; i < 100; i++) 
			if(seed()) return 1;
        return -1;
	}
	
	if(!front.size()) {
        //maybe there are unconnected parts of the mesh:
        //find a non D, V, B point and try to seed if failed D it.
        for(int i = 0; i < m.vert.size();i ++) {
			CVertex &v = m.vert[i];
			if(v.IsD() || v.IsV() || v.IsB()) continue;
			if(seed(true, i)) return 1;
			v.SetD();
        }
        return -1;
	}
    
	list<Hinge>::iterator ei = front.begin();
	Hinge &e = *ei;
	Hinge &previous = *e.previous;           
	Hinge &next = *e.next;  
	int v0 = e.v0, v1 = e.v1;
    
	//last triangle missing. or it is the first?
	if(0 &&next.next == e.previous) {  
		
        int v[3] = { previous.v0, next.v0, e.v0 };
        int c[3] = { 0, 0, 0 };
		
        for(int k = 0; k < 3; k++) {
			int vert = v[k];
			nb[vert]--;
			if(nb[vert] == 0) {       
				m.vert[vert].SetV();
				m.vert[vert].ClearB();
			}              
        }        
        assert(previous.previous == e.next); 
        addFace(previous.v0, next.v0, e.v0);                       
		
        front.erase(e.previous);
        front.erase(e.next);        
        front.erase(ei);    
        
        return 1;
	}
    
	int v2;
	Point3f center;
    
	vector<int> targets;
	bool success = pivot(e, v2, center, targets);
    
	//if no pivoting move this thing to the end and try again
	//or we are trying to connect to the inside of the m. BAD.
	if(!success || m.vert[v2].IsV()) {
        killHinge(ei);
        return 0;
	} 
    
	//does v2 belongs to a front? (and which?)
	list<Hinge>::iterator touch = touches(v2, ei);
    
	assert(v2 != v0 && v2 != v1);  
    
	int fn = m.face.size();
	if(touch != front.end()) {       
		
        if(!checkHinge(v0, v2) || !checkHinge(v2, v1)) {                      
			killHinge(ei);
			return 0;
        }
        
        if(v2 == previous.v0) {    
			/*touching previous Hinge  (we reuse previous)        
			 next
             ------->v2 -----> v1------>
			 \       /
			 \     /
			 previous \   / e
			 \ /
			 v0           */
			
			detach(v0);
			
			previous.v1 = v1;       
			previous.v2 = v0;
			previous.face = fn;
			previous.center = center;
			
			previous.next = e.next;
			next.previous = e.previous;
			moveBack(e.previous);            
			
			//this checks if we can glue something to e.previous
			trovamiunnome(e.previous);      
			front.erase(ei);    
			
			
        } else if(v2 == next.v1) {
			/*touching previous Hinge  (we reuse next)        
			 previous
             ------->v0 -----> v2------>
			 \       /
			 \     /
			 \   / next
			 \ /
			 v1           */      
			
			detach(v1);
			
			next.v0 = v0;
			next.v2 = v1;
			next.face = fn;
			next.center = center;
			next.previous = e.previous;
			previous.next = e.next;
			//      moveBack(e.next);
			
			//this checks if we can glue something to e.previous
			trovamiunnome(e.next);         
			front.erase(ei);
			
        } else {
			
			/*   this code would delay the joining Hinge to avoid bad situations not used but..
			 if(e.count < 2) {
			 e.count++;
			 moveBack(ei);
			 return true;         
			 }*/
			
			//touching some loop: split (or merge it is local does not matter.
			//like this 
			/*                 
			 left        right
			 <--------v2-<------
			 /|\
			 /   \
			 up /     \ down
			 /       \
			 /         V
			 ----v0 - - - > v1---------
			 e                         */           
			list<Hinge>::iterator left = touch;
			list<Hinge>::iterator right = (*touch).previous;      
			list<Hinge>::iterator up = ei;
			
			if(v1 == (*right).v0 || v0 == (*left).v1) {
				//            cout << "Bad join.\n";
				killHinge(ei);
				return 0;
			}
			
			nb[v2]++;    
			
			list<Hinge>::iterator down = newHinge(Hinge(v2, v1, v0, fn, center));      
			
			(*right).next = down;
			(*down).previous = right;
			
			(*down).next = e.next;
			next.previous = down;      
			
			(*left).previous = up;
			(*up).next = left;
			
			(*up).v2 = v1;      
			(*up).v1 = v2;
			(*up).face = fn;
			(*up).center = center;
			moveBack(ei);
        }                         
		
		
		
	} else {
        assert(!m.vert[v2].IsB()); //fatal error! a new point is already a border?
		
        /*  adding a new vertex
		 
		 v2
		 /|\
		 /   \
		 up /     \ down
		 /       \
		 /         V
		 ----v0 - - - > v1--------- */
        cluster(v2);        
        nb[v2]++;                 
        m.vert[v2].SetB();
        list<Hinge>::iterator down = newHinge(Hinge(v2, v1, v0, fn, center));
        (*down).previous = ei;
        (*down).next = e.next;
        next.previous = down;
        
        e.v2 = v1;    
        e.v1 = v2;
        e.face = fn;
        e.center = center;
        e.next = down; 
        moveBack(ei);
	}
	addFace(v0, v2, v1);
	return 1;
}        

/* add a new face. compute normals. */
void ofxVCGLib::addFace(int a, int b, int c) {
	CFace face;
	face.V(0) = (CVertex *)a;
	face.V(1) = (CVertex *)b;
	face.V(2) = (CVertex *)c;
	
	Point3f &p0 = m.vert[a].P();
	Point3f &p1 = m.vert[b].P();
	Point3f &p2 = m.vert[c].P();
	face.N() = ((p1 - p0)^(p2 - p0)).Normalize();
	
	m.face.push_back(face);
	m.fn++;
}   

/* select a vertex at random, a small group of nearby vertices and looks
 for a sphere that touches 3 and contains none.
 Use the center of the box to get a sphere inside (or outside) the model 
 You may be unlucky... */
bool ofxVCGLib::seed(bool outside, int start) {         
	//pick a random point (well...)
	if(start == -1) start = rand()%m.vert.size();
	
	//get a sphere of neighbours
	vector<int> targets;
	vector<float> dists;
	int n = getInSphere(m.vert[start].P(), 2*radius, targets, dists);
	if(n < 3) { 
        //bad luck. we should call seed again (assuming random pick) up to
        //some maximum tries. im lazy.
        return false;
	}
	int v0, v1, v2;
	bool found = false;
	//find a triplet that does not contains any other point
	Point3f center;
	for(int i = 0; i < n; i++) {
        v0 = targets[i];
        CVertex &vv0 = m.vert[v0];
        if(vv0.IsD() || vv0.IsB() || vv0.IsV()) continue;
        Point3f &p0 = m.vert[v0].P();
        Point3f out = (p0 - box.Center());
        if(!outside) out = -out;
		
        for(int k = i+1; k < n; k++) {
			v1 = targets[k];            
			CVertex &vv1 = m.vert[v1];
			if(vv1.IsD() || vv1.IsB() || vv1.IsV()) continue;
			Point3f &p1 = m.vert[v1].P();            
			if((p1 - p0).Norm() < mindist*radius) continue;
			
			for(int j = k+1; j < n; j++) {
				v2 = targets[j];
				CVertex &vv2 = m.vert[v2];
				if(vv2.IsD() || vv2.IsB() || vv2.IsV()) continue;
				Point3f &p2 = m.vert[v2].P();
				if((p2 - p0).Norm() < mindist*radius) continue;
				if((p2 - p1).Norm() < mindist*radius) continue;
				Point3f normal = (p1 - p0)^(p2 - p0);
				//check normal pointing inside
				if(normal * out < 0) continue;
				if(!findSphere(p0, p1, p2, center)) continue;
				
				bool failed = false;
				//check no other point inside
				for(int t = 0; t < n; t++) {
					Point3f &p = m.vert[targets[t]].P();
					if((center - p).Norm() <= radius) {
						failed = true;
						break;
					}
				}
				if(failed) continue;  
				found = true;
				i = k = j = n;
			}
        }
	}
	
	if(!found)  //see bad luck above
        return false;
	
	assert(!front.size());
	//TODO: should i check the Hinges too?
	addFace(v0, v1, v2);
	
	//create the border of the first face  
	list<Hinge>::iterator e = front.end();
	list<Hinge>::iterator last;
	for(int i = 0; i < 3; i++) {
        int v0 = (int)(m.face.back().V0(i));
        int v1 = (int)(m.face.back().V1(i));    
        int v2 = (int)(m.face.back().V2(i));    
        nb[v0] = 1;
        assert(!m.vert[v0].IsB());
        m.vert[v0].SetB();
        Hinge Hinge(v0, v1, v2, 0, center);
        Hinge.previous = e;
        e = front.insert(front.begin(), Hinge);
        if(i == 0) last = e;
        (*Hinge.previous).next = e;
        
        cluster(v0);
	} 
    
	//connect last and first
	(*e).next = last;
	(*last).previous = e;
	return true;
}

/* return all point in a given ball, notice as i want the index
 of the vertices not the pointers... this may change in future */
unsigned int ofxVCGLib::getInSphere(vcg::Point3f &p, 
									float distance, 
									vector<int> &results,
									vector<float> &dists) {
	vector<CVertex *> ptr;
	vector<Point3f> points;
	int n = vcg::tri::GetInSphereVertex(m, grid, p, distance, ptr, dists, points);
	for(int i = 0; i < ptr.size(); i++) 
        results.push_back(ptr[i] - &(m.vert[0]));
	return n;
}   

/* returns the sphere touching p0, p1, p2 of radius r such that
 the normal of the face points toward the center of the sphere */
bool ofxVCGLib::findSphere(Point3f &p0, Point3f &p1, Point3f &p2, Point3f &center) {
	Point3f q1 = p1 - p0;
	Point3f q2 = p2 - p0;  
    
	Point3f up = q1^q2;
	float uplen = up.Norm();
    
	//the three points are aligned
	if(uplen < 0.001*q1.Norm()*q2.Norm()) return false;
	up /= uplen;
	
    
	float a11 = q1*q1;
	float a12 = q1*q2;
	float a22 = q2*q2;
    
	float m = 4*(a11*a22 - a12*a12);
	float l1 = 2*(a11*a22 - a22*a12)/m;
	float l2 = 2*(a11*a22 - a12*a11)/m;
    
	center = q1*l1 + q2*l2;
	float circle_r = center.Norm();
	if(circle_r > radius) return false; //need too big a sphere
    
	float height = sqrt(radius*radius - circle_r*circle_r);
	center += p0 + up*height;
    
	return true;
}    

void ofxVCGLib::cluster(int v) {
	/* clean up too close points */
	vector<int> targets;
	vector<float> dists;    
	getInSphere(m.vert[v].P(), mindist*radius, targets, dists);
	
	for(int i = 0; i < targets.size(); i++) {
		int id = targets[i];
		if(id == v) continue;
		
		CVertex &v = m.vert[id];
		if(v.IsD() || v.IsV() || v.IsB()) continue;
		v.SetD();
	}
	
}

/* return new vertex and the center of the new sphere pivoting from Hinge
 if the vertex belongs to another Hinge, touch points to it. */
bool ofxVCGLib::pivot(Hinge &Hinge, int &candidate, Point3f &end_pivot, vector<int> &targets) {
	Point3f v0 = m.vert[Hinge.v0].P();
	Point3f v1 = m.vert[Hinge.v1].P();  
	Point3f v2 = m.vert[Hinge.v2].P();  
	/* TODO why using the face normals everything goes wrong? should be
	 exactly the same................................................
	 Check if the e.face is correct.
	 Point3f &normal = m.face[Hinge.face].N();
	 */
    
	Point3f normal = ((v1 - v0)^(v2 - v0)).Normalize();
	
	Point3f middle = (v0 + v1)/2;    
	Point3f start_pivot = Hinge.center - middle;          
	Point3f axis = (v1 - v0);
	
	float axis_len = axis.SquaredNorm();
	if(axis_len > 4*radius*radius) return false;
	axis.Normalize();
	
	// r is the radius of the thorus of all possible spheres passing throug v0 and v1
	float r = sqrt(radius*radius - axis_len/4);
	
    
	vector<float> dists;    
	getInSphere(middle, r + radius, targets, dists);
    
	if(targets.size() == 0) return false; //this really would be strange but one never knows.
    
	candidate = -1;
	float minangle = 0;
	Point3f center;  //to be computed for each sample
	for(int i = 0; i < targets.size(); i++) {      
		int id = targets[i];
		
		if(id == Hinge.v0 || id == Hinge.v1 || id == Hinge.v2) continue;
		
		if(m.vert[id].IsD()) {
            continue;      
		}
		
		
		Point3f p = m.vert[id].P();
		
		/* Prevent 360 Hinges, also often reject ~ 50% points */      
		Point3f n = ((p - v0)^(v1 - v0)).Normalize();
		if(n * normal < -0.5) {
            continue;               
		}
		
		
		/* Find the sphere through v0, p, v1 (store center on end_pivot */
		if(!findSphere(v0, p, v1, center)) {
            continue;      
		}
		
		/* Angle between old center and new center */
		float alpha = angle(start_pivot, center - middle, axis);
		
		/* adding a small bias to already chosen vertices.
		 doesn't solve numerical problems, but helps. */
		if(m.vert[id].IsB()) alpha -= 0.001;
		
		/* Sometimes alpha might be little less then M_PI while it should be 0,
		 by numerical errors: happens for example pivoting 
		 on the diagonal of a square. */
		
		if(alpha > 2*M_PI - 0.8) {               
            // Angle between old center and new *point* 
            //TODO is this really overshooting? shouldbe enough to alpha -= 2*M_PI
            Point3f proj = p - axis * (axis * p - axis * middle);
            float beta = angle(start_pivot, proj - middle, axis);
			
            if(alpha > beta) alpha -= 2*M_PI; 
		}
		if(candidate == -1 || alpha < minangle) {        
            candidate = id; 
            minangle = alpha;
            end_pivot = center;
		}
	}
	//found no point suitable.
	if(candidate == -1) {
		return false;
	}
	
	assert(candidate != Hinge.v0 && candidate != Hinge.v1);
	return true;
}  

/* compute angle from p to q, using axis for orientation */
float ofxVCGLib::angle(Point3f p, Point3f q, Point3f &axis) {
	p.Normalize();
	q.Normalize();
	Point3f vec = p^q;
	float angle = acos(p*q);
	if(vec*axis < 0) angle = -angle;
	if(angle < 0) angle += 2*M_PI;
	return angle;
}  

//move an Hinge among the dead ones
void ofxVCGLib::killHinge(list<Hinge>::iterator e) {
	deads.splice(deads.end(), front, e);
}

list<Hinge>::iterator ofxVCGLib::touches(int v, list<Hinge>::iterator e) {
	//TODO what happens when it touches more than one front?
	//might still work.
    
	list<Hinge>::iterator touch = front.end();
	if(m.vert[v].IsB()) {
        //test nearby Hinges: it is faster
        list<Hinge>::iterator p = e;
        p = (*e).previous;
        if(v == (*p).v0) return p;
        e = (*e).next;
        if(v == (*e).v0) return e;
		
        p = (*p).previous;
        if(v == (*p).v0) return p;
        e = (*e).next;
        if(v == (*e).v0) return e;
		
        //test all. sigh.
		
        for(list<Hinge>::iterator k = front.begin(); k != front.end(); k++) {
			if(v == (*k).v0) { 
				touch = k;
				break;
			}
        }
        for(list<Hinge>::iterator k = deads.begin(); k != deads.end(); k++) {
			if(v == (*k).v0) { 
				touch = k;
				break;
			}
        }
        assert(touch != front.end());
	}
    
	return touch;
}     

bool ofxVCGLib::checkHinge(int v0, int v1) {
	int tot = 0;
	//HACK to speed up things until i can use a seach structure
	int i = m.face.size() - 2*(front.size());
    //  i = 0;
	if(i < 0) i = 0;
	for(; i < m.face.size(); i++) { 
        CFace &f = m.face[i];
        for(int k = 0; k < 3; k++) {
			if(v1== (int)f.V(k) && v0 == (int)f.V((k+1)%3)) ++tot;
			else if(v0 == (int)f.V(k) && v1 == (int)f.V((k+1)%3)) { //orientation non constistent
				return false;
			}
        }
        if(tot >= 2) { //non manifold
			return false;
        }
	}
	return true;
}    

void ofxVCGLib::detach(int v) {
	assert(nb[v] > 0);
	if(--nb[v] == 0) {
        m.vert[v].SetV();
        m.vert[v].ClearB();      
	}
}

//move an Hinge to the back of the queue
void ofxVCGLib::moveBack(list<Hinge>::iterator e) {
	front.splice(front.end(), front, e);          
}

void ofxVCGLib::trovamiunnome(list<Hinge>::iterator e) {
	if(glue((*e).previous, e)) return;
	glue(e, (*e).next);
}

//glue toghether a and b (where a.next = b
bool ofxVCGLib::glue(list<Hinge>::iterator a, list<Hinge>::iterator b) {
	if((*a).v0 != (*b).v1) return false; 
	
	list<Hinge>::iterator previous = (*a).previous;
	list<Hinge>::iterator next = (*b).next;
	(*previous).next = next;
	(*next).previous = previous;
	detach((*a).v1);
	detach((*a).v0); 
	front.erase(a);
	front.erase(b);  
	return true;
}

//Add a new Hinge to the back of the queue
list<Hinge>::iterator ofxVCGLib::newHinge(Hinge e) {                  
	return front.insert(front.end(), e);
}  

vector<float> ofxVCGLib::getVertices() {
	
	vector<float> verts;
	for (int i = 0; i < m.vert.size(); i++) {
		verts.push_back(m.vert[i].P()[0]);
		verts.push_back(m.vert[i].P()[1]);
		verts.push_back(m.vert[i].P()[2]);
	}
	return verts;
	
}

vector<int> ofxVCGLib::getFaceIndices() {
	
	vector<int> faceIndices;
	for (int i = 0; i < m.face.size(); i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < m.vert.size(); k++) {
				float xc = m.face[i].V(j)->P()[0];
				float yc = m.face[i].V(j)->P()[1];
				float zc = m.face[i].V(j)->P()[2];				
				ofxVec3f c(xc, yc, zc);
				float xt = m.vert[k].P()[0];
				float yt = m.vert[k].P()[1];
				float zt = m.vert[k].P()[2];
				ofxVec3f t(xt, yt, zt);
				if (c == t) {
					faceIndices.push_back(k);
					break;
				}
			}
		}
	}
	return faceIndices;
	
}

int ofxVCGLib::getFaceNum() {
	
	return m.face.size();
	
}