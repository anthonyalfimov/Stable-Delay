/*
  ==============================================================================

    RBDPresetManager.cpp
    Created: 19 Jan 2021 5:43:10pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDPresetManager.h"

RBDPresetManager::RBDPresetManager (AudioProcessor* inProcessor)
    : mProcessor (inProcessor)
{
    const auto pluginName = mProcessor->getName();
    
    // TODO: Swap preset location to a more sensible directory
    const String parentDirectory = (File::getSpecialLocation (File::userDesktopDirectory))
                                   .getFullPathName();
    mPresetDirectory = File::addTrailingSeparator (parentDirectory) + pluginName;
    
    if (! File (mPresetDirectory).exists())
        File (mPresetDirectory).createDirectory();
    
    storeLocalPresets();
    
    // FIXME: preset name / selection is not recalled as a part of plugin's state by the Host
}

RBDPresetManager::~RBDPresetManager()
{
    
}

void RBDPresetManager::getXmlForPreset (XmlElement* outElement)
{
    // Note: we can't access mProcessor->parameters here because mProcessor is a base class pointer
    //       and current scope is not aware of ReallyBasicDelayAudioProcessor
    
    // TODO: In Meter we include PluginProcessor.h to have access to ReallyBasicDelayAudioProcessor
    // TODO: Why shouldn't we go the same here? 
    
    auto& parameters = mProcessor->getParameters();
    
    for (auto* parameter : parameters)
    {
        // Downcast parameter pointer to AudioProcessorParameterWithID ptr to access its ID
        auto* parameterWithID = dynamic_cast<AudioProcessorParameterWithID*> (parameter);
        
        // Handle failed downcast
        if (parameterWithID == nullptr)
            break;
        
        outElement->setAttribute (parameterWithID->paramID, parameterWithID->getValue());
    }
}

/**
 Parsing parameter names rather than just processing XML element in order should ensure
 better reliability and backwards compatibility
*/

void RBDPresetManager::loadPresetForXml (XmlElement* inElement)
{
    // TODO: Why store current preset XML in the manager object?
    mCurrentPresetXml = inElement;
    
    auto& parameters = mProcessor->getParameters();
    const int numAttributes = mCurrentPresetXml->getNumAttributes();
    
    for (int attr = 0; attr < numAttributes; ++attr)
    {
        const String name = mCurrentPresetXml->getAttributeName (attr);
        const float value = mCurrentPresetXml->getDoubleAttribute (name);
        
        for (auto* parameter : parameters)
        {
            // Downcast parameter pointer to AudioProcessorParameterWithID ptr to access its ID
            auto* parameterWithID = dynamic_cast<AudioProcessorParameterWithID*> (parameter);
            
            // Handle failed downcast
            if (parameterWithID == nullptr)
                break;
            
            if (name == parameterWithID->paramID)
                parameterWithID->setValueNotifyingHost (value);
        }
    }
}

int RBDPresetManager::getNumberOfPresets() const
{
    return mLocalPresets.size();
}

String RBDPresetManager::getPresetName (int inPresetIndex) const
{
    return mLocalPresets[inPresetIndex].getFileNameWithoutExtension();
}

void RBDPresetManager::createNewPreset()
{
    auto& parameters = mProcessor->getParameters();
    
    // Reset all parameters to default values
    for (auto* parameter : parameters)
        parameter->setValueNotifyingHost (parameter->getDefaultValue());
    
    mIsCurrentPresetSaved = false;
    mCurrentPresetName = RBD::untitledPresetName;
}

void RBDPresetManager::savePreset()
{
    /** Should only be called when the preset was already saved to disk via Save As */
    if (! mIsCurrentPresetSaved)
        jassertfalse;
    
    MemoryBlock destinationData;
    mProcessor->getStateInformation (destinationData);
    
    mCurrentlyLoadedPreset.replaceWithData (destinationData.getData(), destinationData.getSize());    
}

void RBDPresetManager::saveAsPreset (String inPresetName)
{
    File presetFile (File::addTrailingSeparator (mPresetDirectory) + inPresetName
                     + RBD::presetFileExtention);
    
    MemoryBlock destinationData;
    mProcessor->getStateInformation (destinationData);
    
    presetFile.replaceWithData (destinationData.getData(), destinationData.getSize());
    
    // TODO: Shouldn't mCurrentlyLoadedPreset be updated here?
    
    mIsCurrentPresetSaved = true;
    mCurrentPresetName = inPresetName;
    
    storeLocalPresets();
}

void RBDPresetManager::loadPreset (int inPresetIndex)
{
    mCurrentlyLoadedPreset = mLocalPresets[inPresetIndex];
    
    MemoryBlock presetBinary;
    
    if (mCurrentlyLoadedPreset.loadFileAsData (presetBinary))
    {
        mIsCurrentPresetSaved = true;
        mCurrentPresetName = getPresetName (inPresetIndex);
        mProcessor->setStateInformation (presetBinary.getData(),
                                         static_cast<int> (presetBinary.getSize()));
    }
}

bool RBDPresetManager::isCurrentPresetSaved() const
{
    return mIsCurrentPresetSaved;
}

String RBDPresetManager::getCurrentPresetName() const
{
    return mCurrentPresetName;
}

void RBDPresetManager::storeLocalPresets()
{
    // TODO: Should we use `clearQuick()` here to reduce allocations / deallocations?
    mLocalPresets.clear();
    
    // TODO: Is there a better way to handle file extentions?
    for (DirectoryEntry entry : RangedDirectoryIterator (File (mPresetDirectory),
                                                         false,
                                                         "*" + RBD::presetFileExtention,
                                                         File::TypesOfFileToFind::findFiles))
    {
        mLocalPresets.add (entry.getFile());
    }
}
