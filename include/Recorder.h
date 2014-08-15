#ifndef __AUDIO_RECORDER_H__
#define __AUDIO_RECORDER_H__

#include <string>

class Recorder
{
public:
    static Recorder& get_instance();
    ~Recorder();
    
    void start(std::string path);
    void stop();
protected:
    Recorder();
    void* _recorder_impl;
};

#endif // __AUDIO_RECORDER_H__
