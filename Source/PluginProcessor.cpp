/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "RBDParameters.h"

//==============================================================================
// This creates the plugin parameter layout
AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    // TODO: (?) Change the type of the Type parameter to AudioParameterChoice
    
    AudioProcessorValueTreeState::ParameterLayout layout;

    for (int i = 0; i < Parameter::NumParameters; ++i)
    {
        layout.add (std::make_unique<AudioParameterFloat>
                                    (Parameter::ID[i],
                                     Parameter::Name[i],
                                     Parameter::Range[i],
                                     Parameter::DefaultValue[i],
                                     Parameter::Label[i],
                                     AudioProcessorParameter::genericParameter,
                                     Parameter::stringFromValue[i]));
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
    // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
    return 1;
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
        mInputGain[channel]->prepare (sampleRate, samplesPerBlock);
        mInputMeterProbe[channel]->prepare (sampleRate, samplesPerBlock);
        mDelay[channel]->prepare (sampleRate, samplesPerBlock);
        mDryWetMixer[channel]->prepare (sampleRate, samplesPerBlock);
        mOutputGain[channel]->prepare (sampleRate, samplesPerBlock);
        mOutputMeterProbe[channel]->prepare (sampleRate, samplesPerBlock);
    }
}

void ReallyBasicDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    for (int channel = 0; channel < 2; ++channel)
    {
        mInputGain[channel]->reset();
        mInputMeterProbe[channel]->reset();
        mDelay[channel]->reset();
        mDryWetMixer[channel]->reset();
        mOutputGain[channel]->reset();
        mOutputMeterProbe[channel]->reset();
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
    // TODO: Allow not matching input and output layouts for mono->stereo operation
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
        // FIXME: Stereo processing hardcoded
        /* Q: Why can't we use one RBD::Gain object for both channels?
           A: If we have parameter smoothing, it must remain continuos between
              blocks. DSP module keeps information about its state via the
              member variable that stores the "old" parameter value.
              This state information must be preserved per-channel.
        */

        // TODO: Update parameter values only when it's needed
        /*  AudioProgrammer does this in his tutorials by creating a
            std::atomic<bool> flag that tells the processor whether it needs
            to update parameter values. The flag itself is updated in a
            AudioProcessorValueTree listener. He makes the processor itself
            a listener.
            Note that he still updates the parameters in the processor before
            any processing takes place, rather than in the listener.
            This is supposed to improve thread safety: so that the listener call
            cannot interrupt the processing and change the values in the middle
            of it. Is this correct?
        */

        updateParameters();

        // TODO: Should we ditch the read pointer for anything that writes data?
        //  This should become more clear when we implement mono->stereo
        //  channel configuration.

        const float* readChannelData = buffer.getReadPointer (channel);
        float* writeChannelData = buffer.getWritePointer (channel);
        const auto numSamples = buffer.getNumSamples();

        mInputGain[channel]->process (readChannelData, writeChannelData, numSamples);
        mInputMeterProbe[channel]->process (readChannelData, writeChannelData,
                                            numSamples);
        mDryWetMixer[channel]->pushDryBlock (readChannelData, numSamples);
        mDelay[channel]->process (readChannelData, writeChannelData, numSamples);
        mDryWetMixer[channel]->process (readChannelData, writeChannelData, numSamples);
        mOutputGain[channel]->process (readChannelData, writeChannelData, numSamples);
        mOutputMeterProbe[channel]->process (readChannelData, writeChannelData,
                                             numSamples);
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
    mPresetManager->createXmlForPreset (presetBody.get());
    
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
            mPresetManager->loadPresetFromXml (subChild);
        }
    }
    else
    {
    // TODO: nullptr can be returned if data is corrupted, send an error instead
        jassertfalse;
    }
}

MeterProbe* ReallyBasicDelayAudioProcessor::getInputMeterProbe (int channel) const
{
    return mInputMeterProbe[channel].get();
}

MeterProbe* ReallyBasicDelayAudioProcessor::getOutputMeterProbe (int channel) const
{
    return mOutputMeterProbe[channel].get();
}

void ReallyBasicDelayAudioProcessor::initialiseDSP()
{
    // TODO: Hardcoding stereo processing here. Refactor!
    
    for (int channel = 0; channel < 2; ++channel)
    {
        mInputGain[channel] = std::make_unique<GainModule>();
        mInputMeterProbe[channel] = std::make_unique<MeterProbe>();
        mDelay[channel] = std::make_unique<DelayModule>();
        mDryWetMixer[channel] = std::make_unique<DryWetModule>();
        mOutputGain[channel] = std::make_unique<GainModule>();
        mOutputMeterProbe[channel] = std::make_unique<MeterProbe>();
    }
}

void ReallyBasicDelayAudioProcessor::initialiseParameters()
{
    // MARK: Retrieving parameter pointers from APVTS uses string search
    //  String search is slow, so better keep this off the processBlock.
    //  Pointers to the parameter atomic values do not change during execution,
    //  so it is preferable to retrieve them from APVTS just once.
    //  Create std::atomic<float> members (or an array of them) and initialise
    //  them once in the processor constructor.

    mInputGainValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::InputGain]);
    mDelayTimeValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DelayTime]);
    mDelayFeedbackValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DelayFeedback]);
    mDryWetValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DryWet]);
    mFxTypeValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::FxType]);
    mOutputGainValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::OutputGain]);
    mModulationRateValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::ModulationRate]);
    mModulationDepthValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::ModulationDepth]);
    mStereoWidthValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::StereoWidth]);

    updateParameters();
}

void ReallyBasicDelayAudioProcessor::updateParameters()
{
    for (int channel = 0; channel < 2; ++channel)
    {
        mInputGain[channel]->setState (mInputGainValue->load());
        mDelay[channel]->setState (mDelayTimeValue->load(),
                                   mDelayFeedbackValue->load(),
                                   mFxTypeValue->load(),
                                   mModulationRateValue->load(),
                                   mModulationDepthValue->load(),
                                   mStereoWidthValue->load(),
                                   (channel != 0));
        mDryWetMixer[channel]->setState (mDryWetValue->load());
        mOutputGain[channel]->setState (mOutputGainValue->load());
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReallyBasicDelayAudioProcessor();
}
