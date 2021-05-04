/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReallyBasicDelayAudioProcessorEditor::ReallyBasicDelayAudioProcessorEditor (ReallyBasicDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set up Editor attributes
    setSize (RBD::mainPanelWidth, RBD::mainPanelHeight);

    // Set up Look And Feel
    mLookAndFeel = std::make_unique<PluginLookAndFeel> ();
    //  Set Look And Feel for the editor and its children
    setLookAndFeel (mLookAndFeel.get());
    //  Set Look And Feel for all components
    LookAndFeel::setDefaultLookAndFeel (mLookAndFeel.get());

    // Set up Main Panel
    mMainPanel = std::make_unique<MainPanel> (audioProcessor);
    addAndMakeVisible (mMainPanel.get());
}

ReallyBasicDelayAudioProcessorEditor::~ReallyBasicDelayAudioProcessorEditor()
{
    // Release the look and feel
    setLookAndFeel (nullptr);
    LookAndFeel::setDefaultLookAndFeel (nullptr);
}

//==============================================================================
void ReallyBasicDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // TODO: Consider adding fitting background fill
    //  Reaper can forcefully resize the editor, revealing the bg colour.
    //  It would be neat to set the bg colour to something that matches our
    //  bg image.
}

void ReallyBasicDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
