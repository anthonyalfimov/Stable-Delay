/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "KAPGain.h"
#include "KAPDelay.h"
#include "KAPLfo.h"
#include "KAPPresetManager.h"

// TODO: PROJECT LEVEL - check what methods can be made const

//==============================================================================
/**
*/
class KadenzeAudioPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    KadenzeAudioPluginAudioProcessor();
    ~KadenzeAudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    float getInputMeterLevel (int inChannel) const;
    float getOutputMeterLevel (int inChannel) const;
    
    /**
     This class contains a ValueTree that is used to manage an AudioProcessor's entire state.
     It has its own internal class of parameter object that is linked to values within its ValueTree,
     and which are each identified by a string ID.
    */
    AudioProcessorValueTreeState parameters;
    
    // TODO: should we return a unique_ptr reference or something like that instead here?
    KAPPresetManager* getPresetManager() const { return mPresetManager.get(); }

private:
    // TODO: switch to British spelling
    void initializeDSP();
    
    // TODO: hardcoding stereo signal here - independent gain for left and right channels. Refactor!
    std::unique_ptr<KAPGain> mInputGain[2];
    std::unique_ptr<KAPGain> mOutputGain[2];
    std::unique_ptr<KAPLfo> mLfo[2];
    std::unique_ptr<KAPDelay> mDelay[2];
    
    std::unique_ptr<KAPPresetManager> mPresetManager;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzeAudioPluginAudioProcessor)
};
