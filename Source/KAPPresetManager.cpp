/*
  ==============================================================================

    KAPPresetManager.cpp
    Created: 19 Jan 2021 5:43:10pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPPresetManager.h"

KAPPresetManager::KAPPresetManager (AudioProcessor* inProcessor)
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

KAPPresetManager::~KAPPresetManager()
{
    
}

void KAPPresetManager::getXmlForPreset (XmlElement* outElement)
{
    // TODO: Could we get a tree of parameters (getParameterTree()) instead of the flat list here?
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

void KAPPresetManager::loadPresetForXml (XmlElement* inElement)
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

int KAPPresetManager::getNumberOfPresets() const
{
    return mLocalPresets.size();
}

String KAPPresetManager::getPresetName (int inPresetIndex) const
{
    return mLocalPresets[inPresetIndex].getFileNameWithoutExtension();
}

void KAPPresetManager::createNewPreset()
{
    auto& parameters = mProcessor->getParameters();
    
    // Reset all parameters to default values
    for (auto* parameter : parameters)
        parameter->setValueNotifyingHost (parameter->getDefaultValue());
    
    mIsCurrentPresetSaved = false;
    mCurrentPresetName = KAP::untitledPresetName;
}

void KAPPresetManager::savePreset()
{
    /** Should only be called when the preset was already saved to disk via Save As */
    if (! mIsCurrentPresetSaved)
        jassertfalse;
    
    MemoryBlock destinationData;
    mProcessor->getStateInformation (destinationData);
    
    mCurrentlyLoadedPreset.replaceWithData (destinationData.getData(), destinationData.getSize());    
}

void KAPPresetManager::saveAsPreset (String inPresetName)
{
    File presetFile (File::addTrailingSeparator (mPresetDirectory) + inPresetName
                     + KAP::presetFileExtention);
    
    MemoryBlock destinationData;
    mProcessor->getStateInformation (destinationData);
    
    presetFile.replaceWithData (destinationData.getData(), destinationData.getSize());
    
    // TODO: Shouldn't mCurrentlyLoadedPreset be updated here?
    
    mIsCurrentPresetSaved = true;
    mCurrentPresetName = inPresetName;
    
    storeLocalPresets();
}

void KAPPresetManager::loadPreset (int inPresetIndex)
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

bool KAPPresetManager::isCurrentPresetSaved() const
{
    return mIsCurrentPresetSaved;
}

String KAPPresetManager::getCurrentPresetName() const
{
    return mCurrentPresetName;
}

void KAPPresetManager::storeLocalPresets()
{
    // TODO: Should we use `clearQuick()` here to reduce allocations / deallocations?
    mLocalPresets.clear();
    
    // TODO: Is there a better way to handle file extentions?
    for (DirectoryEntry entry : RangedDirectoryIterator (File (mPresetDirectory),
                                                         false,
                                                         "*" + KAP::presetFileExtention,
                                                         File::TypesOfFileToFind::findFiles))
    {
        mLocalPresets.add (entry.getFile());
    }
}
