/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "KAPLookAndFeel.h"

//==============================================================================
KadenzeAudioPluginAudioProcessorEditor::KadenzeAudioPluginAudioProcessorEditor (KadenzeAudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    
    mMainPanel = std::make_unique<KAPMainPanel> (&audioProcessor);
    addAndMakeVisible (mMainPanel.get());
}

KadenzeAudioPluginAudioProcessorEditor::~KadenzeAudioPluginAudioProcessorEditor()
{
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
