#include "ofMain.h"
#include "ofApp.h"

namespace ob {

    struct DrawerSettings{
    public:
        ofApp * app;
        ofVec2f indicator;
        vector<float> * data;
        float track_len;
        int buffer_size;
        float xrate;
        float global_amp;
    };

    static DrawerSettings dset;

    // util
    void drawDots( const vector<ofVec3f> &v ) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &v[0].x);
        glDrawArrays(GL_POINTS, 0, v.size());
    }

    void drawDots( const vector<ofVec2f> &v ) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, sizeof(ofVec2f), &v[0].x);
        glDrawArrays(GL_POINTS, 0, v.size());
    }

    void drawLines( const vector<ofVec2f> &v ) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, sizeof(ofVec2f), &v[0].x);
        glDrawArrays(GL_LINES, 0, v.size());
    }

    void drawLines( const vector<ofVec3f> &v ) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &v[0].x);
        glDrawArrays(GL_LINES, 0, v.size());
    }

    void drawLineStrip( const vector<ofVec3f> &v ) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &v[0].x);
        glDrawArrays(GL_LINE_STRIP, 0, v.size());
    }

    void draw_line_wave( int start, int n, float local_amp=1 ){

        vector<ofVec3f> v;
        float amp = dset.global_amp * local_amp;

        for( int i=0; i<n; i++){
            int id = start + i;
            float d1 = (*dset.data)[id];
            float x1 = id * dset.xrate;
            float y1 = d1 * amp;
            v.push_back( ofVec3f(x1,y1,0) );
        }

        ofFloatColor c(0,0,0,1);
        ofSetColor(c);
        drawLineStrip(v);
    }

    void draw_dot_wave( int start, int n, float local_amp=1 ){

        vector<ofVec3f> v(n);
        float amp = dset.global_amp * local_amp;

        for( int i=0; i<n; i++){
            int id = start + i;
            float d = (*dset.data)[id];
            float x = id * dset.xrate;
            float y = d * amp;
            v[i] = ofVec3f(x,y,0);
        }
        ofFloatColor c(0,0,0,1);
        ofSetColor( c );
        drawDots( v );
    }

    void draw_prep_line( int start, int n, float local_amp=1 ){

        vector<ofVec3f> v;
        float amp = dset.global_amp * local_amp;

        for( int i=0; i<n; ){
            int id = start + i;
            float d = (*dset.data)[id];
            float x = id * dset.xrate;
            float y = d * amp;

            v.push_back( ofVec3f(x, y, 0) );
            v.push_back( ofVec3f(x, 0, 0) );
            i += ofRandom(1, 3);
        }

        ofFloatColor c(0,0,0,1);
        ofSetColor( c );
        drawLines( v );
    }

    void draw_prep_line_inv( int start, int n, float local_amp=1 ){

        vector<ofVec3f> v;
        float amp = dset.global_amp * local_amp;

        for( int i=0; i<n; ){
            int id = start + i;
            float d = (*dset.data)[id];
            float sign = d<0 ? -1:1;
            float x = id * dset.xrate;
            float y = d * amp;
            v.push_back( ofVec3f(x, y, 0) );
            v.push_back( ofVec3f(x, amp*sign*0.25, 0) );
            i += ofRandom(3, 10);
        }

        ofFloatColor c(0,0,0,0.5);
        ofSetColor( c );
        drawLines(v);
    }

    void draw_circle( int start, int n, float local_amp=1 ){

        vector<ofVec3f> v;
        float amp = dset.global_amp * local_amp;

        int res = 360*0.4;
        float angleAdder = 360.0f/res;

        int id = start + n*0.5;
        float d = (*dset.data)[id];
        float center_x = id * dset.xrate;
        float center_y = (d+ofRandom(-0.05f, 0.05f))*amp;
        float diam = amp * ofRandom(0.005, 0.04+d);
        for(int i=0; i<res; ){
            float angle = i * angleAdder;
            float x = diam*cos( ofDegToRad(angle)) + center_x;
            float y = diam*sin( ofDegToRad(angle) ) + center_y;

            v.push_back(ofVec3f(x, y, 0));

            i += ofRandom(1, 6);
        }
        ofFloatColor c(0,0,0,1);
        ofSetColor( c );
        drawDots(v);

        if( ofRandomf() >0.1 ){
            ofDrawLine(center_x, center_y, center_x, 0);
        }
    }

    void draw_rect( int start, int n, float local_amp=1 ){

        float amp = dset.global_amp * local_amp;

        //start
        float d1 = (*dset.data)[start];
        float x1 = start * dset.xrate;
        float y1 = d1 * amp;

        // end
        float d2 = (*dset.data)[start+n-1];
        float x2 = (start+n-1) * dset.xrate;
        float y2 = d2 * amp;

        ofNoFill(); ofSetColor(ofFloatColor(1,0,0,1));
        ofDrawRectangle(x1, y1, x2-x1, y2-y1);
    }

    void draw_log_wave( int start, int n, float local_amp=1 ){

        vector<ofVec3f> v;
        float amp = dset.global_amp * local_amp;

        float strength = ofRandom(2, 10);
        float sx = start * dset.xrate;
        v.push_back( ofVec3f(sx,0,0));

        //float ep = (*dset.data)[start+n];
        ofVec3f point;
        for( int i=0; i<n; i++){
            int id = start + i;
            float d = (*dset.data)[id];
            float sign = d<0 ? -1:1;
            float log = log10(1+abs(d)*strength) / log10(1+strength);
            float x = id * dset.xrate;
            float y = log * amp * sign;
            point.set(x,y,0);
            v.push_back(point);
        }

        v.push_back(ofVec3f(point.x,0,0));

        ofSetColor( ofFloatColor(0,0,0,1) );
        drawLineStrip(v);
    }

    void draw_arc( int start, int n, float local_amp=1 ){

        vector<ofVec3f> v;
        float amp = dset.global_amp * local_amp;

        float totalAngle = ofRandom(1, 4) * 45;
        float startAngle = ofRandom(-4, 4) * 45;
        float angleAdder = ofRandom(0.1f,1.5f);
        int nPoints = totalAngle/angleAdder;

        int id = start;
        float d = (*dset.data)[id];
        float center_x = id * dset.xrate;
        float center_y = (d+ofRandom(0.1f, 0.3f))*amp;
        float diam = amp*0.5 + ofRandom(0, amp);

        for(int i=0; i<nPoints; ){
            float angle = i * angleAdder + startAngle;
            float x = diam*cos( ofDegToRad(angle)) + center_x;
            float y = diam*sin( ofDegToRad(angle) ) + center_y;
            v.push_back( ofVec3f(x, y, 0) );

            i += ofRandom(1, 6);
        }

        ofFloatColor c(0,0,0,1);
        ofSetColor(c);
        drawDots( v );
    }
}