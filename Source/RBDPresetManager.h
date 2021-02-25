/*
  ==============================================================================

    RBDPresetManager.h
    Created: 19 Jan 2021 5:43:10pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace RBD
{
    const static String presetFileExtention = ".rbdf";
    const static String untitledPresetName = "Untitled";
}

// NB: We cannot store a reference to ReallyBasicDelayAudioProcessor here,
//  because that would cause circular inclusion.

// TODO: Consider storing a reference to AudioProcessorValueTreeState instead
//  Processor reference is used to:
//  - get plugin name (there might be alternatives?)
//  - get and set plugin state information to save and load presets
//  - access parameters
//  We could include both AudioProcessor& and the AudioProcessorValueTreeState&,
//  but is it worth it?

// TODO: Should this class be a singleton, or do we not care?

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
