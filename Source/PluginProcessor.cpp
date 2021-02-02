/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "KAPParameters.h"

// TODO: Is there a better location for this function? Parameter.h, perhaps?
//==============================================================================
// This creates the plugin parameter layout
AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    // TODO: NormalisableRange takes care of parameter range mapping, why map in the processor?
    // TODO: Implement labels using JUCE's Label class
    // TODO: (?) Change the type of the Delay Type parameter to eliminate the need for casts
    
    AudioProcessorValueTreeState::ParameterLayout layout;

    for (int i = 0; i < kParameter_TotalNumParameters; ++i)
    {
        layout.add (std::make_unique<AudioParameterFloat> (KAPParameterID[i],
                                                           KAPParameterLabel[i],
                                                           NormalisableRange<float> (0.0f, 1.0f),
                                                           KAPParameterDefaultValue[i]));
    }
    
    return layout;
}

//==============================================================================
KadenzeAudioPluginAudioProcessor::KadenzeAudioPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    // TODO: initialising inside #ifndef - source of potential problems?
    parameters (*this,                      // reference to processor
                nullptr,                    // null pointer to undoManager
                Identifier ("KAP"),         // valueTree indentifier
                createParameterLayout())    // parameter layout
                
#endif
{
    // Initialise DSP modules
    initializeDSP();
    // Create the preset manager
    mPresetManager = std::make_unique<KAPPresetManager> (this);
}

KadenzeAudioPluginAudioProcessor::~KadenzeAudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String KadenzeAudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KadenzeAudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeAudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeAudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KadenzeAudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KadenzeAudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KadenzeAudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KadenzeAudioPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KadenzeAudioPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void KadenzeAudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KadenzeAudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (int channel = 0; channel < 2; ++channel)
    {
        mLfo[channel]->setSampleRate (sampleRate);
        mDelay[channel]->setSampleRate (sampleRate);
    }
}

void KadenzeAudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    for (int channel = 0; channel < 2; ++channel)
    {
        mLfo[channel]->reset();
        mDelay[channel]->reset();
    }
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KadenzeAudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KadenzeAudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        
        // TODO: Using the same buffer to read and write - is there a better approach?
        // TODO: Stereo processing hardcoded. This will fail for >2 channels, will it for mono?
        /* Q: Why can't we use one KAP::Gain object for both channels?
           A: If we have parameter smoothing, it must remain continuos between blocks. DSP module
              acquires memory in this case, and this memory needs to be per-channel
        */
        // NB: Calling this when mGain[] unique_ptrs are not initialised is UB!
        // TODO: add smoothing for all parameters
        // TODO: is this the proper way of using modern JUCE parameter handling?
        // TODO: consider using std::atomic<float> for .process() functions of DSP modules!
        float inputGain = *(parameters.getRawParameterValue (KAPParameterID[kParameter_InputGain]));
        mInputGain[channel]->process (channelData,              // inAudio
                                      inputGain,                // inGain
                                      channelData,              // outAudio
                                      buffer.getNumSamples());  // inNumSamplesToRender
        
        float modulationRate = *(parameters
                                 .getRawParameterValue (KAPParameterID[kParameter_ModulationRate]));
        if (channel == 0)
            modulationRate = 0.0f;
        
        float modulationDepth = *(parameters
                                  .getRawParameterValue (KAPParameterID[kParameter_ModulationDepth]));
        
        mLfo[channel]->process (modulationRate,             // inRate
                                modulationDepth,            // inDepth
                                buffer.getNumSamples());    // inNumSamplesToRender
        
        float delayTime = *(parameters.getRawParameterValue (KAPParameterID[kParameter_DelayTime]));
        float delayFeedback = *(parameters
                                .getRawParameterValue (KAPParameterID[kParameter_DelayFeedback]));
        float delayWetDry = *(parameters
                              .getRawParameterValue (KAPParameterID[kParameter_DelayWetDry]));
        float delayType = *(parameters.getRawParameterValue (KAPParameterID[kParameter_DelayTime]));
        
        mDelay[channel]->process (channelData,                  // inAudio
                                  delayTime,                    // inTime
                                  delayFeedback,                // inFeedback
                                  delayWetDry,                  // inWetDry
                                  delayType,                    // inType
                                  mLfo[channel]->getBuffer(),   // inModulationBuffer
                                  channelData,                  // outAudio
                                  buffer.getNumSamples());      // inNumSamplesToRender
        
        float outputGain = *(parameters.getRawParameterValue (KAPParameterID[kParameter_OutputGain]));
        
        mOutputGain[channel]->process (channelData,             // inAudio
                                       outputGain,              // inGain
                                       channelData,             // outAudio
                                       buffer.getNumSamples()); // inNumSamplesToRender
    }
}

//==============================================================================
bool KadenzeAudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* KadenzeAudioPluginAudioProcessor::createEditor()
{
    return new KadenzeAudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void KadenzeAudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // TODO: Why do we need a separate child for the preset body?
    //       Can't we just use one XML element on the stack?
    
    XmlElement preset ("KAP_StateInfo");
    
    auto presetBody = std::make_unique<XmlElement> ("KAP_Preset");
    mPresetManager->getXmlForPreset (presetBody.get());
    
    preset.addChildElement (presetBody.release());  // this transfers the ownership of `presetBody`
    copyXmlToBinary (preset, destData);
}

void KadenzeAudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    std::unique_ptr<XmlElement> xmlState = getXmlFromBinary (data, sizeInBytes);
    
    if (xmlState.get() != nullptr)
    {
        forEachXmlChildElement (*xmlState, subChild)
        {
            mPresetManager->loadPresetForXml (subChild);
        }
    }
    else
    {
    // TODO: nullptr can be returned if data is corrupted, so it should send an error in prod code
        jassertfalse;
    }
}

float KadenzeAudioPluginAudioProcessor::getInputMeterLevel (int inChannel) const
{
    return mInputGain[inChannel]->getMeterLevel();
}

float KadenzeAudioPluginAudioProcessor::getOutputMeterLevel (int inChannel) const
{
    return mOutputGain[inChannel]->getMeterLevel();
}

void KadenzeAudioPluginAudioProcessor::initializeDSP()
{
    // Initialise the DSP Gain modules
    // TODO: hardcoding stereo processing here. Refactor!
    for (int channel = 0; channel < 2; ++channel)
    {
        mInputGain[channel] = std::make_unique<KAPGain>();
        mOutputGain[channel] = std::make_unique<KAPGain>();
        mLfo[channel] = std::make_unique<KAPLfo>();
        mDelay[channel] = std::make_unique<KAPDelay>();
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KadenzeAudioPluginAudioProcessor();
}
