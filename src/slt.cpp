#include "slt.h"

extern "C"{
void unregister_cmu_us_slt(cst_voice *vox);
cst_voice *register_cmu_us_slt(const char *voxdir);
}

Slt::Slt(){
    voice = register_cmu_us_slt(NULL);
    target_mean_default = 170.0;
    target_stddev_default = 15.0;
    duration_stretch_default = 1.0;
}
Slt::~Slt(){
    unregister_cmu_us_slt(voice);
}