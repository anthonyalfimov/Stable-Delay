/*
  ==============================================================================

    RBDSliderLabel.cpp
    Created: 3 Mar 2021 6:25:25pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "RBDSliderLabel.h"

SliderLabel::SliderLabel (Slider* ownerSlider)
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
}

SliderLabel::~SliderLabel()
{
    if (mOwnerSlider != nullptr)
        mOwnerSlider->removeMouseListener (this);
}

void SliderLabel::paint (Graphics& g)
{
    // Draw label background
    g.setColour (mBgColour);
    g.fillRoundedRectangle (getLocalBounds().toFloat(), RBD::defaultCornerSize);

    // Draw label text
    getLookAndFeel().drawLabel (g, *this);
}

void SliderLabel::componentMovedOrResized (Component& component,
                                           bool /*wasMoved*/,
                                           bool /*wasResized*/)
{
    const int width = component.getWidth();
    const int height = RBD::labelHeight;
    const int yPosition = component.getBottom();
    auto bounds = component.getBounds().withSizeKeepingCentre (width, height)
                                       .withY (yPosition);
    setBounds (bounds);
}

void SliderLabel::mouseEnter (const MouseEvent& event)
{
    if (event.eventComponent != mOwnerSlider)
        return;

    mBgColour = RBD::controlHoverColour;

    repaint();
}

void SliderLabel::mouseExit (const MouseEvent& event)
{
    if (event.eventComponent != mOwnerSlider)
        return;

    if (! mOwnerSlider->isMouseButtonDown())
        mBgColour = RBD::controlNormalColour;

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
        mBgColour = RBD::controlNormalColour;

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

