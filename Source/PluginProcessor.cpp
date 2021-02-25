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
    // TODO: NormalisableRange takes care of range mapping, why map in the processor?
    // TODO: Implement labels using JUCE's Label class
    // TODO: (?) Change the type of the Type parameter to AudioParameterChoice
    // TODO: (OR) Supply ValueToText function for Type parameter
    //  This should make the host display this parameter using suitable
    //  text labels
    
    AudioProcessorValueTreeState::ParameterLayout layout;

    for (int i = 0; i < Parameter::NumParameters; ++i)
    {
        layout.add (std::make_unique<AudioParameterFloat> (Parameter::ID[i],
                                                           Parameter::Label[i],
                                                           Parameter::Range[i],
                                                           Parameter::DefaultValue[i]));
    }
    
    return layout;
}

//==============================================================================
ReallyBasicDelayAudioProcessor::ReallyBasicDelayAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),         
#endif
    parameters (*this,                      // reference to processor
                nullptr,                    // null pointer to undoManager
                Identifier ("RBD"),         // valueTree indentifier
                createParameterLayout())    // parameter layout
{
    // Initialise DSP modules
    initialiseDSP();
    // Initialise Parameter pointers
    initialiseParameters();
    // Create the preset manager
    mPresetManager = std::make_unique<PresetManager> (*this);
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
        const float* readChannelData = buffer.getReadPointer (channel);
        float* writeChannelData = buffer.getWritePointer (channel);
        
        // TODO: Stereo processing hardcoded. This will fail for >2 channels, will it for mono?
        /* Q: Why can't we use one RBD::Gain object for both channels?
           A: If we have parameter smoothing, it must remain continuos between
              blocks. DSP module keeps information about its state via the
              member variable that stores the "old" parameter value.
              This state information must be preserved per-channel.
        */
        // TODO: Add smoothing for all parameters
        // TODO: Consider updating parameter values only when they change
        //  AudioProgrammer does this in his tutorials by creating a
        //  std::atomic<bool> flag that tells the processor whether it needs
        //  to update parameter values. The flag itself is updated in a
        //  AudioProcessorValueTree listener. He makes the processor itself
        //  a listener.
        //  Note that he still updates the parameters in the processor before
        //  any processing takes place, rather than in the listener.
        //  This is supposed to improve thread safety: so that the listener call
        //  cannot interrupt the processing and change the values in the middle
        //  of it. Is this correct?
        // Q:    Are there significant performance benefits from this approach?
        // A(?): Perhaps, when the plugin has a lot of parameters

        mInputGain[channel]->process (readChannelData,              // inAudio
                                      mInputGainParameter->load(),  // gain
                                      writeChannelData,             // outAudio
                                      buffer.getNumSamples());      // numSamplesToRender
        
        float modulationRate = (channel == 0) ? 0.0f : mModulationRateParameter->load();

        mLfo[channel]->process (modulationRate,                     // rate
                                mModulationDepthParameter->load(),  // depth
                                buffer.getNumSamples());            // numSamplesToRender
        
        mDelay[channel]->process (readChannelData,                  // inAudio
                                  mDelayTimeParameter->load(),      // time
                                  mDelayFeedbackParameter->load(),  // feedback
                                  mDryWetParameter->load(),         // dryWet
                                  mFxTypeParameter->load(),         // type
                                  mLfo[channel]->getBuffer(),       // modulationBuffer
                                  writeChannelData,                 // outAudio
                                  buffer.getNumSamples());          // numSamplesToRender
        
        mOutputGain[channel]->process (readChannelData,             // inAudio
                                       mOutputGainParameter->load(),// gain
                                       writeChannelData,            // outAudio
                                       buffer.getNumSamples());     // numSamplesToRender
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
    //  Can't we just use one XML element on the stack?
    //  Perhaps, this is to add more information to the preset, like information
    //  about currently selected preset?
    
    // TODO: Can we simplify preset management using ValueTree methods?

    // One option is to use `copyState()` and `replaceState()`
    //  AudioProcessorValueTreeState methods together with
    //  `createXml()` and `fromXml()`  ValueTree methods.
    //
    //  Tutorial
    //  https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html
    //  uses these functions to save and recall the whole plugin state to and
    //  from XML.
    //
    // Q: How does this approach work when the number or ranges of parameters
    //    change? Is our approach safer for backwards compatibility, or could we
    //    just use the approach from the tutorial?
    // A: `replaceState()` is not equivalent to our 'loadPresetForXml()' method,
    //    as it simply replaces the reference to the ValueTree's SharedObject.
    //    I.e., it completely replaces the old tree with a new one.
    //    Therefore, it's not suitable for the backwards compatible way of
    //    loading presets.

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
    
    if (xmlState != nullptr)
    {
        forEachXmlChildElement (*xmlState, subChild)
        {
            mPresetManager->loadPresetForXml (subChild);
        }
    }
    else
    {
    // TODO: nullptr can be returned if data is corrupted, send an error instead
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

void ReallyBasicDelayAudioProcessor::initialiseParameters()
{
    mInputGainParameter
    = parameters.getRawParameterValue (Parameter::ID[Parameter::InputGain]);
    mModulationRateParameter
    = parameters.getRawParameterValue (Parameter::ID[Parameter::ModulationRate]);
    mModulationDepthParameter
    = parameters.getRawParameterValue (Parameter::ID[Parameter::ModulationDepth]);
    mDelayTimeParameter
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DelayTime]);
    mDelayFeedbackParameter
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DelayFeedback]);
    mDryWetParameter
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DryWet]);
    mFxTypeParameter
    = parameters.getRawParameterValue (Parameter::ID[Parameter::FxType]);
    mOutputGainParameter
    = parameters.getRawParameterValue (Parameter::ID[Parameter::OutputGain]);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReallyBasicDelayAudioProcessor();
}
