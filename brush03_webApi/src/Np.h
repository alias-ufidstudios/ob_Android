#pragma once

#include "ofMain.h"
#include "BrushData.h"

class Np{
    
public:
    
    Np();
    
    void addVertices(const vector<BrushData> & data, ob::plot::TYPE type, float minRad, float maxRad);
    void create(int numLine);
    void draw();
    void clear();
    
    vector<glm::vec2> pos;
    ofVboMesh mesh;
    
};
