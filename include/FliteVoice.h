#ifndef __OPENEARS_FLITE_VOICE_H__
#define __OPENEARS_FLITE_VOICE_H__

 
#include "flite.h"
 
struct FliteVoice {
    int target_mean_default;
    int target_stddev_default;
    int duration_stretch_default;
    cst_voice *voice;
};

#endif
