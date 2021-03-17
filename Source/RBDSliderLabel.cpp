/*
  ==============================================================================

    RBDSliderLabel.cpp
    Created: 3 Mar 2021 6:25:25pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "RBDSliderLabel.h"

SliderLabel::SliderLabel (Slider* ownerSlider, bool onLeft)
    : isLeftOfOwner (onLeft)
{
    mOwnerSlider = ownerSlider;
    attachToComponent (mOwnerSlider, false);

    if (mOwnerSlider != nullptr)
    {
        // Unregister as a listener with the slider in case it was registered already
        ownerSlider->removeMouseListener (this);

        ownerSlider->addMouseListener (this, false);
        mSliderName = mOwnerSlider->getName();
        setName (mSliderName + "Label");
        setText (mSliderName, dontSendNotification);
    }

    // Set appearance
    setFont (RBD::mainFont);
    setJustificationType (Justification::centred);
    setColour (Label::backgroundColourId, RBD::controlNormalColour);
}

SliderLabel::~SliderLabel()
{
    if (mOwnerSlider != nullptr)
        mOwnerSlider->removeMouseListener (this);
}

void SliderLabel::componentMovedOrResized (Component& component,
                                           bool /*wasMoved*/,
                                           bool /*wasResized*/)
{
    const int height = RBD::labelHeight;
    Rectangle<int> bounds;

    if (isLeftOfOwner)
    {
        const int width = RBD::defaultLabelWidth;
        const int rightPosition = component.getX();
        bounds = component.getBounds().withSizeKeepingCentre (width, height)
                          .withRightX (rightPosition);
    }
    else
    {
        const int width = component.getWidth();
        const int yPosition = component.getBottom();
        bounds = component.getBounds().withSizeKeepingCentre (width, height)
                                      .withY (yPosition);
    }

    setBounds (bounds);
}

void SliderLabel::mouseEnter (const MouseEvent& event)
{
    if (event.eventComponent != mOwnerSlider)
        return;

    setColour (Label::backgroundColourId, RBD::controlHoverColour);

    repaint();
}

void SliderLabel::mouseExit (const MouseEvent& event)
{
    if (event.eventComponent != mOwnerSlider)
        return;

    if (! mOwnerSlider->isMouseButtonDown())
        setColour (Label::backgroundColourId, RBD::controlNormalColour);

    repaint();
}

void SliderLabel::mouseDown (const MouseEvent& event)
{
    if (event.eventComponent != mOwnerSlider)
        return;

    if (isTimerRunning())
        stopTimer();

    setText (mOwnerSlider->getTextFromValue (mOwnerSlider->getValue()),
             dontSendNotification);

    repaint();
}

void SliderLabel::mouseDrag (const MouseEvent& event)
{
    if (event.eventComponent != mOwnerSlider)
        return;

    setText (mOwnerSlider->getTextFromValue (mOwnerSlider->getValue()),
             dontSendNotification);

    repaint();
}

void SliderLabel::mouseUp (const MouseEvent& event)
{
    if (event.eventComponent != mOwnerSlider)
        return;

    if (! mOwnerSlider->isMouseOver())
        setColour (Label::backgroundColourId, RBD::controlNormalColour);

    // Start the timer for switching back to displaying slider name
    startTimer (switchDelayTimeInMs);

    repaint();
}

void SliderLabel::mouseDoubleClick (const MouseEvent& event)
{
    if (event.eventComponent != mOwnerSlider)
        return;

    setText (mOwnerSlider->getTextFromValue (mOwnerSlider->getValue()),
             dontSendNotification);

    repaint();
}

void SliderLabel::timerCallback()
{
    setText (mSliderName, dontSendNotification);

    stopTimer();
    repaint();
}

