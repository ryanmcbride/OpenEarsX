
#include "OpenEarsEventsObserver.h"

std::vector<OpenEarsEventsObserverDelegate*> OpenEarsEventsObserver::_delegates;
OpenEarsEventsObserver& OpenEarsEventsObserver::get_instance(){
    static OpenEarsEventsObserver s_observer;
    return s_observer;
}
OpenEarsEventsObserver::OpenEarsEventsObserver(){}
OpenEarsEventsObserver::~OpenEarsEventsObserver(){}
void OpenEarsEventsObserver::init(){}
void OpenEarsEventsObserver::send_message(std::string msg, std::string param1, std::string param2, std::string param3, std::string param4){
    for(auto delegate : _delegates){
        if(msg == "PocketsphinxDidReceiveHypothesis") {
            delegate->pocketsphinxDidReceiveHypothesis(param1,param2,param3);
        } else if(msg == "PocketsphinxDidDetectSpeech") {
            delegate->pocketsphinxDidDetectSpeech();
        } else if(msg == "PocketsphinxDidDetectFinishedSpeech") {
            delegate->pocketsphinxDidDetectFinishedSpeech();
        } else if(msg == "AudioSessionInterruptionDidBegin") {
            delegate->audioSessionInterruptionDidBegin();
        } else if(msg == "AudioSessionInterruptionDidEnd") {
            delegate->audioSessionInterruptionDidEnd();
        } else if(msg == "AudioInputDidBecomeUnavailable") {
            delegate->audioInputDidBecomeUnavailable();
        } else if(msg == "AudioInputDidBecomeAvailable") {
            delegate->audioInputDidBecomeAvailable();
        } else if(msg == "AudioRouteDidChangeRoute") {
            delegate->audioRouteDidChangeToRoute(param1);
        } else if(msg == "PocketsphinxDidStartCalibration") {
            delegate->pocketsphinxDidStartCalibration();
        } else if(msg == "PocketsphinxDidCompleteCalibration") {
            delegate->pocketsphinxDidCompleteCalibration();
        } else if(msg == "PocketsphinxRecognitionLoopDidStart") {
            delegate->pocketsphinxRecognitionLoopDidStart();
        } else if(msg == "PocketsphinxDidStartListening") {
            delegate->pocketsphinxDidStartListening();
        } else if(msg == "PocketsphinxDidStopListening") {
            delegate->pocketsphinxDidStopListening();
        } else if(msg == "PocketsphinxDidSuspendRecognition") {
            delegate->pocketsphinxDidSuspendRecognition();
        } else if(msg == "PocketsphinxDidResumeRecognition") {
            delegate->pocketsphinxDidResumeRecognition();
        } else if(msg == "PocketsphinxDidChangeLanguageModel") {
            delegate->pocketsphinxDidChangeLanguageModelToFile(param1,param2);
        } else if(msg == "FliteDidStartSpeaking") {
            delegate->fliteDidStartSpeaking();
        } else if(msg == "FliteDidFinishSpeaking") {
            delegate->fliteDidFinishSpeaking();
        } else if(msg == "PocketsphinxDidReceiveNbestHypothesisArray") {
            delegate->pocketsphinxDidReceiveNBestHypothesisArray(param1);
        } else if(msg == "PocketsphinxContinuousSetupDidFail") {
            delegate->pocketSphinxContinuousSetupDidFail();
        } else if(msg == "TestRecognitionCompleted") {
            delegate->testRecognitionCompleted();
        } else if(msg == "PocketsphinxFailedNoMicPermissions") {
            delegate->pocketsphinxFailedNoMicPermissions();
        } else if(msg == "MicPermissionCheckCompleted") {
            delegate->micPermissionCheckCompleted(param1 == "PermissionGranted"?true:false);
        } else if(msg == "LongRecognition") {
            delegate->longRecognition();
        }
    }
}
