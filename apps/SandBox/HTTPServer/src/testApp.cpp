#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetVerticalSync(true);
	ofBackground(255,255,255);
	ofSetDataPathRoot("../Resources/");

	server = ofxHTTPServer::getServer(); // get the instance of the server
	server->setServerRoot("www");		 // folder with files to be served
	server->setCallbackExtension("of");	 // extension of urls that aren't files but will generate a post or get event
	ofAddListener(server->postEvent,this,&testApp::postRequest);
	ofAddListener(server->getEvent,this,&testApp::getRequest);
	server->start(8888);
	
	font.loadFont("Pigiarniq Heavy.ttf", 20);
	r = 0; g = 0; b = 0; 

}

void testApp::getRequest(ofxHTTPServerResponse & response){ processRequest(response); }
void testApp::postRequest(ofxHTTPServerResponse & response){ processRequest(response); }
void testApp::processRequest(ofxHTTPServerResponse & response){
	if(response.url=="/getSMS.of"){
		string res = "";
		
		string start = 	
//		"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><TangibleHUBYesNoResponce><SMSs>";
		"<TangibleHUBYesNoResponce><SMSs>";

		urlParam = response.requestFields["time"];

		time_t t;
		time(&t);
		char gmttime[256];
		strftime(gmttime, 255, "%Y¥/%m¥/%d¥/%H¥/%I¥/%S", gmtime(&t));
		string timestr = gmttime;
		timestr = str_replace(timestr, "¥", "");		
		
		string yes = "<SMS><Answer>YES</Answer><Time>" + timestr + "</Time></SMS>";
		string no = "<SMS><Answer>NO</Answer><Time>" + timestr + "</Time></SMS>";
		string contents = "";
		for (int i = 0; i < ofRandom(1, 20); i++) {
			float f = ofRandomf();
			if (f > 0.0) {
				contents += yes;
			}else {
				contents += no;
			}
		}
		
		string end = "</SMSs></TangibleHUBYesNoResponce>";
		
		res = start+contents+end;
		
		response.response = res;
		r = ofRandom(200, 255);
		g = ofRandom(200, 255);
		b = ofRandom(200, 255);		
	}
}

//--------------------------------------------------------------
void testApp::update(){
	
	if (r > 0 && g > 0 && g > 0) {
		r-=2;g-=2;b-=2;
	}
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(r, g, b);
	font.drawString("HUB Simulator", 20, 35);
	ofDrawBitmapString(urlParam, 50, 65);
	
}

//--------------------------------------------------------------
string testApp::str_replace(const string &source,
							const string &pattern,
							const string &placement) {
    string result;
    string::size_type pos_before = 0;
    string::size_type pos = 0;
    string::size_type len = pattern.size();
    while ((pos = source.find(pattern, pos)) != string::npos) {
        result.append(source, pos_before, pos - pos_before);
        result.append(placement);
        pos += len;
        pos_before = pos;
    }
    result.append(source, pos_before, source.size() - pos_before);
    return result;
}
