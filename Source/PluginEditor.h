/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MainPanel.h"
#include "PluginLookAndFeel.h"

//==============================================================================
/**
*/
class ReallyBasicDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ReallyBasicDelayAudioProcessorEditor (ReallyBasicDelayAudioProcessor&);
    ~ReallyBasicDelayAudioProcessorEditor() override;

//==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<MainPanel> mMainPanel;
    std::unique_ptr<PluginLookAndFeel> mLookAndFeel;
  
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReallyBasicDelayAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReallyBasicDelayAudioProcessorEditor)
};
