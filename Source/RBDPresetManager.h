/*
  ==============================================================================

    RBDPresetManager.h
    Created: 19 Jan 2021 5:43:10pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// TODO: Make static class members - these belong as a part of PresetManager

namespace RBD
{
    const static String presetFileExtention = ".rbdf";
    const static String untitledPresetName = "Untitled";
}

// TODO: should this class be a singleton, or do we not care?

class PresetManager
{
public:
    PresetManager (AudioProcessor& processor);
    ~PresetManager();
    
    // TODO: better method names
    void getXmlForPreset (XmlElement* outElement);
    void loadPresetForXml (XmlElement* inElement);
    
    int getNumberOfPresets() const;
    String getPresetName (int inPresetIndex) const;
    
    void createNewPreset();
    void savePreset();
    void saveAsPreset (String inPresetName);
    void loadPreset (int inPresetIndex);
    bool isCurrentPresetSaved() const;
    String getCurrentPresetName() const;
    
private:
    void storeLocalPresets();
    
    bool mIsCurrentPresetSaved = false;     // is current preset file saved to disk?
    String mCurrentPresetName = RBD::untitledPresetName;
    File mCurrentlyLoadedPreset;
    
    String mPresetDirectory;
    Array<File> mLocalPresets;
    
    XmlElement* mCurrentPresetXml;  // TODO: What is the point of this member?
    AudioProcessor& mProcessor;
};
