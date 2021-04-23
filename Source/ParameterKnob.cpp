/*
  ==============================================================================

    ParameterKnob.cpp
    Created: 13 Jan 2021 9:29:21pm
    Author:  Anthony

  ==============================================================================
*/

#include "ParameterKnob.h"
#include "InterfaceConstants.h"

//==============================================================================
//  KnobScale

KnobScale::KnobScale (Slider& parentSlider, Parameter::Index parameterIndex)
{
    // TODO: Size should be set by the parent knob for flexibility
    setSize (RBD::knobSize, RBD::knobSize);

    // Retrieve parameter properties
    const auto& majorTickValues = Parameter::majorTicks[parameterIndex];
    const auto& minorTickValues = Parameter::minorTicks[parameterIndex];
    const auto& range = Parameter::Range[parameterIndex];

    // Retrieve geometric properties
    const auto origin = getLocalBounds().getCentre().toFloat();
    const auto [minAngle, maxAngle, other] = parentSlider.getRotaryParameters();
    ignoreUnused (other);

    {
        Path minorTicks;

        for (auto minorTickValue : minorTickValues)
        {
            const float value0to1 = range.convertTo0to1 (minorTickValue);
            const float angle = jmap (value0to1, minAngle, maxAngle);

            Line tick (origin.getPointOnCircumference (tickStart, angle),
                       origin.getPointOnCircumference (minorTickEnd, angle));

            minorTicks.addLineSegment (tick, minorTickThickness);
        }

        mTicks.addPath (minorTicks
                        .createPathWithRoundedCorners (minorTickThickness / 2.0f));
    }

    {
        Path majorTicks;

        for (auto majorTickValue : majorTickValues)
        {
            const float value0to1 = range.convertTo0to1 (majorTickValue);
            const float angle = jmap (value0to1, minAngle, maxAngle);

            Line tick (origin.getPointOnCircumference (tickStart, angle),
                       origin.getPointOnCircumference (majorTickEnd, angle));

            majorTicks.addLineSegment (tick, majorTickThickness);
        }

        mTicks.addPath (majorTicks
                        .createPathWithRoundedCorners (majorTickThickness / 2.0f));
    }

    mTickShadow = mTicks;
    mTickShadow.applyTransform (AffineTransform::translation (RBD::tickShadowOffset));
    mTickShadow.addRectangle (mTickShadow.getBounds().expanded (2.0f));
    mTickShadow.setUsingNonZeroWinding (false);
}

void KnobScale::paint (Graphics& g)
{
    g.setColour (RBD::sliderTickColour);
    g.fillPath (mTicks);

    // Reset the following changes to the graphics context at the end of the scope
    Graphics::ScopedSaveState saveState (g);
    g.reduceClipRegion (mTicks);  // set clip region to the knob mark

    g.setColour (RBD::shadowColour);
    g.fillPath (mTickShadow);
}

//==============================================================================
//  ParameterKnob

ParameterKnob::ParameterKnob (AudioProcessorValueTreeState& stateToControl,
                              Parameter::Index parameterIndex)
{
    const auto parameter = stateToControl.getParameter (Parameter::ID[parameterIndex]);

    if (parameter == nullptr)
    {
        jassertfalse;
        return;
    }

    // Set up component
    setSize (RBD::knobSize, RBD::knobSize + RBD::labelHeight);
    setName (Parameter::Name[parameterIndex] + "Control");

    // Create and set up Slider as a rotary knob
    mSlider = std::make_unique<Slider> (Parameter::Name[parameterIndex]);
    mSlider->setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mSlider->setTextBoxStyle (Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mSlider->setTextValueSuffix (parameter->getLabel());

    // NOTE: Knob image rotation range is [-126º, 126º]
    //  RotaryParameters requires angles in radians that are positive, going
    //  from 12 o'clock position clockwise, with end angle greater than start
    //  angle.
    const float knobAngleAmplitudeDegrees = 126.0f;
    const float startAngle = degreesToRadians (360.0f - knobAngleAmplitudeDegrees);
    const float endAngle = degreesToRadians (360.0f + knobAngleAmplitudeDegrees);

    mSlider->setRotaryParameters (startAngle, endAngle, true);

    mSlider->setBounds (0, 0, RBD::knobSize, RBD::knobSize);
    addAndMakeVisible (mSlider.get());

    // Create Slider Attachment
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    mAttachment = std::make_unique<SliderAttachment> (stateToControl,
                                                      Parameter::ID[parameterIndex],
                                                      *mSlider);

    // Create and set up knob scale
    mScale = std::make_unique<KnobScale> (*mSlider, parameterIndex);
    mScale->setBounds (0, 0, RBD::knobSize, RBD::knobSize);
    addAndMakeVisible (mScale.get());
    mScale->toBehind (mSlider.get());   // place scale ticks behind slider

    // Create label
    mLabel = std::make_unique<SliderLabel> (mSlider.get());
    addAndMakeVisible (mLabel.get());
}

ParameterKnob::~ParameterKnob()
{
    
}
