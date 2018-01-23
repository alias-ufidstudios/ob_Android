#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
//#include "ofxiOSAlerts.h"
#include "boost/date_time.hpp"
#include "BrushData.h"

using namespace boost::date_time;
using namespace boost::gregorian;
using namespace boost::local_time;
using namespace boost::posix_time;

class BrushDataHandler{//}; : public ofxiOSAlertsListener{

public:
    BrushDataHandler(){};
    ~BrushDataHandler();

    void getDataFromServer();
    ofxJSONElement getDataFromDummyFile(string path);
    void createData(ofxJSONElement & elem, vector<BrushData> & data);
    
    void urlResponse(ofHttpResponse & response);    // oF callback
    void requestSessionData(string url);
    
    local_date_time get_ldt(string ss);

    void requestBearer(string appId, string appKey);
    void requestAuthUrl(string bearer);

    ofURLFileLoader loader;

    string baseurl = "https://api.developer.oralb.com/v1";
    string appId = "fac078cd-1cf0-4e04-82ab-385a98359d09";
    string appKey = "6bb594ec-2860-427e-8e34-891fdb33995d";
    string bear = "n.a";
    string authUrl = "n.a";
    string userToken = "n.a";
    
};

