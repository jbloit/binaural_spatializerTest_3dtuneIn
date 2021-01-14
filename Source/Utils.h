/*
  ==============================================================================

    Utils.h
    Created: 14 Jan 2021 11:21:59am
    Author:  Julien Bloit

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Utils
{
public:
    Utils(){};
    ~Utils(){};
    
    /** Write bundled binary data to a file.*/
    static void binaryDataToFile(juce::String name, juce::File destFile);

};
