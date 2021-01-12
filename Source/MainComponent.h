#pragma once

#include <JuceHeader.h>
#include <BinauralSpatializer/Core.h>
#include "3dti_ResourceManager/ILD/ILDCereal.h"
#include "3dti_ResourceManager/HRTF/HRTFCereal.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    
    Binaural::CCore   myCore;
    int HRTF_resamplingStep = 45;

    // the binaural listener
    shared_ptr<Binaural::CListener> listener;

    // an audio source
    shared_ptr<Binaural::CSingleSourceDSP> mySource;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
