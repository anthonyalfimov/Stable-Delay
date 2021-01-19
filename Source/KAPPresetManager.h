/*
  ==============================================================================

    KAPPresetManager.h
    Created: 19 Jan 2021 5:43:10pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// TODO: should this class be a singleton, or do we not care?

class KAPPresetManager
{
public:
    KAPPresetManager (AudioProcessor* inProcessor);
    ~KAPPresetManager();
    
    // TODO: better method names
    void getXmlForPreset (XmlElement* outElement);
    void loadPresetForXml (XmlElement* inElement);
    
private:
    AudioProcessor* mProcessor;
    XmlElement* mCurrentPresetXml;
};
