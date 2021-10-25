/*
  ==============================================================================

    SpatializerPane.cpp
    Created: 14 Jan 2021 2:14:47pm
    Author:  Julien Bloit

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SpatializerPane.h"

//==============================================================================
SpatializerPane::SpatializerPane()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SpatializerPane::~SpatializerPane()
{
}

void SpatializerPane::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.drawEllipse(getLocalBounds().toFloat(), 0.5);
    
    auto headEllipseRectBounds = getLocalBounds().reduced(proportionOfWidth(0.45));
    g.drawEllipse(headEllipseRectBounds.toFloat(), 0.5);
    
    g.fillEllipse(sourcePosition.getX(), sourcePosition.getY(), 10, 10);
}

void SpatializerPane::resized()
{
    scaledRange = juce::NormalisableRange<float>(0, getHeight() * 0.5f, 1.f, 3.5f);

}

juce::Point<float> SpatializerPane::getSourceCoordinates()
{
    float sourceX = Utils::scale(sourcePosition.getX(), 0, getWidth(), RANGE_METERS, -RANGE_METERS);
    float sourceY = Utils::scale(sourcePosition.getY(), 0, getHeight(), RANGE_METERS, -RANGE_METERS);
    return juce::Point<float>(sourceX, sourceY);
}

void SpatializerPane::mouseDrag(const juce::MouseEvent& e)
{
    if (getLocalBounds().contains(e.position.toInt()))
    {
        sourcePosition = e.position;
        
        DBG("dragged to point X "
            + juce::String(sourcePosition.getX())
            + " Y: "
            + juce::String(sourcePosition.getY()));

        repaint();
    }
}
