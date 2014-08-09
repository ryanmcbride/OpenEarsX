#include "FliteController.h"
#include "AudioSessionManager.h"
#include "RuntimeVerbosity.h"

int openears_logging = 0;

FliteController::FliteController(){
    speechInProgress = FALSE;
    userCanInterruptSpeech = FALSE;
    quietFlite = FALSE;
    noAudioSessionOverrides = FALSE;
    audioSessionHasBeenInstantiated = FALSE;
}
FliteController::~FliteController(){
    
}

void FliteController::say(std::string statement, FliteVoice* voiceToUse){
    
	cst_voice *voice = voiceToUse->voice; // Flite uses this to generate speech
	
	const char *statementToSay = statement.c_str();
	
	if(target_mean > 0) {
		float actual_target_mean = voiceToUse->target_mean_default;
		float new_target_mean = actual_target_mean * target_mean;
		flite_feat_set_float(voice->features,"int_f0_target_mean",new_target_mean);
		
	}
	
	if(target_stddev > 0) {
		
		float actual_target_stddev = voiceToUse->target_stddev_default;
		float new_target_stddev = actual_target_stddev * target_stddev;
		flite_feat_set_float(voice->features,"int_f0_target_stddev",new_target_stddev);
		
	}
	
	if(duration_stretch > 0) {
		
		float actual_duration_stretch = voiceToUse->duration_stretch_default;
		float new_duration_stretch = actual_duration_stretch * duration_stretch;
		flite_feat_set_float(voice->features,"duration_stretch",new_duration_stretch);
		
	}
    
	cst_wave *speechwaveform = flite_text_to_wave(statementToSay,voice);
	
	// Let's make a virtual wav file
	
	char *headerstring;
    short headershort;
    int headerint;
    int numberofbytes;
	char* wavBuffer = (char*)malloc((speechwaveform->num_samples * 2) + 8 + 16 + 12 + 8);
	int writeoffset = 0;
    headerstring = "RIFF";
	memcpy(wavBuffer + writeoffset,headerstring,4);
	writeoffset += 4;
	
    numberofbytes = (speechwaveform->num_samples * 2) + 8 + 16 + 12;
	memcpy(wavBuffer + writeoffset,&numberofbytes,4);
	writeoffset += 4;
	
    headerstring = "WAVE";
	memcpy(wavBuffer + writeoffset,headerstring,4);
	writeoffset += 4;
    
    headerstring = "fmt ";
	memcpy(wavBuffer + writeoffset,headerstring,4);
	writeoffset += 4;
    
    numberofbytes = 16;
	memcpy(wavBuffer + writeoffset,&numberofbytes,4);
	writeoffset += 4;
    
    headershort = 0x0001;  // Type of sample, this is for PCM
	memcpy(wavBuffer + writeoffset,&headershort,2);
	writeoffset += 2;
    
    headershort = 1; // channels
	memcpy(wavBuffer + writeoffset,&headershort,2);
	writeoffset += 2;
    
    headerint = speechwaveform->sample_rate;  // rate
	memcpy(wavBuffer + writeoffset,&headerint,4);
	writeoffset += 4;
	
    headerint = (speechwaveform->sample_rate * 1 * sizeof(short)); // bytes per second
	memcpy(wavBuffer + writeoffset,&headerint,4);
	writeoffset += 4;
    
    headershort = (1 * sizeof(short)); // block alignment
	memcpy(wavBuffer + writeoffset,&headershort,2);
	writeoffset += 2;
    
    headershort = 2 * 8; // bits per sample
	memcpy(wavBuffer + writeoffset,&headershort,2);
	writeoffset += 2;
    
    headerstring = "data";
	memcpy(wavBuffer + writeoffset,headerstring,4);
	writeoffset += 4;
    
	headerint = (speechwaveform->num_samples * 2); // bytes in the sample buffer
	memcpy(wavBuffer + writeoffset,&headerint,4);
	writeoffset += 4;
	memcpy(wavBuffer + writeoffset,speechwaveform->samples,speechwaveform->num_samples * 2);
    
	int overallsize = (speechwaveform->num_samples * 1 * sizeof(short)) + 8 + 16 + 12 + 8;
    
    OpenEarsEventsObserver::get_instance().send_message("FliteDidStartSpeaking");
    audioPlayer(wavBuffer,overallsize);
    
	free(wavBuffer);
	delete_wave(speechwaveform);
	speechInProgress = true;
}
void FliteController::done_speaking(){
    OpenEarsEventsObserver::get_instance().send_message("FliteDidStopSpeaking");
    speechInProgress = false;
}

