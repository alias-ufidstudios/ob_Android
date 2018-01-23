#include "BrushDataHandler.h"
#include "ofMain.h"
#include "ofxAndroidUtils.h"
#include "ofApp.h"

void BrushDataHandler::getDataFromServer(){
    ofLogNotice("BrushDataHandler") << "Loading from server";

    //ofxiOSAlerts.addListener(this);
    ofRegisterURLNotification(this);
    
    requestBearer(appId, appKey);
}

ofxJSONElement BrushDataHandler::getDataFromDummyFile(string path){
    
    ofLogNotice("BrushDataHandler") << "Loading Dummy file";
    
    ofxJSONElement json;
    bool parsingSuccessful = json.open(path);
    
    if (parsingSuccessful){
        ofLogNotice("BrushDataHandler") << "parse JSON file : OK";
        //cout << result.getRawString() << endl;
        //createData(result);
    }else{
        ofLogError("BrushDataHandler")  << "parse JSON file : ERROR";
    }
    
    ofLogNotice("dummy data") << json.getRawString();
    return json;
}

void BrushDataHandler::requestBearer(string appId, string appKey){
    ofLogNotice("BrushDataHandler") << "Request bearer";
    ofHttpRequest req;
    req.url = baseurl + "/bearertoken/" + appId + "?key=" + appKey;
    req.name = "bear";
    loader.handleRequestAsync(req);
}

void BrushDataHandler::requestAuthUrl(string bearer){
    ofLogNotice("BrushDataHandler") << "Request Auth";
    ofHttpRequest req;
    req.url = baseurl + "/authorize";
    req.headers["Authorization"] = "Bearer " + bearer;
    req.name = "auth";
    loader.handleRequestAsync(req);
}


void BrushDataHandler::requestSessionData(string url){

    // request session data
    // example https://api.developer.oralb.com/v1/sessions?from=2015-02-20T12:40:45.327-07:00&to=

    ofLogNotice("requestSessionData") << url;
    // TODO better string operation
    userToken = url.erase(0, 43); // remove http://dominofactory.net/obTest?userToken=
    ofLogNotice("userToken") << userToken;
    
    ofHttpRequest req;
    req.url = baseurl + "/sessions?from=2015-02-20T12:40:45.327-07:00";
    req.headers["X-User-Token"] = userToken;
    req.headers["Authorization"] = "Bearer " + bear;
    req.name = "session data";
    loader.handleRequestAsync(req);
    ofLogNotice("BrushDataHandler") << "Request Session Data";
}

local_date_time BrushDataHandler::get_ldt(string s){
    local_date_time ldt(not_a_date_time);
    stringstream ss(s);

    try
    {
        boost::local_time::local_time_input_facet* ifc= new boost::local_time::local_time_input_facet("%Y-%m-%dT%H:%M:%S%F%Q");
        ifc->set_iso_extended_format();
        ss.imbue(std::locale(ss.getloc(), ifc));
        ss >> ldt;
    }
    catch( std::exception const& e )
    {
        cout << "ERROR:" << e.what() <<std::endl;
    }
 
    return ldt;
}

void BrushDataHandler::urlResponse(ofHttpResponse & response){

    string name = response.request.name;
    int status = response.status;

    if(response.status==200){
        ofxJSONElement json;
        ofBuffer buf = response.data;
        string raw = ofToString(buf);
        json.parse(raw);
        
        if(name == "bear"){
            bear = json["bearerToken"].asString();
            ofLogNotice("BrushDataHandler") << "got bearer " << bear;
            requestAuthUrl(bear);
        }else if(name == "auth"){
            authUrl = json["url"].asString();
            ofLogNotice("BrushDataHandler") << "got authURL " << authUrl;
            //ofxAndroidPauseApp();
            //ofLaunchBrowser(authUrl); // -> destroy
            ofApp::get().webView.showWebView(authUrl);
        }else if(name == "session data"){
            //createData(json);
            ofLogNotice("session data") << "arrived";
        }
    }else{
        ofLogError("url response") << status << " " << response.error << " for request " << name;
        if(status!=-1){}
    }
}

BrushDataHandler::~BrushDataHandler(){
    //ofxiOSAlerts.removeListener(this);
    ofUnregisterURLNotification(this);
}
