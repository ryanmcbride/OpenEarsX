#ifndef __OPENEARS_FLITE_CONTROLLER_H__
#define __OPENEARS_FLITE_CONTROLLER_H__

#include <functional>
#include "OpenEarsEventsObserver.h"
#include "AudioConstants.h"
#include "flite.h"
#include "FliteVoice.h"

typedef std::function<void(const char*, int)> AudioPlayerCallback;

class FliteController{
public:
    FliteController();
    virtual ~FliteController();
    void set_audio_player_callback(AudioPlayerCallback callback){audioPlayer = callback;}
    void say(std::string statement, FliteVoice* voiceToUse);
    void done_speaking();
/**A read-only attribute that tells you the volume level of synthesized speech in progress. This is a UI hook. You can't read it on the main thread. */
    float fliteOutputLevel();
    // End methods to be called directly by an OpenEars project
    
/**duration_stretch changes the speed of the voice. It is on a scale of 0.0-2.0 where 1.0 is the default.*/
    float duration_stretch;
/**target_mean changes the pitch of the voice. It is on a scale of 0.0-2.0 where 1.0 is the default.*/
	float target_mean;
/**target_stddev changes convolution of the voice. It is on a scale of 0.0-2.0 where 1.0 is the default.*/    
	float target_stddev;
/**Set userCanInterruptSpeech to TRUE in order to let new incoming human speech cut off synthesized speech in progress.*/    
    bool userCanInterruptSpeech;
/**Set noAudioSessionOverrides to TRUE in order to run Flite without the OpenEars AudioSessionManager (useful if you aren't using PocketsphinxController and you want to set your own audiosession behavior or have none but the defaults). This can't be used in combination with PocketsphinxController, it is only for apps which used FliteController exclusively. This BOOL is only picked up once per instantiation of FliteController so if you want to switch back and forth it is necessary to release your FliteController and create a new FliteController with the new noAudioSessionOverrides setting. */        
    bool noAudioSessionOverrides;
    AudioPlayerCallback audioPlayer; // Plays back the speech
	void* speechData;
	bool speechInProgress;
    bool quietFlite;
    bool audioSessionHasBeenInstantiated;
};


#endif





