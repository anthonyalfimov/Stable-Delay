/*
  ==============================================================================

    RBDParameterKnob.h
    Created: 13 Jan 2021 9:29:21pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDParameters.h"
#include "RBDSliderLabel.h"

// TODO: Consider using setBufferedToImage() since scale doesn't change
// TODO: Consider using setComponentEffect() for drop shadow

class KnobScale  : public Component
{
public:
    KnobScale (Slider& parentSlider, Parameter::Index parameterIndex);

//==============================================================================
    /** @internal */
    void paint (Graphics& g) override;

private:
    Path mTicks;
    Path mTickShadow;

    inline static const float tickStart = 29.0f;

    inline static const float majorTickEnd = 38.6f;
    inline static const float majorTickThickness = 1.8f;

    inline static const float minorTickEnd = majorTickEnd - 2.0f;
    inline static const float minorTickThickness = 1.4f;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobScale)
};

class ParameterKnob  : public Component
{
public:
    ParameterKnob (AudioProcessorValueTreeState& stateToControl,
                   Parameter::Index parameterIndex);
    ~ParameterKnob();

private:
    std::unique_ptr<Slider> mSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttachment;
    std::unique_ptr<KnobScale> mScale;
    std::unique_ptr<SliderLabel> mLabel;


    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterKnob)
};
