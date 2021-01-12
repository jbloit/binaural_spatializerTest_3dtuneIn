#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{

    addAndMakeVisible(sliderX);
    sliderX.setRange(-20.0, 20.0);
    sliderX.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderX.addListener(this);
    
    addAndMakeVisible(sliderY);
    sliderY.setRange(-20.0, 20.0);
    sliderY.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderY.addListener(this);
    
    addAndMakeVisible(sliderZ);
    sliderZ.setRange(-20.0, 20.0);
    sliderZ.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderZ.addListener(this);
    
    
    setSize (800, 600);
    
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    // binaural setup
    listener = myCore.CreateListener();
    mySource = myCore.CreateSingleSourceDSP();
    mySource->SetSpatializationMode(Binaural::TSpatializationMode::HighQuality);
    
    // Load HRTF file, from a SOFA or 3DTI file, into the CHRTF head of the listener.
    juce::String hrtf3DTIFile_PATH = "/Users/jbloit/repos/libs/3dti_AudioToolkit/resources/HRTF/3DTI/3DTI_HRTF_IRC1053_512s_44100Hz.3dti-hrtf";
    
    bool result = HRTF::CreateFrom3dti(hrtf3DTIFile_PATH.toStdString(), listener);
    if (result) {
        cout<< "HRTF has been loaded successfully";
    }
    else {
        cout<< "HRTF has NOT been loaded successfully";
    }
    
    // Load ILD for Near Field effect from 3DTI file.
    juce::String fileILDNearFieldEffect = "/Users/jbloit/repos/libs/3dti_AudioToolkit/resources/ILD/NearFieldCompensation_ILD_44100.3dti-ild";
    result = ILD::CreateFrom3dti_ILDNearFieldEffectTable(fileILDNearFieldEffect.toStdString(), listener);
    if (result) { cout<< "ILD Near Field Effect simulation file has been loaded successfully";}
    else {
        cout<< "ILD Near Field Effect simulation file has NOT been loaded successfully";
    }
    
    startTimer(20);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    audioPlayer.releaseResources();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    myCore.SetAudioState({static_cast<int>(sampleRate), samplesPerBlockExpected});
    myCore.SetHRTFResamplingStep(HRTF_resamplingStep);
    
    audioPlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    
    audioPlayer.getNextAudioBlock(bufferToFill);

//    auto rmsL = bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.numSamples);
//    auto rmsR = bufferToFill.buffer->getRMSLevel(1, 0, bufferToFill.numSamples);
//    DBG("rmsL " << juce::String(rmsL));
//    DBG("rmsR " << juce::String(rmsR));
    
    
//    bufferToFill.clearActiveBufferRegion();
    
      // 1. Declare anechoic output mix, which consists of a pair of mono buffers (one for each channel/ear)
  Common::CEarPair<CMonoBuffer<float>> bAnechoicOutput;
  bAnechoicOutput.left.resize(bufferToFill.numSamples);
  bAnechoicOutput.right.resize(bufferToFill.numSamples);

  // 2. Iterate through sources

    // 3. Get input chunk for this source
    // Your audio framework should provide you with the necessary methods to obtain the chunk
    CMonoBuffer< float > bInput(bufferToFill.numSamples);

    bInput.Feed( bufferToFill.buffer->getWritePointer(0), bufferToFill.numSamples, 1);
    
    
    // 4. Declare output for this source. Core assumes output is a pair of mono buffers (one for each channel/ear)
    Common::CEarPair<CMonoBuffer<float>> singleSourceAnechoicOut;

    // 5. Spatialise this source, updating the input buffer and passing the output buffer
    {
        const juce::ScopedLock sl (lock);
        mySource->SetBuffer(bInput);
          mySource->ProcessAnechoic(singleSourceAnechoicOut.left, singleSourceAnechoicOut.right);
    }
    
    // 6. Add this source output to the anechoic output mix
    bAnechoicOutput.left += singleSourceAnechoicOut.left;
    bAnechoicOutput.right += singleSourceAnechoicOut.right;
  
    for (int chan = 0; chan < 2; chan++)
    {
        for (int i=0; i < bufferToFill.numSamples; i++)
        {
            //QUICK AND DIRTY I KNOW
            if (chan == 0)
                bufferToFill.buffer->getWritePointer(chan)[i]  = bAnechoicOutput.left[i];
            else
                bufferToFill.buffer->getWritePointer(chan)[i]  = bAnechoicOutput.right[i];
        }
    }
    
     

}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    
    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    
    sliderX.setBounds(area.removeFromLeft(proportionOfWidth(0.3)));
    sliderY.setBounds(area.removeFromLeft(proportionOfWidth(0.3)));
    sliderZ.setBounds(area);
}


void MainComponent::timerCallback()
{
    
    Common::CTransform newSourceTrf;
    newSourceTrf.SetPosition(Common::CVector3(sliderX.getValueObject().getValue(),
                                              sliderY.getValueObject().getValue(), sliderZ.getValueObject().getValue()));    //Move source to absolute position

    {
        const juce::ScopedLock sl (lock);
        mySource->SetSourceTransform(newSourceTrf);
    }
    
    // 3. Set the transformation (position & orientation) for the listener, for instance:
    Common::CTransform listenerTrf;
//    listenerTrf.SetOrientation(Common::CQuaternion(0, 0, 0, 0));
    listenerTrf.SetPosition(Common::CVector3(0,0,0));
    {
        const juce::ScopedLock sl (lock);
        listener->SetListenerTransform(listenerTrf);
    }
}


void MainComponent::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &sliderX)
    {
        auto val = sliderX.getValueObject().getValue();
        DBG("slider X value " << val.toString());
        return;
    }
    
    if (slider == &sliderY)
    {
        auto val = sliderY.getValueObject().getValue();
        DBG("slider Y value " << val.toString());
        return;
    }
    
    if (slider == &sliderZ)
    {
        auto val = sliderZ.getValueObject().getValue();
        DBG("slider Z value " << val.toString());
        return;
    }
}
