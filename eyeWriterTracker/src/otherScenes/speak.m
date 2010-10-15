#include <Cocoa/Cocoa.h>
#include <AppKit/NSOpenGL.h>
#include "speak.h"

extern void speakMe(const char * message){
	
	NSString * myWords = [NSString stringWithUTF8String:message];
	
	NSSpeechSynthesizer* s = [[NSSpeechSynthesizer alloc] initWithVoice:@"com.apple.speech.synthesis.voice.Alex"];
	
	[s startSpeakingString:myWords];
	
	[s release];
}