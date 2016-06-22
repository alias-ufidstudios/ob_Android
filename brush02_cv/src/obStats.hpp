#include "ofMain.h"
#include "ofApp.h"

namespace ob {

    struct stats{
        
    public:
        float min = 0;
        float max = 0;
        float sum = 0;
        float avg = 0;
        float mean = -123.345;
        
        int index_min = -12345;
        int index_max = -12345;
    };
    
    static stats audioStats;
    
    void clear(){
        audioStats.min = 0;
        audioStats.max = 0;
        audioStats.sum = 0;
        audioStats.avg = 0;
        audioStats.mean = -123;
        audioStats.index_min = -123456;
        audioStats.index_max = -123456;
    }
    
    void calc( const vector<float> & v ){
        
        float prev_min = audioStats.min;
        float prev_max = audioStats.max;
        
        for (unsigned int i=5; i<v.size()-5; i++) {
            
            const float f = v[i];
            const float af = abs(f);
            
            // min
            if( f < audioStats.min ){
                audioStats.min = f;
                audioStats.index_min = i;
            }
            
            // max
            if( f > audioStats.max ){
                audioStats.max = f;
                audioStats.index_max = i;
            }
            
            audioStats.sum += af;
        }
        
        // low pass
        if( audioStats.min<prev_min ){
            audioStats.min = audioStats.min*0.5 + prev_min*0.5;
        }else{
            audioStats.min = audioStats.min*0.1 + prev_min*0.9;
        }
        
        if( audioStats.max>prev_max ){
            audioStats.max = audioStats.max*0.5 + prev_max*0.5;
        }else{
            audioStats.max = audioStats.max*0.1 + prev_max*0.9;
        }
        
        audioStats.avg = audioStats.sum / v.size();
    }
    
    
    void dim(){
        audioStats.min *= 0.98;
        audioStats.max *= 0.98;
    }
}