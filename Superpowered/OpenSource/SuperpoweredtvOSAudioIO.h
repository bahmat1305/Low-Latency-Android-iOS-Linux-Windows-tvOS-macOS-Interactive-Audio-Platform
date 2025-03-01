#import <AVFoundation/AVFoundation.h>

@protocol SuperpoweredtvOSAudioIODelegate;

/// @brief You can have an audio processing callback in Objective-C or pure C. This is the pure C prototype.
/// @return Return false when you did no audio processing (silence).
/// @param clientData A custom pointer your callback receives.
/// @param output Output buffer.
/// @param numberOfFrames The number of frames requested.
/// @param samplerate The current sample rate in Hz.
/// @param hostTime A mach timestamp, indicates when this buffer of audio will be passed to the audio output.
typedef bool (*audioProcessingCallback_C) (void *clientData, float *output, unsigned int numberOfFrames, unsigned int samplerate, uint64_t hostTime);

/// @brief Handles all audio session, audio lifecycle (interruptions), output, buffer size and samplerate headaches.
/// @warning All methods and setters should be called on the main thread only!
@interface SuperpoweredtvOSAudioIO: NSObject {
    int preferredBufferSizeMs;
}

@property (nonatomic, assign) int preferredBufferSizeMs; ///< The preferred buffer size in milliseconds. Recommended: 12.

/// @brief Constructor.
/// @param delegate The object fully implementing the SuperpoweredIOSAudioIODelegate protocol. Not retained.
/// @param preferredBufferSize The initial value for preferredBufferSizeMs. 12 is good for every tvOS device (512 frames).
/// @param preferredMinimumSamplerate The preferred minimum sample rate. 44100 or 48000 are recommended for good sound quality.
/// @param audioSessionCategory The audio session category.
/// @param channels The number of channels in the audio processing callback.
- (id)initWithDelegate:(id<SuperpoweredtvOSAudioIODelegate>)delegate preferredBufferSize:(unsigned int)preferredBufferSize preferredMinimumSamplerate:(unsigned int)preferredMinimumSamplerate audioSessionCategory:(NSString *)audioSessionCategory channels:(int)channels;

/// @brief Starts audio I/O.
/// @return True if successful, false if failed.
- (bool)start;

/// @brief Stops audio I/O.
- (void)stop;

/// @brief Sets the audio processing callback to a C function, instead of the delegate's Objective-C method.
/// 99% of all audio apps work great with the Objective-C method, so you don't need to use this. Don't call this method after [start]!
/// @param callback The callback function.
/// @param clientdata Some custom pointer for the C processing callback. You can set it to NULL.
- (void)setProcessingCallback_C:(audioProcessingCallback_C)callback clientdata:(void *)clientdata;

- (void)reconfigureWithAudioSessionCategory:(NSString *)audioSessionCategory;

@end


/// @brief You must implement this protocol to use SuperpoweredtvOSAudioIO.
@protocol SuperpoweredtvOSAudioIODelegate

/// @brief The audio session may be interrupted by a phone call, etc. This method is called on the main thread when the interrupt starts.
@optional
- (void)interruptionStarted;

/// @brief The audio session may be interrupted by a phone call, etc. This method is called on the main thread when audio resumes.
@optional
- (void)interruptionEnded;

/// @brief Process audio here.
/// @return Return false for no audio output (silence).
/// @param output Output buffer.
/// @param numberOfFrames The number of frames requested.
/// @param samplerate The current sample rate in Hz.
/// @param hostTime A mach timestamp, indicates when this chunk of audio will be passed to the audio output.
/// @warning It's called on a high priority real-time audio thread, so please take care of blocking and processing time to prevent audio dropouts.
- (bool)audioProcessingCallback:(float *)output numberOfFrames:(unsigned int)numberOfFrames samplerate:(unsigned int)samplerate hostTime:(UInt64)hostTime;

@end
