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
    setSize (RBD::mainPanelWidth, RBD::mainPanelHeight);
    
    mLookAndFeel = std::make_unique<PluginLookAndFeel> ();
    setLookAndFeel (mLookAndFeel.get());    // set Look And Feel for the editor and its children
    LookAndFeel::setDefaultLookAndFeel (mLookAndFeel.get());    // set default for all components
    
    mMainPanel = std::make_unique<MainPanel> (&audioProcessor);
    addAndMakeVisible (mMainPanel.get());
    
    mBackgroundImage = ImageCache::getFromMemory (BinaryData::kadenze_bg_png,
                                                  BinaryData::kadenze_bg_pngSize);
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
    g.setOpacity (1.0f);    // Make sure the image is drawn opaque
    g.drawImage (mBackgroundImage, getLocalBounds().toFloat());
}

void ReallyBasicDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
