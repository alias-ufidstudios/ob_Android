#include "Voro.h"
#include <unordered_map>

void Voro::clear(){
    vPs.clear();
    vD.clear();
}

void Voro::addVertices(const vector<BrushData> & data, ob::plot::TYPE type, float minRad, float maxRad){

    static const glm::vec2 xAxis(1,0);

    for(int i=0; i<data.size(); i++){
        
        const PlotData & p = data[i].plot.at(type);
        const glm::vec2 & pos1 = p.stPos;
        glm::vec2 n1 = glm::normalize(pos1);
        float angle = glm::angle(xAxis, n1);
        
        if(minRad<angle && angle<=maxRad){
            vPs.push_back(vPoint(pos1.x, pos1.y));
        }
    }
}

void Voro::create(){
    vD.clear();
    boost::polygon::construct_voronoi(vPs.begin(), vPs.end(), &vD);
}

void Voro::draw(){
    if( 1 ){
        voronoi_diagram<double>::const_cell_iterator it = vD.cells().begin();
        for (int i=0; it!= vD.cells().end(); ++it, i++) {
            
            const cell_type& cell = *it;
            const edge_type* edge = cell.incident_edge();
            
            do{
                if(edge->is_primary()){
                    if( edge->is_finite() ){
                        if (edge->cell()->source_index() < edge->twin()->cell()->source_index()){
                            float x0 = edge->vertex0()->x();
                            float y0 = edge->vertex0()->y();
                            float x1 = edge->vertex1()->x();
                            float y1 = edge->vertex1()->y();
                            
                            if(1){
                                glm::vec2 v0(x0, y0);
                                glm::vec2 v1(x1, y1);
                                glm::vec2 d = v1 - v0;
                                float length = glm::length(d);
                                float space = ofGetWidth() * 0.01;
                                if(length>space*2){
                                    v0 += d*(space/length);
                                    v1 -= d*(space/length);
                                    
                                    x0 = v0.x;
                                    y0 = v0.y;
                                    x1 = v1.x;
                                    y1 = v1.y;
                                }
                            }
                            float limity = ofGetHeight()/2;
                            float limitx = ofGetWidth()/2;
                            bool xOver = ( abs(x0)>limitx || abs(x1)>limitx );
                            bool yOver = ( abs(y0)>limity || abs(y1)>limity );
                            
                            bool draw = !xOver && !yOver;
                            
                            if(draw){
                                ofFill();
                                ofSetColor(0,0,0, 150);
                                ofSetLineWidth(1);
                                ofDrawLine(x0,y0,0,x1,y1,0);
                                ofDrawCircle(x0,y0,2);
                                ofDrawCircle(x1,y1,2);
                            }
                        }
                    }else{
                        if( 0 ){
                            const vertex_type * v0 = edge->vertex0();
                            if( v0 ){
                                vPoint p1 = vPs[edge->cell()->source_index()];
                                vPoint p2 = vPs[edge->twin()->cell()->source_index()];
                                float x0 = edge->vertex0()->x();
                                float y0 = edge->vertex0()->y();
                                float end_x = (p1.y() - p2.y()) * ofGetWidth();
                                float end_y = (p1.x() - p2.x()) * -ofGetWidth();
                                ofDrawLine(x0,y0,0,end_x,end_y,0);
                            }
                        }
                    }
                }
                edge = edge->next();
            }while (edge != cell.incident_edge());
        }
    }
    
    // draw Point
    if( 0 ){
        glPointSize(3);
        ofSetColor(255, 0,0);
        for( auto v : vPs ){
            ofDrawCircle(v.x(), v.y(), 2);
        }
    }
    
}
