#pragma once

#include "ofMain.h"
#include "boost/polygon/voronoi.hpp"
#include "boost/polygon/segment_data.hpp"
#include "BrushData.h"

using namespace std;
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
typedef boost::polygon::point_data<float> vPoint;
typedef boost::polygon::segment_data<float> vSegment;
typedef voronoi_diagram<double>::cell_type cell_type;
typedef voronoi_diagram<double>::edge_type edge_type;
typedef voronoi_diagram<double>::vertex_type vertex_type;

class Voro{
    
public:
    
    Voro(){};
    
    void addVertices(const vector<BrushData> & data, ob::plot::TYPE type, float minRad, float maxRad);
    void create();
    void draw();
    void clear();
    
    vector<vPoint> vPs;
    voronoi_diagram<double> vD;
    
};
