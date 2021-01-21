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
}

KAPPresetManager::~KAPPresetManager()
{
    
}

void KAPPresetManager::getXmlForPreset (XmlElement* outElement)
{
    const int numParameters = mProcessor->getNumParameters();
    
    for (int param = 0; param < numParameters; ++param)
    {
        outElement->setAttribute (mProcessor->getParameterName (param),
                                  mProcessor->getParameter (param));
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
    
    const int numAttributes = mCurrentPresetXml->getNumAttributes();
    const int numParameters = mProcessor->getNumParameters();
    
    for (int attr = 0; attr < numAttributes; ++attr)
    {
        const String name = mCurrentPresetXml->getAttributeName (attr);
        const float value = mCurrentPresetXml->getDoubleAttribute (name);
        
        for (int param = 0; param < numParameters; ++param)
        {
            if (name == mProcessor->getParameterName (param))
                mProcessor->setParameterNotifyingHost (param, value);
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
    const int numParameters = mProcessor->getNumParameters();
    
    // Reset all parameters to default values
    for (int param = 0; param < numParameters; ++param)
        mProcessor->setParameterNotifyingHost (param, mProcessor->getParameterDefaultValue (param));
    
    mIsCurrentPresetSaved = true;
    mCurrentPresetName = KAP::untitledPresetName;
}

/** Should only be called when the preset was already created via Save As */
void KAPPresetManager::savePreset()
{
    MemoryBlock destinationData;
    mProcessor->getStateInformation (destinationData);
    
    mCurrentlyLoadedPreset.replaceWithData (destinationData.getData(), destinationData.getSize());
    
    mIsCurrentPresetSaved = true;
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
