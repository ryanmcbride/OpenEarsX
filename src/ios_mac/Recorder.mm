#include "Recorder.h"
#import "EZMicrophone.h"
#import "EZRecorder.h"


@interface RecorderImpl : NSObject<EZMicrophoneDelegate>
@property (nonatomic,strong) EZMicrophone *microphone;
@property (nonatomic,strong) EZRecorder *recorder;
@property (nonatomic,assign) BOOL isRecording;
-(void)start: (NSString*) path;
-(void)stop;
@end

@implementation RecorderImpl
@synthesize microphone;
@synthesize recorder;
@synthesize isRecording;

-(id) init{
    [super init];
    self.microphone = [EZMicrophone microphoneWithDelegate:self];
    [self.microphone retain];
    return self;
}
-(void)start: (NSString*) path{
    [self.microphone startFetchingAudio];
    self.isRecording = YES;
    self.recorder = [EZRecorder recorderWithDestinationURL:[NSURL fileURLWithPath:path]
                                              sourceFormat:self.microphone.audioStreamBasicDescription
                                       destinationFileType:EZRecorderFileTypeRAW];
}
-(void)dealloc{
    [self.microphone release];
}
-(void)stop
{
    [self.microphone stopFetchingAudio];
    self.isRecording = NO;
    [self.recorder closeAudioFile];
}

#pragma mark - EZMicrophoneDelegate
#warning Thread Safety
// Note that any callback that provides streamed audio data (like streaming microphone input) happens on a separate audio thread that should not be blocked. When we feed audio data into any of the UI components we need to explicity create a GCD block on the main thread to properly get the UI to work.
-(void)microphone:(EZMicrophone *)microphone
 hasAudioReceived:(float **)buffer
   withBufferSize:(UInt32)bufferSize
withNumberOfChannels:(UInt32)numberOfChannels {
    // Getting audio data as an array of float buffer arrays. What does that mean? Because the audio is coming in as a stereo signal the data is split into a left and right channel. So buffer[0] corresponds to the float* data for the left channel while buffer[1] corresponds to the float* data for the right channel.
    
    // See the Thread Safety warning above, but in a nutshell these callbacks happen on a separate audio thread. We wrap any UI updating in a GCD block on the main thread to avoid blocking that audio flow.
/*    dispatch_async(dispatch_get_main_queue(),^{
        // All the audio plot needs is the buffer data (float*) and the size. Internally the audio plot will handle all the drawing related code, history management, and freeing its own resources. Hence, one badass line of code gets you a pretty plot :)
        [self.audioPlot updateBuffer:buffer[0] withBufferSize:bufferSize];
    });*/
}

// Append the microphone data coming as a AudioBufferList with the specified buffer size to the recorder
-(void)microphone:(EZMicrophone *)microphone
    hasBufferList:(AudioBufferList *)bufferList
   withBufferSize:(UInt32)bufferSize
withNumberOfChannels:(UInt32)numberOfChannels {
    
    // Getting audio data as a buffer list that can be directly fed into the EZRecorder. This is happening on the audio thread - any UI updating needs a GCD main queue block.
    if( self.isRecording ){
        [self.recorder appendDataFromBufferList:bufferList
                                 withBufferSize:bufferSize];
    }
    
}


@end

Recorder& Recorder::get_instance(){
    static Recorder ret;
    return ret;
}
Recorder::Recorder(){
    RecorderImpl* obj = [[[RecorderImpl alloc]init]retain];
    _recorder_impl = (void*)obj;
}
Recorder::~Recorder(){
    RecorderImpl* obj = (RecorderImpl*)_recorder_impl;
    [obj release];
}
void Recorder::start(std::string path){
    RecorderImpl* obj = (RecorderImpl*)_recorder_impl;
    [obj start:[NSString stringWithUTF8String:path.c_str()]];
}
void Recorder::stop(){
    RecorderImpl* obj = (RecorderImpl*)_recorder_impl;
    [obj stop];
}
