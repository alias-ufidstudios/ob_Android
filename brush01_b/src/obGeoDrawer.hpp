#include "ofMain.h"
#include "ofApp.h"

namespace ob {

    struct settings{
    public:
        ofApp * app;
        ofVec2f indicator;
        vector<float> * data;
        float track_len;
        int bufferSize;
        float xrate;
    };

    // util

    void drawDots( const vector<ofVec3f> &v ) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &v[0].x);
        glDrawArrays(GL_POINTS, 0, v.size());
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

    void draw_line_wave( settings & s, int start, int n, float amp ){

        vector<ofVec3f> v;

        for( int i=0; i<n; i++){
            int id = start + i;
            float d1 = (*s.data)[id];
            float x1 = id * s.xrate;
            float y1 = d1 * amp;
            v.push_back( ofVec3f(x1,y1,0) );
        }

        ofFloatColor c(0,0,0,1);
        ofSetColor(c);
        drawLineStrip(v);
    }

    void draw_dot_wave( settings & s, int start, int n, float amp ){

        vector<ofVec3f> dots(n);

        for( int i=0; i<n; i++){
            int id = start + i;
            float d = (*s.data)[id];
            float x = id * s.xrate;
            float y = d * amp;

            bool inv = s.indicator.x > x;

            dots[i] = ofPoint(x,y,0);
        }
        ofFloatColor c(0,0,0,1);
        ofSetColor( c );
        drawDots( dots );
    }

    void draw_prep_line( settings & s, int start, int n, float amp ){

        vector<ofVec3f> v;

        for( int i=0; i<n; ){
            int id = start + i;
            float d = (*s.data)[id];
            float x = id * s.xrate;
            float y = d * amp;

            bool inv = s.indicator.x > x;

            v.push_back( ofVec3f(x, y, 0) );
            v.push_back( ofVec3f(x, 0, 0) );
            i += ofRandom(1, 3);
        }

        ofFloatColor c(0,0,0,1);
        ofSetColor( c );
        drawLines( v );
    }

    void draw_prep_line_inv( settings & s, int start, int n, float amp ){

        vector<ofVec3f> v;

        for( int i=0; i<n; ){
            int id = start + i;
            float d = (*s.data)[id];
            float sign = d<0 ? -1:1;
            float x = id * s.xrate;
            float y = sign * (1.0f-abs(d)) * amp;

            bool inv = s.indicator.x > x;

            v.push_back( ofVec3f(x, y, 0) );
            v.push_back( ofVec3f(x, 0, 0) );
            //ofDrawLine( ofVec3f(x, y, 0), ofVec3f(x, 0, 0));
            i += ofRandom(3, 10);
        }

        ofFloatColor c(0,0,0,0.5);
        ofSetColor( c );
        drawLines(v);
    }
    void draw_circle( settings & s, int start, int n, float amp ){

        vector<ofVec3f> dots;
        int res = 360*0.4;
        float angleAdder = 360.0f/res;

        int id = start + n*0.5;
        float d = (*s.data)[id];
        float center_x = id * s.xrate;
        float center_y = (d+ofRandom(-0.05f, 0.05f))*amp;
        float diam = amp * ofRandom(0.005, 0.04+d);
        for(int i=0; i<res; ){
            float angle = i * angleAdder;
            float x = diam*cos( ofDegToRad(angle)) + center_x;
            float y = diam*sin( ofDegToRad(angle) ) + center_y;

            bool inv = s.indicator.x > x;
            //if(inv) c.invert();

            dots.push_back(ofVec3f(x, y, 0));

            i += ofRandom(1, 6);
        }
        ofFloatColor c(0,0,0,1);
        ofSetColor( c );
        drawDots(dots);
    }

    void draw_rect( settings & s, int start, int n, float amp ){

        //start
        float d1 = (*s.data)[start];
        float x1 = start * s.xrate;
        float y1 = d1 * amp;

        // end
        float d2 = (*s.data)[start+n-1];
        float x2 = (start+n-1) * s.xrate;
        float y2 = d2 * amp;

        ofNoFill(); ofSetColor(ofFloatColor(1,0,0,1));
        ofDrawRectangle(x1, y1, x2-x1, y2-y1);
    }

    void draw_log_wave( settings & s, int start, int n, float amp ){

        vector<ofVec3f> v;

        float strength = ofRandom(3, 10);

        float sd = (*s.data)[start];
        float ssign = sd<0 ? -1 : 1;
        float slog = log10(abs(sd)*strength) / log10(strength);
        float sx = start * s.xrate;
        float sy = slog * amp * ssign;

         v.push_back( ofVec3f(sx,0,0));
        v.push_back( ofVec3f(sx,sy,0));

        float ep = (*s.data)[start+n];

        for( int i=0; i<n-1; i++){
            int id1 = start + i;
            int id2 = start + i + 1;

            float d1 = (*s.data)[id1]; // 0~1
            float d2 = (*s.data)[id2];
            float sign1 = d1<0 ? -1:1;
            float sign2 = d2<0 ? -1:1;

            float log1 = log10(abs(d1)*strength) / log10(strength);
            float log2 = log10(abs(d2)*strength) / log10(strength);

            float x1 = id1 * s.xrate;
            float x2 = id2 * s.xrate;
            float y1 = log1 * amp * sign1;
            float y2 = log2 * amp * sign2;

            bool inv = s.indicator.x > x1;

            v.push_back(ofVec3f(x1,y1,0));
            v.push_back(ofVec3f(x2,y2,0));
        }

        float ed = (*s.data)[start+n];
        float esign = ed<0 ? -1 : 1;
        float elog = log10(abs(ed)*strength) / log10(strength);
        float ex = (start+n) * s.xrate;
        float ey = elog * amp * esign;

        v.push_back(ofVec3f(ex,ey,0));
        v.push_back(ofVec3f(ex,0,0));

        ofSetColor( ofFloatColor(0,0,0,1) );
        drawLines(v);
    }

    void draw_arc( settings & s, int start, int n, float amp ){

        vector<ofVec3f> dots;

        float totalAngle = ofRandom(2, 8) * 15;
        float startAngle = ofRandom(-360, 360);
        float angleAdder = ofRandom(0.2f,1.5f);
        int nPoints = totalAngle/angleAdder;

        int id = start;
        float d = (*s.data)[id];
        float center_x = id * s.xrate;
        float center_y = (d+ofRandom(0.1f, 0.2f))*amp;
        float diam = amp + ofRandom(0, amp);

        for(int i=0; i<nPoints; ){
            float angle = i * angleAdder + startAngle;
            float x = diam*cos( ofDegToRad(angle)) + center_x;
            float y = diam*sin( ofDegToRad(angle) ) + center_y;
            dots.push_back( ofPoint(x, y, 0) );

            bool inv = s.indicator.x > x;

            i += ofRandom(1, 8);
        }

        ofFloatColor c(0,0,0,1);
        ofSetColor(c);
        drawDots( dots );
    }
}