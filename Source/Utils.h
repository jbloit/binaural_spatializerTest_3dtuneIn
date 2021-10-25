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

    // from https://stackoverflow.com/questions/5731863/mapping-a-numeric-range-onto-another
    static double round(double d)
    {
        return floor(d + 0.5);
    }
    
    static float scale(float input, float inputLow, float inputHigh, float outputLow, float outputHigh)
    {
        double slope = 1.0 * (outputHigh - outputLow) / (inputHigh - inputLow);
        return (outputLow + round(slope * (input - inputLow)));
    }
    
    
};
