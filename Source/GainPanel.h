/*
  ==============================================================================

    GainPanel.h
    Created: 11 Jan 2021 4:33:22pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "InterfacePanel.h"
#include "Parameters.h"
#include "ParameterKnob.h"
#include "Meter.h"

class GainPanel  : public InterfacePanel
{
public:
    GainPanel (ReallyBasicDelayAudioProcessor& processor,
               Parameter::Index parameterIndex);
    
private:
    std::unique_ptr<ParameterKnob> mKnob;
    std::unique_ptr<Meter> mMeter;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainPanel)
};
