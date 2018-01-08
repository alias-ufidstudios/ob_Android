#include "Np.h"

Np::Np(){
    
    mesh.setMode(OF_PRIMITIVE_LINES);
    mesh.setUsage(GL_STATIC_DRAW);
}

void Np::addVertices(const vector<BrushData> & data, ob::plot::TYPE type, float minRad, float maxRad){
    
    static const glm::vec2 xAxis(1,0);
    
    for(int i=0; i<data.size(); i++){
        
        const PlotData & p = data[i].plot.at(type);
        const glm::vec2 & pos1 = p.stPos;
        glm::vec2 n1 = glm::normalize(pos1);
        float angle = glm::angle(xAxis, n1);
        
        if(minRad<angle && angle<=maxRad){
            pos.push_back(pos1);
        }
    }
}

void Np::create(int numLine){
    
    //float min = std::numeric_limits<float>::min();
    float max = std::numeric_limits<float>::max();
    
    multimap<float, glm::vec2> near_p;
    
    for(int i=0; i<pos.size(); i++){
        
        glm::vec2 & p1 = pos[i];
        
        near_p.clear();
        for( int line=0; line<numLine; line++ ){
            near_p.insert({max, glm::vec2(-12345,0)});
        }
        
        for(int j=i; j<pos.size(); j++){
            if(i==j) continue;
            
            glm::vec2 & p2 = pos[j];
            
            float dist = glm::distance(p1, p2);
            if(50<dist && dist<80){
                
                multimap<float, glm::vec2>::iterator itr = near_p.end();
                itr--;
                if(dist < itr->first){
                    near_p.insert({dist, p2});
                    multimap<float, glm::vec2>::iterator end = near_p.end();
                    near_p.erase( --end );
                }
            }
        }
        
        multimap<float, glm::vec2>::iterator itr = near_p.begin();
        for(int j=0; itr!=near_p.end(); itr++, j++ ){
            glm::vec2 & p2 = itr->second;
            if(p2.x != -12345){
                mesh.addVertex(glm::vec3(p1.x, p1.y, 0));
                mesh.addVertex(glm::vec3(p2.x, p2.y, 0));
                mesh.addColor(ofFloatColor(0,0,0,0.5));
                mesh.addColor(ofFloatColor(0,0,0,0.5));
            }
        }
    }
}

void Np::draw(){
    mesh.draw();
    
    for(int i=0; i<pos.size(); i++){
        ofFill();
        ofSetColor(0,0,0,200);
        ofDrawCircle(pos[i].x, pos[i].y, 2);
    }
}

void Np::clear(){
    pos.clear();
    mesh.clear();
}
