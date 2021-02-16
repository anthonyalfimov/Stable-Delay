/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "RBDParameters.h"

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
        layout.add (std::make_unique<AudioParameterFloat> (RBDParameterID[i],
                                                           RBDParameterLabel[i],
                                                           NormalisableRange<float> (0.0f, 1.0f),
                                                           RBDParameterDefaultValue[i]));
    }
    
    return layout;
}

//==============================================================================
ReallyBasicDelayAudioProcessor::ReallyBasicDelayAudioProcessor()
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
                Identifier ("RBD"),         // valueTree indentifier
                createParameterLayout())    // parameter layout
                
#endif
{
    // Initialise DSP modules
    initialiseDSP();
    // Create the preset manager
    mPresetManager = std::make_unique<PresetManager> (this);
}

ReallyBasicDelayAudioProcessor::~ReallyBasicDelayAudioProcessor()
{
}

//==============================================================================
const juce::String ReallyBasicDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReallyBasicDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReallyBasicDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReallyBasicDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReallyBasicDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReallyBasicDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReallyBasicDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReallyBasicDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReallyBasicDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReallyBasicDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReallyBasicDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (int channel = 0; channel < 2; ++channel)
    {
        mLfo[channel]->setSampleRateAndBlockSize (sampleRate, samplesPerBlock);
        mDelay[channel]->setSampleRate (sampleRate);
    }
}

void ReallyBasicDelayAudioProcessor::releaseResources()
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
bool ReallyBasicDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (/*layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     &&*/ layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void ReallyBasicDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Denormals are temporarily disabled when this object is created at the beginning of
    // the process block and re-enabled when it's destroyed at the end of the process
    // block. Therefore, anything that happens within the process block doesn't need
    // to disable denormals - they won't be re-enabled until the end of the process
    // block.
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
        /* Q: Why can't we use one RBD::Gain object for both channels?
           A: If we have parameter smoothing, it must remain continuos between blocks. DSP module
              acquires memory in this case, and this memory needs to be per-channel
        */
        // NB: Calling this when mGain[] unique_ptrs are not initialised is UB!
        // TODO: add smoothing for all parameters
        // TODO: is this the proper way of using modern JUCE parameter handling?
        // TODO: consider using std::atomic<float> for .process() functions of DSP modules!
        float inputGain = *(parameters.getRawParameterValue (RBDParameterID[kParameter_InputGain]));
        mInputGain[channel]->process (channelData,              // inAudio
                                      inputGain,                // inGain
                                      channelData,              // outAudio
                                      buffer.getNumSamples());  // inNumSamplesToRender
        
        float modulationRate
            = *(parameters.getRawParameterValue (RBDParameterID[kParameter_ModulationRate]));
        
        if (channel == 0)
            modulationRate = 0.0f;
        
        float modulationDepth
            = *(parameters.getRawParameterValue (RBDParameterID[kParameter_ModulationDepth]));
        
        mLfo[channel]->process (modulationRate,             // inRate
                                modulationDepth,            // inDepth
                                buffer.getNumSamples());    // inNumSamplesToRender
        
        float delayTime = *(parameters.getRawParameterValue (RBDParameterID[kParameter_DelayTime]));
        float delayFeedback
            = *(parameters.getRawParameterValue (RBDParameterID[kParameter_DelayFeedback]));
        float delayWetDry
            = *(parameters.getRawParameterValue (RBDParameterID[kParameter_DelayWetDry]));
        float delayType = *(parameters.getRawParameterValue (RBDParameterID[kParameter_DelayType]));
        
        mDelay[channel]->process (channelData,                  // inAudio
                                  delayTime,                    // inTime
                                  delayFeedback,                // inFeedback
                                  delayWetDry,                  // inWetDry
                                  delayType,                    // inType
                                  mLfo[channel]->getBuffer(),   // inModulationBuffer
                                  channelData,                  // outAudio
                                  buffer.getNumSamples());      // inNumSamplesToRender
        
        float outputGain
            = *(parameters.getRawParameterValue (RBDParameterID[kParameter_OutputGain]));
        
        mOutputGain[channel]->process (channelData,             // inAudio
                                       outputGain,              // inGain
                                       channelData,             // outAudio
                                       buffer.getNumSamples()); // inNumSamplesToRender
    }
}

//==============================================================================
bool ReallyBasicDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReallyBasicDelayAudioProcessor::createEditor()
{
    return new ReallyBasicDelayAudioProcessorEditor (*this);
}

//==============================================================================
void ReallyBasicDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // TODO: Why do we need a separate child for the preset body?
    //       Can't we just use one XML element on the stack?
    
    XmlElement preset ("RBD_StateInfo");
    
    auto presetBody = std::make_unique<XmlElement> ("RBD_Preset");
    mPresetManager->getXmlForPreset (presetBody.get());
    
    preset.addChildElement (presetBody.release());  // this transfers the ownership of `presetBody`
    copyXmlToBinary (preset, destData);
}

void ReallyBasicDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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

float ReallyBasicDelayAudioProcessor::getInputMeterLevel (int inChannel) const
{
    return mInputGain[inChannel]->getMeterLevel();
}

float ReallyBasicDelayAudioProcessor::getOutputMeterLevel (int inChannel) const
{
    return mOutputGain[inChannel]->getMeterLevel();
}

void ReallyBasicDelayAudioProcessor::initialiseDSP()
{
    // Initialise the DSP Gain modules
    // TODO: hardcoding stereo processing here. Refactor!
    for (int channel = 0; channel < 2; ++channel)
    {
        mInputGain[channel] = std::make_unique<GainModule>();
        mOutputGain[channel] = std::make_unique<GainModule>();
        mLfo[channel] = std::make_unique<LfoModule>();
        mDelay[channel] = std::make_unique<DelayModule>();
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReallyBasicDelayAudioProcessor();
}
