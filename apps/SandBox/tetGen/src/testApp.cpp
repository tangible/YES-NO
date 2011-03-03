#include "testApp.h"
#include <set>
//--------------------------------------------------------------
void testApp::setup(){
	
	cam.setup(this, 760);

//	for (int i = 0; i < 100; i++) {
//		ofxVec3f p = ofxVec3f(ofRandom(-50, 50),
//							  ofRandom(-50, 50),
//							  ofRandom(-50, 50));
//		points.push_back(p);
//	}
//				 
//	
//	tetgenio in;
//	in.numberofpoints = points.size();
//	in.pointlist = new REAL[3*points.size()];
//	for(unsigned int i = 0; i < in.numberofpoints; i++){
//		ofxVec3f p = points[i];
//		in.pointlist[3*i+0] = p.x;
//		in.pointlist[3*i+1] = p.y;
//		in.pointlist[3*i+2] = p.z;
//	}	
//	out = new tetgenio();
//	tetrahedralize((char*)"Qv", &in, out);
	
	tetgenio in;
	tetgenio::facet *f;
	tetgenio::polygon *p;
	int i;
	
	in.firstnumber = 1;
	
	in.numberofpoints = 8;
	in.pointlist = new REAL[in.numberofpoints * 3];
	in.pointlist[0]  = 0;  // node 1.
	in.pointlist[1]  = 0;
	in.pointlist[2]  = 0;
	in.pointlist[3]  = 2;  // node 2.
	in.pointlist[4]  = 0;
	in.pointlist[5]  = 0;
	in.pointlist[6]  = 2;  // node 3.
	in.pointlist[7]  = 2;
	in.pointlist[8]  = 0;
	in.pointlist[9]  = 0;  // node 4.
	in.pointlist[10] = 2;
	in.pointlist[11] = 0;
	// Set node 5, 6, 7, 8.
	for (i = 4; i < 8; i++) {
		in.pointlist[i * 3]     = in.pointlist[(i - 4) * 3];
		in.pointlist[i * 3 + 1] = in.pointlist[(i - 4) * 3 + 1];
		in.pointlist[i * 3 + 2] = 12;
	}
	
	in.numberoffacets = 6;
	in.facetlist = new tetgenio::facet[in.numberoffacets];
	in.facetmarkerlist = new int[in.numberoffacets];
	
	// Facet 1. The leftmost facet.
	f = &in.facetlist[0];
	f->numberofpolygons = 1;
	f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
	f->numberofholes = 0;
	f->holelist = NULL;
	p = &f->polygonlist[0];
	p->numberofvertices = 4;
	p->vertexlist = new int[p->numberofvertices];
	p->vertexlist[0] = 1;
	p->vertexlist[1] = 2;
	p->vertexlist[2] = 3;
	p->vertexlist[3] = 4;
	
	// Facet 2. The rightmost facet.
	f = &in.facetlist[1];
	f->numberofpolygons = 1;
	f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
	f->numberofholes = 0;
	f->holelist = NULL;
	p = &f->polygonlist[0];
	p->numberofvertices = 4;
	p->vertexlist = new int[p->numberofvertices];
	p->vertexlist[0] = 5;
	p->vertexlist[1] = 6;
	p->vertexlist[2] = 7;
	p->vertexlist[3] = 8;
	
	// Facet 3. The bottom facet.
	f = &in.facetlist[2];
	f->numberofpolygons = 1;
	f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
	f->numberofholes = 0;
	f->holelist = NULL;
	p = &f->polygonlist[0];
	p->numberofvertices = 4;
	p->vertexlist = new int[p->numberofvertices];
	p->vertexlist[0] = 1;
	p->vertexlist[1] = 5;
	p->vertexlist[2] = 6;
	p->vertexlist[3] = 2;
	
	// Facet 4. The back facet.
	f = &in.facetlist[3];
	f->numberofpolygons = 1;
	f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
	f->numberofholes = 0;
	f->holelist = NULL;
	p = &f->polygonlist[0];
	p->numberofvertices = 4;
	p->vertexlist = new int[p->numberofvertices];
	p->vertexlist[0] = 2;
	p->vertexlist[1] = 6;
	p->vertexlist[2] = 7;
	p->vertexlist[3] = 3;
	
	// Facet 5. The top facet.
	f = &in.facetlist[4];
	f->numberofpolygons = 1;
	f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
	f->numberofholes = 0;
	f->holelist = NULL;
	p = &f->polygonlist[0];
	p->numberofvertices = 4;
	p->vertexlist = new int[p->numberofvertices];
	p->vertexlist[0] = 3;
	p->vertexlist[1] = 7;
	p->vertexlist[2] = 8;
	p->vertexlist[3] = 4;
	
	// Facet 6. The front facet.
	f = &in.facetlist[5];
	f->numberofpolygons = 1;
	f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
	f->numberofholes = 0;
	f->holelist = NULL;
	p = &f->polygonlist[0];
	p->numberofvertices = 4;
	p->vertexlist = new int[p->numberofvertices];
	p->vertexlist[0] = 4;
	p->vertexlist[1] = 8;
	p->vertexlist[2] = 5;
	p->vertexlist[3] = 1;
	
	// Set 'in.facetmarkerlist'
	
	in.facetmarkerlist[0] = -1;
	in.facetmarkerlist[1] = -2;
	in.facetmarkerlist[2] = 0;
	in.facetmarkerlist[3] = 0;
	in.facetmarkerlist[4] = 0;
	in.facetmarkerlist[5] = 0;
	
	// Output the PLC to files 'barin.node' and 'barin.poly'.
	in.save_nodes((char*)"barin");
	in.save_poly((char*)"barin");
	
	// Tetrahedralize the PLC. Switches are chosen to read a PLC (p),
	//   do quality mesh generation (q) with a specified quality bound
	//   (1.414), and apply a maximum volume constraint (a0.1).
	
	out = new tetgenio();
	tetrahedralize((char*)"pq1.414a0.1Qv", &in, out);	
//	tetrahedralize((char*)"pq1.414a0.1", &in, out);
	
	int test = 0;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255, 255, 255);
	
	ofDrawBitmapString("number of points = " + ofToString(out->numberofpoints), 10, 20);	
	ofDrawBitmapString("number of point attributes = " + ofToString(out->numberofpointattributes), 10, 40);	
	ofDrawBitmapString("number of point metric = " + ofToString(out->numberofpointmtrs), 10, 60);	
	
	ofDrawBitmapString("number of tetrahedra = " + ofToString(out->numberoftetrahedra), 10, 90);	
	ofDrawBitmapString("number of tetrahedra corners = " + ofToString(out->numberofcorners), 10, 110);	
	ofDrawBitmapString("number of tetrahedra attributes = " + ofToString(out->numberoftetrahedronattributes), 10, 130);	
	
	ofDrawBitmapString("number of facets = " + ofToString(out->numberoffacets), 10, 160);		
	ofDrawBitmapString("number of holes = " + ofToString(out->numberofholes), 10, 180);		
	ofDrawBitmapString("number of regions = " + ofToString(out->numberofregions), 10, 200);		
	ofDrawBitmapString("number of facet constraints = " + ofToString(out->numberoffacetconstraints), 10, 220);		
	ofDrawBitmapString("number of segment constraints = " + ofToString(out->numberofsegmentconstraints), 10, 240);		
	ofDrawBitmapString("number of pbc groups = " + ofToString(out->numberofpbcgroups), 10, 260);		
	ofDrawBitmapString("number of trifaces = " + ofToString(out->numberoftrifaces), 10, 280);		
	ofDrawBitmapString("number of edges = " + ofToString(out->numberofedges), 10, 300);
	
	ofDrawBitmapString("number of voronoi points = " + ofToString(out->numberofvpoints), 10, 330);		
	ofDrawBitmapString("number of voronoi edges = " + ofToString(out->numberofvedges), 10, 350);		
	ofDrawBitmapString("number of voronoi facets = " + ofToString(out->numberofvfacets), 10, 370);		
	ofDrawBitmapString("number of voronoi cells = " + ofToString(out->numberofvcells), 10, 390);			
	
	cam.draw();

	vector<ofxVec3f> vpoints;
	vpoints.reserve(out->numberofvpoints);
	
	for(unsigned int i = 0; i < out->numberofvpoints; i++){
		vpoints.push_back(ofxVec3f(
							  out->vpointlist[3*i+0],
							  out->vpointlist[3*i+1],
							  out->vpointlist[3*i+2]));
	}
	
	// Example 1: Iterate over all edges
	for(unsigned int i = 0; i < out->numberofvedges; i++){
		tetgenio::voroedge *cur_edge = &(out->vedgelist[i]);
		ofxVec3f normal(cur_edge->vnormal[0], cur_edge->vnormal[1], cur_edge->vnormal[2]);
		
		// We can easily recover the primal Delaunay triangle:
		// It should be at the same index as the Voronoi edge in the list
		ofxVec3f primal_triangle[3];
		primal_triangle[0] = out->trifacelist[3*i+0];
		primal_triangle[1] = out->trifacelist[3*i+1];
		primal_triangle[2] = out->trifacelist[3*i+2];
		
		// The dual triangle's plane should be normal to the edge direction.
//		assert(Vec3Dot(normal, primal_triangle[1] - primal_triangle[0]) < EPSILON);
//		assert(Vec3Dot(normal, primal_triangle[2] - primal_triangle[0]) < EPSILON);
		
		// There are now 4 cases:
		//   The edge is finite in length,
		//   The edge is a line (shouldn't be possible with more than 3 non-degenerate points)
		//   The edge is a ray emanating from v1
		//   The edge is a ray emanating from v2
		if(cur_edge->v1 == -1){
			if(cur_edge->v2 != -1){
				// Edge is a ray emanating from v2
				ofxVec3f end2(vpoints[cur_edge->v2]);
				
				// Do something with end2 and normal
			}else{
				// Edge is a line
			}
		}else if(cur_edge->v2 == -1){
			if(cur_edge->v1 != -1){
				// Edge is a ray emanating from v1
				ofxVec3f end1(vpoints[cur_edge->v1]);
				
				// Do something with end2 and normal
			}else{
				// Edge is a line
			}
		}else{
			// Edge is finite length
			ofxVec3f end1(vpoints[cur_edge->v1]);
			ofxVec3f end2(vpoints[cur_edge->v2]);
			
			// Do something with the endpoints
		}
	}	
	
	
	// Example 2: Recover each facet
	//            We will run through all facets and extract the polygon.
	ofSetColor(255, 255, 255);
	for(unsigned int i = 0; i < out->numberofvfacets; i++){
		tetgenio::vorofacet *cur_facet = &(out->vfacetlist[i]);
		
		// First element in the facet list is the number of elements remaining,
		// which are actual indices into out->vedgelist
		unsigned int size = cur_facet->elist[0];
		
		// The primal edge should be at the same index in the edge list as this facet
		// if we used the "ee" flag when calling TetGen.
//		ofxVec3f primal_edge[2];
//		primal_edge[0] = out->edgelist[2*i+0];
//		primal_edge[1] = out->edgelist[2*i+1];
		
		// The adjacent cells are given by cur_facet->c1 and cur_facet->c2,
		// which are indices into out->vcelllist[]
		
		vector<unsigned int> vertex_indices; // the final list of indices in circulation order
		set<unsigned int> vertex_seen; // used to prevent duplicating voronoi vertex indices
		
		bool infinite_facet = false; // record whether the facet is finite-area or not
		for(unsigned int j = 0; j < size; j++){
			tetgenio::voroedge *cur_edge = &(out->vedgelist[cur_facet->elist[j+1]]);
			
			// Technically, the vedgelist is given in circulation order such that if there
			// are vedge rays, they will be the first and last in the list. However, it is
			// not necessary to depend on this.
			if(cur_edge->v1 == -1 || cur_edge->v2 == -1){
				infinite_facet = true;
				
				if(cur_edge->v1 != -1){
					if(vertex_seen.find(cur_edge->v1) == vertex_seen.end()){
						vertex_indices.push_back(cur_edge->v1);
						vertex_seen.insert(cur_edge->v1);
					}
				}else if(cur_edge->v2 != -1){
					if(vertex_seen.find(cur_edge->v2) == vertex_seen.end()){
						vertex_indices.push_back(cur_edge->v2);
						vertex_seen.insert(cur_edge->v2);
					}
				}
			}else{
				if(vertex_seen.find(cur_edge->v1) == vertex_seen.end()){
					vertex_indices.push_back(cur_edge->v1);
					vertex_seen.insert(cur_edge->v1);
				}
				if(vertex_seen.find(cur_edge->v2) == vertex_seen.end()){
					vertex_indices.push_back(cur_edge->v2);
					vertex_seen.insert(cur_edge->v2);
				}
			}
		}
		
		if(!infinite_facet){
			// For example, render the finite-area facet
			ofPushMatrix();
			ofScale(100, 100, 100);
			glBegin(GL_POLYGON);
			for(unsigned int j = 0; j < vertex_indices.size(); j++){
				glVertex3f(vpoints[vertex_indices[j]].x, vpoints[vertex_indices[j]].y, vpoints[vertex_indices[j]].z);
//				glVertex3f(vpoints[vertex_indices[j]]);
			}
			glEnd();
			ofPopMatrix();
		}else{
			// vertex_indices contains the indices into vpoints for the finite
			// points of the facet.
		}
	}	
	
	// Example 3: Recover each cell
	//            Here we will recover the entire polyhedron defining a Voronoi cell
	//            Note, however, that the facets will be unoriented. To recover
	//            oriented facets, one can exploit the fact that Voronoi cells are
	//            convex, and test the normal of the facet against the vector from a
	//            facet vertex to any point within the cell. If the dot product is
	//            negative, the facet indices need reversal. A convenient interior
	//            point to use is any average of all vertices used in the cell; any
	//            convex combination will do.
	ofSetColor(255, 0, 0);
	for(unsigned int i = 0; i < out->numberofvcells; i++){	
		// The primal vertex is at the same index:
		ofxVec3f primal_vertex(
						  out->pointlist[3*i+0],
						  out->pointlist[3*i+1],
						  out->pointlist[3*i+2]
						  );
		
		std::set<unsigned int> cell_indices; // the set of unique Voronoi vertex indices used in the cell
		
		for(unsigned int j = 0; j < out->vcelllist[i][0]; j++){
			tetgenio::vorofacet *cur_facet = &(out->vfacetlist[out->vcelllist[i][j+1]]);
			
			// Perform the iteration over facets like in Example 2, except also
			// add to cell_indices each time a Voronoi vertex is stored in vertex_indices.
		}
	}	
	
//	ofPushMatrix();
//	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);	
//	ofScale(20, 20, 20);	
//	
//	ofSetColor(0, 0, 0);
//	for (int i = 0; i < out->numberofpoints; i++) {
//		double x = out->pointlist[i*3];
//		double y = out->pointlist[i*3+1];
//		double z = out->pointlist[i*3+2];
//		ofxPoint(x, y, z);
//	}
//	
//	vector<ofxVec3f> triVerts;
//	for (int i = 0; i < out->numberoftrifaces; i++) {
//		int i1 = out->trifacelist[i*3];
//		int i2 = out->trifacelist[i*3+1];
//		int i3 = out->trifacelist[i*3+2];
//		double x1 = out->pointlist[i1];
//		double y1 = out->pointlist[i1+1];
//		double z1 = out->pointlist[i1+2];
//		triVerts.push_back(ofxVec3f(x1, y1, z1));
//		double x2 = out->pointlist[i2];
//		double y2 = out->pointlist[i2+1];
//		double z2 = out->pointlist[i2+2];
//		triVerts.push_back(ofxVec3f(x2, y2, z2));
//		double x3 = out->pointlist[i3];
//		double y3 = out->pointlist[i3+1];
//		double z3 = out->pointlist[i3+2];
//		triVerts.push_back(ofxVec3f(x3, y3, z3));		
//	}
//	
//	for (int i = 0; i < 1; i++) {
//		cp.setColorAngle(i/10);
//		cp.setColorScale(1.0);
//		cp.setColorRadius(1.0);
//		cp.update();
//		ofColor col = cp.getColor();
//		ofSetColor(col.r, col.g, col.b);	
//		ofxTriangleShape(triVerts[i], triVerts[i+1], triVerts[i+2]);
//	}
//	
//	
//	ofPopMatrix();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

