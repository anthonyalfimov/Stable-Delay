/*
  ==============================================================================

    RBDPresetManager.cpp
    Created: 19 Jan 2021 5:43:10pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDPresetManager.h"

PresetManager::PresetManager (AudioProcessor& processor)
    : mProcessor (processor)
{
    const auto pluginName = mProcessor.getName();
    
    // TODO: Swap preset location to a more sensible directory
    const auto parentDirectory = (File::getSpecialLocation (File::userDesktopDirectory))
                                  .getFullPathName();
    mPresetDirectory = File::addTrailingSeparator (parentDirectory) + pluginName;
    
    if (! File (mPresetDirectory).exists())
        File (mPresetDirectory).createDirectory();
    
    storeLocalPresets();
    
    // FIXME: preset selection is not recalled as a part of plugin's state
}

PresetManager::~PresetManager()
{
    
}

void PresetManager::createXmlForPreset (XmlElement* outElement)
{
    // Note: we can't access mProcessor->parameters here because mProcessor is
    //  a base class pointer and current scope is not aware of
    //  ReallyBasicDelayAudioProcessor. We'd need to include "Processor.h" to
    //  do that. Why not?
    
    // TODO: Why not access AudioProcessorValueTreeState directly here?
    //  All our Interface Panels use a ptr/reference to our
    //  ReallyBasicDelayAudioProcessor object, not a base-class pointer

    auto& parameters = mProcessor.getParameters();
    
    for (auto* parameter : parameters)
    {
        // Downcast parameter pointer to AudioProcessorParameterWithID ptr
        //  to access its ID
        auto* parameterWithID = dynamic_cast<AudioProcessorParameterWithID*> (parameter);
        
        // Handle failed downcast
        if (parameterWithID == nullptr)
            break;

        // MARK: getValue() returns a 0to1 range
        outElement->setAttribute (parameterWithID->paramID,
                                  parameterWithID->getValue());
    }
}

/**
 Parsing parameter names rather than just processing XML element in order should ensure
 better reliability and backwards compatibility
*/

void PresetManager::loadPresetFromXml (XmlElement* inElement)
{
    // TODO: Why store current preset XML in the manager object?
    //  Usually, these are leftovers from what was planned for the course and
    //  they have a reason behind them. So, before getting rid of it, let's try
    //  to figure out what was it intended for

    //  Maybe this is to avoid creating a new XML if the current preset hasn't
    //  changed, and just return the already existing one?
    
    mCurrentPresetXml = inElement;
    
    auto& parameters = mProcessor.getParameters();
    const int numAttributes = mCurrentPresetXml->getNumAttributes();
    
    for (int attr = 0; attr < numAttributes; ++attr)
    {
        const String name = mCurrentPresetXml->getAttributeName (attr);
        const float value = mCurrentPresetXml->getDoubleAttribute (name);
        
        for (auto* parameter : parameters)
        {
        // Downcast parameter ptr to AudioProcessorParameterWithID to access ID
            auto* parameterWithID
            = dynamic_cast<AudioProcessorParameterWithID*> (parameter);
            
            // Handle failed downcast
            if (parameterWithID == nullptr)
                break;

            // MARK: setValueNotifyingHost() expects 0to1 range
            if (name == parameterWithID->paramID)
                parameterWithID->setValueNotifyingHost (value);
        }
    }
}

int PresetManager::getNumberOfPresets() const
{
    return mLocalPresets.size();
}

String PresetManager::getPresetName (int inPresetIndex) const
{
    return mLocalPresets[inPresetIndex].getFileNameWithoutExtension();
}

void PresetManager::createNewPreset()
{
    auto& parameters = mProcessor.getParameters();
    
    // Reset all parameters to default values
    for (auto* parameter : parameters)
        parameter->setValueNotifyingHost (parameter->getDefaultValue());
    
    mIsCurrentPresetSaved = false;
    mCurrentPresetName = RBD::untitledPresetName;
}

void PresetManager::savePreset()
{
    /** Should only be called when the preset was already saved to disk via Save As */
    if (! mIsCurrentPresetSaved)
        jassertfalse;
    
    MemoryBlock destinationData;
    mProcessor.getStateInformation (destinationData);
    
    mCurrentlyLoadedPreset.replaceWithData (destinationData.getData(),
                                            destinationData.getSize());
}

void PresetManager::saveAsPreset (String inPresetName)
{
    File presetFile (File::addTrailingSeparator (mPresetDirectory)
                     + inPresetName + RBD::presetFileExtention);
    
    MemoryBlock destinationData;
    mProcessor.getStateInformation (destinationData);
    
    presetFile.replaceWithData (destinationData.getData(), destinationData.getSize());

    // TODO: More robust design for updating the loaded preset
    //  Note: Technically, we don't need to update mCurrentlyLoadedPreset here.
    //  This method is currently only called from TopPanel::displaySaveAsPopup()
    //  That method also calls an update on the preset list, which in turn
    //  updates the comboBox selection to the newly created preset. Updating the
    //  comboBox selection calls PresetManager::loadPreset(), and that updates
    //  mCurrentlyLoadedPreset.
    //  However, is this a robust design? If we forget to update the preset
    //  list, loadPreset() will not be called.
    
    mIsCurrentPresetSaved = true;
    mCurrentPresetName = inPresetName;
    
    storeLocalPresets();
}

void PresetManager::loadPreset (int inPresetIndex)
{
    mCurrentlyLoadedPreset = mLocalPresets[inPresetIndex];
    
    MemoryBlock presetBinary;
    
    if (mCurrentlyLoadedPreset.loadFileAsData (presetBinary))
    {
        mIsCurrentPresetSaved = true;
        mCurrentPresetName = getPresetName (inPresetIndex);
        mProcessor.setStateInformation (presetBinary.getData(),
                                        static_cast<int> (presetBinary.getSize()));
    }
}

bool PresetManager::isCurrentPresetSaved() const
{
    return mIsCurrentPresetSaved;
}

String PresetManager::getCurrentPresetName() const
{
    return mCurrentPresetName;
}

void PresetManager::storeLocalPresets()
{
    // TODO: Should we use `clearQuick()` here to reduce allocations / deallocations?
    mLocalPresets.clear();
    
    // TODO: Is there a better way to handle file extentions?
    for (DirectoryEntry entry
            : RangedDirectoryIterator (File (mPresetDirectory),
                                       false,
                                       "*" + RBD::presetFileExtention,
                                       File::TypesOfFileToFind::findFiles))
    {
        mLocalPresets.add (entry.getFile());
    }
}
