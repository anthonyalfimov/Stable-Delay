/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzeAudioPluginAudioProcessorEditor::KadenzeAudioPluginAudioProcessorEditor (KadenzeAudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    
    mLookAndFeel = std::make_unique<KAPLookAndFeel> ();
    setLookAndFeel (mLookAndFeel.get());    // set Look And Feel for the editor and its children
    LookAndFeel::setDefaultLookAndFeel (mLookAndFeel.get());    // set default for all components
    
    mMainPanel = std::make_unique<KAPMainPanel> (&audioProcessor);
    addAndMakeVisible (mMainPanel.get());
}

KadenzeAudioPluginAudioProcessorEditor::~KadenzeAudioPluginAudioProcessorEditor()
{
    // Release the look and feel
    setLookAndFeel (nullptr);
    LookAndFeel::setDefaultLookAndFeel (nullptr);
}

//==============================================================================
void KadenzeAudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void KadenzeAudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
