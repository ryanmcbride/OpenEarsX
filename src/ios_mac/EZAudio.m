//
//  EZAudio.m
//  EZAudio
//
//  Created by Syed Haris Ali on 11/21/13.
//  Copyright (c) 2013 Syed Haris Ali. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#import "EZAudio.h"

@implementation EZAudio

#pragma mark - AudioStreamBasicDescription Utility
+(AudioStreamBasicDescription)AIFFFormatWithNumberOfChannels:(UInt32)channels
                                                  sampleRate:(float)sampleRate
{
    AudioStreamBasicDescription asbd;
    memset(&asbd, 0, sizeof(asbd));
    asbd.mFormatID          = kAudioFormatLinearPCM;
    asbd.mFormatFlags       = kAudioFormatFlagIsBigEndian|kAudioFormatFlagIsPacked|kAudioFormatFlagIsSignedInteger;
    asbd.mSampleRate        = sampleRate;
    asbd.mChannelsPerFrame  = channels;
    asbd.mBitsPerChannel    = 32;
    asbd.mBytesPerPacket    = (asbd.mBitsPerChannel / 8) * asbd.mChannelsPerFrame;
    asbd.mFramesPerPacket   = 1;
    asbd.mBytesPerFrame     = (asbd.mBitsPerChannel / 8) * asbd.mChannelsPerFrame;
    return asbd;
}

+(AudioStreamBasicDescription)iLBCFormatWithSampleRate:(float)sampleRate
{
    AudioStreamBasicDescription asbd;
    memset(&asbd, 0, sizeof(asbd));
    asbd.mFormatID          = kAudioFormatiLBC;
    asbd.mChannelsPerFrame  = 1;
    asbd.mSampleRate        = sampleRate;
    
    // Fill in the rest of the descriptions using the Audio Format API
    UInt32 propSize = sizeof(asbd);
    [EZAudio checkResult:AudioFormatGetProperty(kAudioFormatProperty_FormatInfo,
                                                0,
                                                NULL,
                                                &propSize,
                                                &asbd)
               operation:"Failed to fill out the rest of the m4a AudioStreamBasicDescription"];
    
    return asbd;
}

+(AudioStreamBasicDescription)M4AFormatWithNumberOfChannels:(UInt32)channels
                                                 sampleRate:(float)sampleRate
{
    AudioStreamBasicDescription asbd;
    memset(&asbd, 0, sizeof(asbd));
    asbd.mFormatID          = kAudioFormatMPEG4AAC;
    asbd.mChannelsPerFrame  = channels;
    asbd.mSampleRate        = sampleRate;
    
    // Fill in the rest of the descriptions using the Audio Format API
    UInt32 propSize = sizeof(asbd);
    [EZAudio checkResult:AudioFormatGetProperty(kAudioFormatProperty_FormatInfo,
                                                0,
                                                NULL,
                                                &propSize,
                                                &asbd)
               operation:"Failed to fill out the rest of the m4a AudioStreamBasicDescription"];
    
    return asbd;
}

+(AudioStreamBasicDescription)RawFormat
{
    AudioStreamBasicDescription asbd;
    UInt32 byteSize = sizeof(AudioUnitSampleType);
    asbd.mBitsPerChannel   = 16;
    asbd.mChannelsPerFrame = 1;
    asbd.mFramesPerPacket  = 1;
    asbd.mBytesPerFrame    = asbd.mBitsPerChannel / 8 * asbd.mChannelsPerFrame;
    asbd.mBytesPerPacket   = asbd.mBytesPerFrame * asbd.mFramesPerPacket;
    asbd.mFormatFlags      = kAudioFormatFlagIsPacked|kAudioFormatFlagIsSignedInteger;
    asbd.mFormatID         = kAudioFormatLinearPCM;
    asbd.mSampleRate       = 16000;
    return asbd;
}

+(AudioStreamBasicDescription)monoFloatFormatWithSampleRate:(float)sampleRate
{
    AudioStreamBasicDescription asbd;
    UInt32 byteSize = sizeof(AudioUnitSampleType);
    asbd.mBitsPerChannel   = 8 * byteSize;
    asbd.mBytesPerFrame    = byteSize;
    asbd.mBytesPerPacket   = byteSize;
    asbd.mChannelsPerFrame = 1;
    asbd.mFormatFlags      = kAudioFormatFlagIsPacked|kAudioFormatFlagIsFloat;
    asbd.mFormatID         = kAudioFormatLinearPCM;
    asbd.mFramesPerPacket  = 1;
    asbd.mSampleRate       = sampleRate;
    return asbd;
}

+(AudioStreamBasicDescription)monoCanonicalFormatWithSampleRate:(float)sampleRate
{
    AudioStreamBasicDescription asbd;
    UInt32 byteSize = sizeof(AudioUnitSampleType);
    asbd.mBitsPerChannel   = 8 * byteSize;
    asbd.mBytesPerFrame    = byteSize;
    asbd.mBytesPerPacket   = byteSize;
    asbd.mChannelsPerFrame = 1;
    asbd.mFormatFlags      = kAudioFormatFlagsCanonical|kAudioFormatFlagIsNonInterleaved;
    asbd.mFormatID         = kAudioFormatLinearPCM;
    asbd.mFramesPerPacket  = 1;
    asbd.mSampleRate       = sampleRate;
    return asbd;
}

+(AudioStreamBasicDescription)stereoCanonicalNonInterleavedFormatWithSampleRate:(float)sampleRate
{
    AudioStreamBasicDescription asbd;
    UInt32 byteSize = sizeof(AudioUnitSampleType);
    asbd.mBitsPerChannel   = 8 * byteSize;
    asbd.mBytesPerFrame    = byteSize;
    asbd.mBytesPerPacket   = byteSize;
    asbd.mChannelsPerFrame = 2;
    asbd.mFormatFlags      = kAudioFormatFlagsCanonical|kAudioFormatFlagIsNonInterleaved;
    asbd.mFormatID         = kAudioFormatLinearPCM;
    asbd.mFramesPerPacket  = 1;
    asbd.mSampleRate       = sampleRate;
    return asbd;
}

+(AudioStreamBasicDescription)stereoFloatInterleavedFormatWithSampleRate:(float)sampleRate
{
    AudioStreamBasicDescription asbd;
    UInt32 floatByteSize   = sizeof(float);
    asbd.mChannelsPerFrame = 2;
    asbd.mBitsPerChannel   = 8 * floatByteSize;
    asbd.mBytesPerFrame    = asbd.mChannelsPerFrame * floatByteSize;
    asbd.mBytesPerPacket   = asbd.mChannelsPerFrame * floatByteSize;
    asbd.mFormatFlags      = kAudioFormatFlagIsPacked|kAudioFormatFlagIsFloat;
    asbd.mFormatID         = kAudioFormatLinearPCM;
    asbd.mFramesPerPacket  = 1;
    asbd.mSampleRate       = sampleRate;
    return asbd;
}

+(AudioStreamBasicDescription)stereoFloatNonInterleavedFormatWithSampleRate:(float)sampleRate
{
    AudioStreamBasicDescription asbd;
    UInt32 floatByteSize   = sizeof(float);
    asbd.mBitsPerChannel   = 8 * floatByteSize;
    asbd.mBytesPerFrame    = floatByteSize;
    asbd.mBytesPerPacket   = floatByteSize;
    asbd.mChannelsPerFrame = 2;
    asbd.mFormatFlags      = kAudioFormatFlagIsFloat|kAudioFormatFlagIsNonInterleaved;
    asbd.mFormatID         = kAudioFormatLinearPCM;
    asbd.mFramesPerPacket  = 1;
    asbd.mSampleRate       = sampleRate;
    return asbd;
}


#pragma mark - OSStatus Utility
+(void)checkResult:(OSStatus)result
         operation:(const char *)operation {
	if (result == noErr) return;
	char errorString[20];
	// see if it appears to be a 4-char-code
	*(UInt32 *)(errorString + 1) = CFSwapInt32HostToBig(result);
	if (isprint(errorString[1]) && isprint(errorString[2]) && isprint(errorString[3]) && isprint(errorString[4])) {
		errorString[0] = errorString[5] = '\'';
		errorString[6] = '\0';
	} else
		// no, format it as an integer
		sprintf(errorString, "%d", (int)result);
	fprintf(stderr, "Error: %s (%s)\n", operation, errorString);
	exit(1);
}

@end