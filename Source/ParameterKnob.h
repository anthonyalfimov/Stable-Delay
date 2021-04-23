/*
  ==============================================================================

    ParameterKnob.h
    Created: 13 Jan 2021 9:29:21pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters.h"
#include "SliderLabel.h"

// TODO: Refactor KnobScale when creating SliderScale
//  These classes should be related, since the only difference is whether
//  ticks are drawn linearly or in a circle.
//  These classes don't need to be publically visiable, so they could be moved
//  to be neseted protected / private classes? Or have one class used by
//  Knobs and Sliders, like SliderLabel?

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

    inline static const float majorTickEnd = 39.6f;
    inline static const float majorTickThickness = 1.8f;

    inline static const float minorTickEnd = majorTickEnd - 3.6f;
    inline static const float minorTickThickness = 1.4f;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobScale)
};

class ParameterKnob  : public Component
{
public:
    ParameterKnob (AudioProcessorValueTreeState& stateToControl,
                   Parameter::Index parameterIndex);

private:
    std::unique_ptr<Slider> mSlider;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttachment;
    std::unique_ptr<KnobScale> mScale;
    std::unique_ptr<SliderLabel> mLabel;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterKnob)
};
