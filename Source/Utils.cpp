/*
 ==============================================================================
 
 Utils.cpp
 Created: 14 Jan 2021 11:21:59am
 Author:  Julien Bloit
 
 ==============================================================================
 */

#include "Utils.h"

void Utils::binaryDataToFile(juce::String sampleFileName, juce::File destFile)
{
    // find binary resource from filename
    const char* namedResourceForFile;
    for (int j = 0; j < BinaryData::namedResourceListSize; ++j)
    {
        if (sampleFileName.compare(BinaryData::getNamedResourceOriginalFilename(BinaryData::namedResourceList[j])) == 0)
        {
            
            namedResourceForFile = BinaryData::namedResourceList[j];
        }
    }
    // if this is hit, then the sample file was not found.
    jassert (namedResourceForFile != nullptr);
    
    int dataSizeInBytes = 0;
    const char* data = BinaryData::getNamedResource (namedResourceForFile, dataSizeInBytes);
    
    // if you hit this, the provided sample file name was probably not found
    // in the BinaryData.
    jassert (data != nullptr);
    destFile.replaceWithData (data, (size_t) dataSizeInBytes);
}
