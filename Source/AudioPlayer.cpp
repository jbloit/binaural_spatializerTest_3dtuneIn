#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
    formatManager.registerBasicFormats();

    readAudioFile(buffer, BinaryData::keysRattling_aif, BinaryData::keysRattling_aifSize);
    
};
AudioPlayer::~AudioPlayer(){};

void AudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{}

void AudioPlayer::releaseResources()
{
    buffer.setSize (0, 0);
}

void AudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    

    auto numInputChannels = buffer.getNumChannels();
    auto numOutputChannels = bufferToFill.buffer->getNumChannels();

    auto outputSamplesRemaining = bufferToFill.numSamples;
    auto outputSamplesOffset = bufferToFill.startSample;

    while (outputSamplesRemaining > 0)
    {
        auto bufferSamplesRemaining = buffer.getNumSamples() - position;
        auto samplesThisTime = juce::jmin (outputSamplesRemaining, bufferSamplesRemaining);

        for (auto channel = 0; channel < numOutputChannels; ++channel)
        {
            bufferToFill.buffer->copyFrom (channel,
                                           outputSamplesOffset,
                                           buffer,
                                           channel % numInputChannels,
                                           position,
                                           samplesThisTime);


        }

        outputSamplesRemaining -= samplesThisTime;
        outputSamplesOffset += samplesThisTime;
        position += samplesThisTime;

        if (position == buffer.getNumSamples())
            position = 0;
    }

    
}

void AudioPlayer::readAudioFile(juce::AudioSampleBuffer& buffer, const void* data, size_t sizeBytes)
{

    juce::AiffAudioFormat aifFormat;
    
    std::unique_ptr<juce::AudioFormatReader> reader (aifFormat.createReaderFor (new juce::MemoryInputStream (data, sizeBytes, false), true));
    if (reader.get() != nullptr)
    {
        buffer.setSize(reader->numChannels, reader->lengthInSamples);
        reader->read(&buffer, 0, reader->lengthInSamples, 0, true, true);
    }
    // reader is automatically deleted by using unique_ptr
}

