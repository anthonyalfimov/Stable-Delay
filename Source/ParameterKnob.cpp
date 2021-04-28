/*
  ==============================================================================

    ParameterKnob.cpp
    Created: 13 Jan 2021 9:29:21pm
    Author:  Anthony

  ==============================================================================
*/

#include "ParameterKnob.h"
#include "ParameterToggle.h"
#include "InterfaceConstants.h"

//==============================================================================
//  KnobScale

KnobScale::KnobScale (Slider& parentSlider, Parameter::Index parameterIndex)
{
    // TODO: Size should be set by the parent component for flexibility
    setSize (RBD::knobSize, RBD::knobSize);

    // TODO: Consider using setBufferedToImage() since scale doesn't change

    // Retrieve parameter properties
    const auto& majorTickValues = Parameter::majorTicks[parameterIndex];
    const auto& minorTickValues = Parameter::minorTicks[parameterIndex];
    const auto& range = Parameter::Range[parameterIndex];

    // Retrieve geometric properties
    const auto origin = getLocalBounds().getCentre().toFloat();
    const auto [minAngle, maxAngle, other] = parentSlider.getRotaryParameters();
    ignoreUnused (other);

    // NOTE: Instead of adding line segments to the path and then rounding the
    //  corners, we could create a PathStrokeType object and use its
    //  PathStrokeType::createStrokedPath() method to generate the tick shapes.
    //  In this case, we need to offset the path end positions to account for
    //  the rounded end caps that stick beyond the end point.

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
//  ParameterKnob::ConnectedToggle

class ParameterKnob::ConnectedToggle  : public ParameterToggle
{
public:
    ConnectedToggle (AudioProcessorValueTreeState& stateToControl,
                     Parameter::Index parameterIndex);

protected:
//==============================================================================
    /** @internal */
    void paintButton (Graphics& g,
                      bool shouldDrawButtonAsHighlighted,
                      bool shouldDrawButtonAsDown) override;

private:
    Path getToggleShape (Rectangle<float> bounds, float inset, bool isClosed) const;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectedToggle)
};

ParameterKnob
::ConnectedToggle::ConnectedToggle (AudioProcessorValueTreeState& stateToControl,
                                    Parameter::Index parameterIndex)
    : ParameterToggle (stateToControl, parameterIndex)
{
}

void ParameterKnob::ConnectedToggle::paintButton (Graphics& g,
                                                  bool shouldDrawButtonAsHighlighted,
                                                  bool shouldDrawButtonAsDown)
{
    Colour bgColour = /*shouldDrawButtonAsHighlighted ? RBD::controlHoverColour
                                                    :*/ Colour (0, 0, 0).withAlpha (0.1f);

    if (getToggleState())
        bgColour = RBD::controlActiveColour;

    Path background = getToggleShape (getLocalBounds().toFloat(), 0.0f, true);
    g.setColour (bgColour);
    g.fillPath (background);

    // TODO: use clipping to draw background inset by 1 / whatever the thickness

    // Reset the following changes to the graphics context at the end of the scope
    Graphics::ScopedSaveState saveState (g);
    g.reduceClipRegion (background);  // set clip region to the knob mark

    Colour outlineColour = /*shouldDrawButtonAsHighlighted ? RBD::controlHoverColour
                                                         :*/ Colour (36, 36, 36);
    Path outline = getToggleShape (getLocalBounds().toFloat(), 0.5f, false);
    g.setColour (outlineColour);
    g.strokePath (outline, PathStrokeType (1.0f, PathStrokeType::mitered));
}

Path ParameterKnob::ConnectedToggle::getToggleShape (Rectangle<float> bounds,
                                                     float inset,
                                                     bool isClosed) const
{
    const float x0 = bounds.getX() + inset;
    const float y0 = bounds.getY();
    const float x1 = bounds.getRight() - inset;
    const float y1 = bounds.getBottom() - inset;

    const float cs = RBD::defaultCornerSize;
    const float cs45 = cs * 0.45f;

    Path shape;

    shape.startNewSubPath (x0, y0);
    shape.lineTo (x0, y1 - cs);
    shape.cubicTo (x0, y1 - cs45,
                   x0 + cs45, y1,
                   x0 + cs, y1);
    shape.lineTo (x1 - cs, y1);
    shape.cubicTo (x1 - cs45, y1,
                   x1, y1 - cs45,
                   x1, y1 - cs);
    shape.lineTo (x1, y0);

    if (! isClosed)
        return shape;

    shape.cubicTo (x1, y0 + cs - cs45,
                   x1 - cs45, y0 + cs,
                   x1 - cs, y0 + cs);
    shape.lineTo (x0 + cs, y0 + cs);
    shape.cubicTo (x0 + cs45, y0 + cs,
                   x0, y0 + cs - cs45,
                   x0, y0);

    return shape;
}

//==============================================================================
//  ParameterKnob

ParameterKnob::ParameterKnob (AudioProcessorValueTreeState& stateToControl,
                              Parameter::Index knobParameterIndex)
{
    const auto parameter
    = stateToControl.getParameter (Parameter::ID[knobParameterIndex]);

    if (parameter == nullptr)
    {
        jassertfalse;
        return;
    }

    // Set up component
    setSize (RBD::knobSize, RBD::knobSize + RBD::labelHeight);
    setName (Parameter::Name[knobParameterIndex] + "Control");

    // Create and set up Slider as a rotary knob
    mSlider = std::make_unique<Slider> (Parameter::Name[knobParameterIndex]);
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
                                                      Parameter::ID[knobParameterIndex],
                                                      *mSlider);

    // Create and set up knob scale
    mScale = std::make_unique<KnobScale> (*mSlider, knobParameterIndex);
    mScale->setBounds (0, 0, RBD::knobSize, RBD::knobSize);
    addAndMakeVisible (mScale.get());
    mScale->toBehind (mSlider.get());   // place scale ticks behind slider

    // Create label
    mLabel = std::make_unique<SliderLabel> (mSlider.get());
    addAndMakeVisible (mLabel.get());
}

ParameterKnob::ParameterKnob (AudioProcessorValueTreeState& stateToControl,
                              Parameter::Index knobParameterIndex,
                              Parameter::Index toggleParameterIndex)
    : ParameterKnob (stateToControl, knobParameterIndex)
{
    if (toggleParameterIndex == knobParameterIndex
        || stateToControl.getParameter (Parameter::ID[toggleParameterIndex]) == nullptr)
    {
        jassertfalse;
        return;
    }

    // Extend bounds to house connected toggle
    setSize (getWidth(), getHeight() + RBD::toggleHeight);

    // Create connected toggle
    mToggle = std::make_unique<ConnectedToggle> (stateToControl,
                                                 toggleParameterIndex);
    const auto bounds
    = getLocalBounds().withHeight (RBD::toggleHeight + RBD::defaultCornerSize)
                      .withBottomY (getBottom());
    mToggle->setBounds (bounds);
    addAndMakeVisible (mToggle.get());
    mToggle->toBehind (mLabel.get());   // place toggle behind knob label

    mHasToggle = true;
}

// Dtor must be not inline to be aware of ConnectedToggle definition
ParameterKnob::~ParameterKnob() = default;
