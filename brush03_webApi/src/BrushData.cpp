#include "BrushData.h"

void BrushData::createData(ofxJSONElement & elem, vector<BrushData> & data){
    
    const Json::Value& sessions = elem["sessions"];
    int size = sessions.size();
    
    data.clear();
    data.assign(size, BrushData());
    
    for (Json::ArrayIndex i=0; i<size; ++i){
        
        BrushData & b = data[i];
        
        // ISO 8601 extended format
        // "timeEnd" : "2017-10-13T17:38:40.000+02:00"
        
        // Get std::tm
        
        if(1){
            string start_s   = sessions[i]["timeStart"].asString().substr(0, 19);
            string end_s  = sessions[i]["timeEnd"].asString().substr(0, 19);
            strptime(start_s.c_str(), "%Y-%m-%dT%H:%M:%S", &b.start);
            strptime(end_s.c_str(), "%Y-%m-%dT%H:%M:%S", &b.end);
        }else{
            std::istringstream st (sessions[i]["timeStart"].asString().substr(0, 19));
            std::istringstream end(sessions[i]["timeEnd"].asString().substr(0, 19));
            st  >> std::get_time(&b.start, "%Y-%m-%dT%H:%M:%S");
            end >> std::get_time(&b.end, "%Y-%m-%dT%H:%M:%S");
        }
        std::mktime(&b.start);
        std::mktime(&b.end);
        
        b.duration      = sessions[i]["brushingDuration"].asInt();
        b.pressureCount = sessions[i]["pressureCount"].asInt();
        b.pressureTime  = sessions[i]["pressureTime"].asInt();
    }
    
    ofLogNotice("BrushDataHandler") << "created " << data.size() << " sesion data";
}
