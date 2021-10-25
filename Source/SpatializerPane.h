/*
 ==============================================================================
 
 SpatializerPane.h
 Created: 14 Jan 2021 2:14:47pm
 Author:  Julien Bloit
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "Utils.h"

//==============================================================================
/*
 */
class SpatializerPane  : public juce::Component
{
public:
    SpatializerPane();
    ~SpatializerPane() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** position of the source in meters, usable by spatializer */
    juce::Point<float> getSourceCoordinates();
    
    
private:
    
    juce::Point<float> sourcePosition {0.f, 0.f};
    
    
    // The distance in meters at the edge of screen coordinates
    constexpr static const float RANGE_METERS = 20.f;
    
    void mouseDrag(const juce::MouseEvent& event) override;
    
    juce::NormalisableRange<float> scaledRange;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpatializerPane)
};
