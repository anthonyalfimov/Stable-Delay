/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RBDMainPanel.h"
#include "RBDLookAndFeel.h"

//==============================================================================
/**
*/
class ReallyBasicDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ReallyBasicDelayAudioProcessorEditor (ReallyBasicDelayAudioProcessor&);
    ~ReallyBasicDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    std::unique_ptr<RBDMainPanel> mMainPanel;
    std::unique_ptr<RBDLookAndFeel> mLookAndFeel;
    
    // TODO: why are we drawing it here and not in the MainPanel?
    Image mBackgroundImage;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReallyBasicDelayAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReallyBasicDelayAudioProcessorEditor)
};
