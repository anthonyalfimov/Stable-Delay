/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDGainModule.h"
#include "RBDDelayModule.h"
#include "RBDDryWetModule.h"
#include "RBDMeterProbe.h"
#include "RBDPresetManager.h"

//==============================================================================
/**
*/
class ReallyBasicDelayAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ReallyBasicDelayAudioProcessor();
    ~ReallyBasicDelayAudioProcessor() override;

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
    MeterProbe* getInputMeterProbe (int channel) const;
    MeterProbe* getOutputMeterProbe (int channel) const;

    //==============================================================================
    // TODO: Should we return a reference instead here?
    PresetManager* getPresetManager() const { return mPresetManager.get(); }

    /**
     This class contains a ValueTree that is used to manage an AudioProcessor's entire
     state. It has its own internal class of parameter object that is linked to values
     within its ValueTree, and which are each identified by a string ID.
    */
    AudioProcessorValueTreeState parameters;

private:
    void initialiseDSP();
    void initialiseParameters();
    void updateParameters();
    
    // DSP Modules
    OwnedArray<GainModule> mInputGain;
    OwnedArray<MeterProbe> mInputMeterProbe;
    OwnedArray<DelayModule> mDelay;
    OwnedArray<DryWetModule> mDryWetMixer;
    OwnedArray<GainModule> mOutputGain;
    OwnedArray<MeterProbe> mOutputMeterProbe;
    
    // Parameter Values
    // TODO: Is there a better option than storing raw ptrs to parameter values?
    //  Lifetime of these ptrs is the same at the AudioProcessorValueTreeState that
    //  stores them, so there should be no problems
    std::atomic<float>* mInputGainValue = nullptr;
    std::atomic<float>* mDelayTimeValue = nullptr;
    std::atomic<float>* mDelayFeedbackValue = nullptr;
    std::atomic<float>* mDryWetValue = nullptr;
    std::atomic<float>* mFxTypeValue = nullptr;
    std::atomic<float>* mOutputGainValue = nullptr;
    std::atomic<float>* mModulationRateValue = nullptr;
    std::atomic<float>* mModulationDepthValue = nullptr;
    std::atomic<float>* mStereoSpreadValue = nullptr;
    
    std::unique_ptr<PresetManager> mPresetManager;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReallyBasicDelayAudioProcessor)
};
