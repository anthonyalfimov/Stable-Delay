/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

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
    return 4.0;
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

    for (auto inputMeterProbe : mInputMeterProbe)
        inputMeterProbe->prepare (sampleRate, samplesPerBlock);

    for (auto fxProcessor : mFxProcessor)
        fxProcessor->prepare (sampleRate, samplesPerBlock);

    for (auto dryWetMixer : mDryWetMixer)
        dryWetMixer->prepare (sampleRate, samplesPerBlock);

    for (auto outputClipper : mOutputClipper)
    {
        outputClipper->setState (SaturationCurve::gamma);
        outputClipper->prepare (sampleRate, samplesPerBlock);
    }

    for (auto outputGain : mOutputGain)
        outputGain->prepare (sampleRate, samplesPerBlock);

    for (auto outputMeterProbe : mOutputMeterProbe)
        outputMeterProbe->prepare (sampleRate, samplesPerBlock);
}

void ReallyBasicDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

    for (auto inputMeterProbe : mInputMeterProbe)
        inputMeterProbe->reset();

    for (auto fxProcessor : mFxProcessor)
        fxProcessor->reset();

    for (auto dryWetMixer : mDryWetMixer)
        dryWetMixer->reset();

    for (auto outputClipper : mOutputClipper)
        outputClipper->reset();

    for (auto outputGain : mOutputGain)
        outputGain->reset();

    for (auto outputMeterProbe : mOutputMeterProbe)
        outputMeterProbe->reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReallyBasicDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // Support mono & stereo layout for output bus
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // Input bus should not have more channels than output bus
    if (layouts.getMainOutputChannels() < layouts.getMainInputChannels())
        return false;

    return true;
  #endif
}
#endif

void ReallyBasicDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    /* Denormals are temporarily disabled when this object is created at the
       beginning of the process block and re-enabled when it's destroyed at the
       end of the process block. Therefore, anything that happens within the
       process block doesn't need to disable denormals - they won't be
       re-enabled until the end of the process block.
    */
    juce::ScopedNoDenormals noDenormals;

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

    const auto totalNumInputChannels  = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();
    const auto numSamples = buffer.getNumSamples();

    // Processing for input channels:
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        mInputMeterProbe[channel]->process (channelData, channelData, numSamples);
    }

    // If we have more output channels than input channels, copy data from the
    //  first input channel to all extra output channels
    for (int channel = totalNumInputChannels; channel < totalNumOutputChannels;
         ++channel)
    {
        buffer.copyFrom (channel, 0, buffer, 0, 0, numSamples);
    }

    // Processing for output channels:
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        mDryWetMixer[channel]->writeDryBlock (channelData, numSamples);
        mFxProcessor[channel]->process (channelData, channelData, numSamples);
        mDryWetMixer[channel]->process (channelData, channelData, numSamples);
        mOutputGain[channel]->process (channelData, channelData, numSamples);
        mOutputMeterProbe[channel]->process (channelData, channelData, numSamples);
        //mOutputClipper[channel]->process(channelData, channelData, numSamples);
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
        jassertfalse;
    }
}

MeterProbe* ReallyBasicDelayAudioProcessor::getInputMeterProbe (int channel) const
{
    if (channel < getTotalNumInputChannels())
        return mInputMeterProbe[channel];

    jassertfalse;
    return nullptr;
}

MeterProbe* ReallyBasicDelayAudioProcessor::getOutputMeterProbe (int channel) const
{
    if (channel < getTotalNumOutputChannels())
        return mOutputMeterProbe[channel];

    jassertfalse;
    return nullptr;
}

void ReallyBasicDelayAudioProcessor::initialiseDSP()
{
    const auto totalNumInputChannels = getTotalNumInputChannels();

    // Create DSP modules for input channels
    for (int i = 0; i < totalNumInputChannels; ++i)
    {
        mInputMeterProbe. add (std::make_unique<MeterProbe>());
    }

    const auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Create DSP modules for output channels
    for (int i = 0; i < totalNumOutputChannels; ++i)
    {
        mFxProcessor.add (std::make_unique<FxModule>());
        mDryWetMixer.add (std::make_unique<DryWetModule>());
        mOutputClipper.add (std::make_unique<SaturationModule>());
        mOutputGain.add (std::make_unique<GainModule>());
        mOutputMeterProbe.add (std::make_unique<MeterProbe>());
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

    mInputDriveValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::InputDrive]);
    mInputBoostValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::InputBoost]);
    mDelayTimeValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DelayTime]);
    mFeedbackValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::Feedback]);
    mInvertFeedbackValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::InvertFeedback]);
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
    mStereoSpreadValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::StereoSpread]);

    mDClipDynamicValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DClipDynamic]);
    mDClipRiseValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DClipRise]);
    mDClipFallValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DClipFall]);
    mDClipFbHeadroomValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DClipFbHeadroom]);
    mDClipFeedbackDecayValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DClipFeedbackDecay]);
    mDClipOutputDetectorValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DClipOutputDetector]);
    mDClipPostCutFactorValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DClipPostCutFactor]);
    mDClipFbCompensationValue
    = parameters.getRawParameterValue (Parameter::ID[Parameter::DClipFbCompensation]);
    
    updateParameters();
}

void ReallyBasicDelayAudioProcessor::updateParameters()
{
    const float feedbackFactor
    = (mInvertFeedbackValue->load() == Toggle::On) ? -1.0f : 1.0f;
    const float feedback = feedbackFactor * mFeedbackValue->load();

    const float stereoSpread
    = (getTotalNumOutputChannels() == 2) ? mStereoSpreadValue->load() : 0.0f;
    
    const bool outputDetector = mDClipOutputDetectorValue->load() == Toggle::On;
    
    for (int channel = 0; channel < mFxProcessor.size(); ++channel)
        mFxProcessor[channel]->setState (mInputDriveValue->load(),
                                         mDelayTimeValue->load(),
                                         feedback,
                                         mFxTypeValue->load(),
                                         mModulationRateValue->load(),
                                         mModulationDepthValue->load(),
                                         stereoSpread,
                                         (channel != 0),
                                         mDClipDynamicValue->load() == Toggle::On,
                                         mDClipRiseValue->load(),
                                         mDClipFallValue->load(),
                                         mDClipFbHeadroomValue->load(),
                                         static_cast<DClip::FeedbackDecayMode>(mDClipFeedbackDecayValue->load()),
                                         (channel == 0) && outputDetector,
                                         mDClipPostCutFactorValue->load(),
                                         static_cast<DClip::CompensationMode> (mDClipFbCompensationValue->load()));

    for (auto dryWetMixer : mDryWetMixer)
        dryWetMixer->setState (mDryWetValue->load());

    for (auto outputGain : mOutputGain)
        outputGain->setState (mOutputGainValue->load());
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReallyBasicDelayAudioProcessor();
}
