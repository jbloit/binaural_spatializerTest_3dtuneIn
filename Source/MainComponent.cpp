#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
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
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    myCore.SetAudioState({static_cast<int>(sampleRate), samplesPerBlockExpected});
    myCore.SetHRTFResamplingStep(HRTF_resamplingStep);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    
    // For more details, see the help for AudioProcessor::getNextAudioBlock()
    
    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
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
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}


void MainComponent::timerCallback()
{
    
    Common::CTransform newSourceTrf;
    newSourceTrf.SetPosition(Common::CVector3(0.2,0.3,0.0));    //Move source to absolute position

    {
        const juce::ScopedLock sl (lock);
        mySource->SetSourceTransform(newSourceTrf);
    }
    
    // 3. Set the transformation (position & orientation) for the listener, for instance:
//    Common::CTransform listenerTrf;
//    listernerTrf.SetOrientation(CQuaternion(qw, qx, qy, qz));
//    {
//        const juce::ScopedLock sl (lock);
//        listener->SetListenerTransform(listenerTrf);
//    }
}
