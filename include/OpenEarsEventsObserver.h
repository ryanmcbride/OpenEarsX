#ifndef __OPENEARS_EVENTS_OBSERVER_H__
#define __OPENEARS_EVENTS_OBSERVER_H__

#include <string>
#include <vector>

class OpenEarsEventsObserverDelegate;

class OpenEarsEventsObserver{
public:
    static OpenEarsEventsObserver& get_instance();
    virtual ~OpenEarsEventsObserver();
    void setDelegate(OpenEarsEventsObserverDelegate* delegate){_delegates.push_back(delegate);}
    void send_message(std::string msg, std::string param1="", std::string param2="", std::string param3="", std::string param4="");
protected:
    OpenEarsEventsObserver();
    void init();
    //void createNotificationObserverOnMainThread();
    
    static std::vector<OpenEarsEventsObserverDelegate*> _delegates;
};

// A protocol for delegates of OpenEarsEventsObserver.
class OpenEarsEventsObserverDelegate{
public:
    OpenEarsEventsObserverDelegate(){}
    virtual ~OpenEarsEventsObserverDelegate(){}
// Audio Session Status Methods.
/** There was an interruption.*/
    virtual void audioSessionInterruptionDidBegin(){}
/** The interruption ended.*/
    virtual void audioSessionInterruptionDidEnd(){}
/** The input became unavailable.*/
    virtual void audioInputDidBecomeUnavailable(){}
/** The input became available again.*/
    virtual void audioInputDidBecomeAvailable(){}
/** The audio route changed.*/
    virtual void audioRouteDidChangeToRoute(std::string newRoute){}

// Pocketsphinx Status Methods.
/** Pocketsphinx isn't listening yet but it started calibration.*/
    virtual void pocketsphinxDidStartCalibration(){}
/** Pocketsphinx isn't listening yet but calibration completed.*/
    virtual void pocketsphinxDidCompleteCalibration(){}
/** Pocketsphinx isn't listening yet but it has entered the main recognition loop.*/
    virtual void pocketsphinxRecognitionLoopDidStart(){}
/** Pocketsphinx is now listening.*/
    virtual void pocketsphinxDidStartListening(){}
/** Pocketsphinx heard speech and is about to process it.*/
    virtual void pocketsphinxDidDetectSpeech(){}
/** Pocketsphinx detected a second of silence indicating the end of an utterance*/
    virtual void pocketsphinxDidDetectFinishedSpeech(){}
/** Pocketsphinx has a hypothesis.*/
    virtual void pocketsphinxDidReceiveHypothesis(std::string hypothesis, std::string recognitionScore, std::string utteranceID){}
/** Pocketsphinx has an n-best hypothesis dictionary.*/
    virtual void pocketsphinxDidReceiveNBestHypothesisArray(std::string hypothesisArray){}
/** Pocketsphinx has exited the continuous listening loop.*/
    virtual void pocketsphinxDidStopListening(){}
/** Pocketsphinx has not exited the continuous listening loop but it will not attempt recognition.*/
    virtual void pocketsphinxDidSuspendRecognition(){}
/** Pocketsphinx has not existed the continuous listening loop and it will now start attempting recognition again.*/
    virtual void pocketsphinxDidResumeRecognition(){}
/** Pocketsphinx switched language models inline.*/
    virtual void pocketsphinxDidChangeLanguageModelToFile(std::string newLanguageModelPathAsString,std::string newDictionaryPathAsString){}
/** Some aspect of setting up the continuous loop failed, turn on OpenEarsLogging for more info.*/
    virtual void pocketSphinxContinuousSetupDidFail(){}
/** Your test recognition run has completed.*/
    virtual void testRecognitionCompleted(){}
/** Pocketsphinx couldn't start because it has no mic permissions (will only be returned on iOS7 or later).*/
    virtual void pocketsphinxFailedNoMicPermissions(){}
/** The user prompt to get mic permissions, or a check of the mic permissions, has completed with a TRUE or a FALSE result  (will only be returned on iOS7 or later).*/
    virtual void micPermissionCheckCompleted(bool result){}

// Flite Status Methods.
/** Flite started speaking. You probably don't have to do anything about this.*/
    virtual void fliteDidStartSpeaking(){}
/** Flite finished speaking. You probably don't have to do anything about this.*/
    virtual void fliteDidFinishSpeaking(){}

    virtual void longRecognition(){}
};

#endif
