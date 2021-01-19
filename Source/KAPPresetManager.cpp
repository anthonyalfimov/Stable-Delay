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
