/*
  ==============================================================================

    RBDPresetManager.h
    Created: 19 Jan 2021 5:43:10pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// TODO: can we move these to the .cpp file? Or does it need to be globally available if needed?
namespace RBD
{
    // TODO: Should we use StringRef instead here?
    const static String presetFileExtention = ".kpf";
    const static String untitledPresetName = "Untitled";
}

// TODO: should this class be a singleton, or do we not care?

class PresetManager
{
public:
    PresetManager (AudioProcessor* inProcessor);
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
    
    XmlElement* mCurrentPresetXml;          // TODO: What is the point of this member?
    AudioProcessor* mProcessor;
};