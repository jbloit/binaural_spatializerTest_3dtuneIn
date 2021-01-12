/*
 ==============================================================================
 
 AudioPlayer.h
 Created: 12 Jan 2021 3:58:08pm
 Author:  Julien Bloit
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
class AudioPlayer : public juce::AudioSource
{
public:
    AudioPlayer();
    ~AudioPlayer();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    
private:
    
    void readAudioFile(juce::AudioSampleBuffer& buffer, const void* data, size_t sizeBytes);
    juce::AudioFormatManager formatManager;
    
    juce::AudioSampleBuffer buffer;
    int position = 0;
};
