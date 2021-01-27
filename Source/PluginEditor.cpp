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
    
    mBackgroundImage = ImageCache::getFromMemory (BinaryData::kadenze_bg_png,
                                                  BinaryData::kadenze_bg_pngSize);
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
    g.setOpacity (1.0f);    // Make sure the image is drawn opaque
    g.drawImage (mBackgroundImage, getLocalBounds().toFloat());
}

void KadenzeAudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
