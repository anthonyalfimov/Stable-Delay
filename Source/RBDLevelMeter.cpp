/*
  ==============================================================================

    RBDLevelMeter.cpp
    Created: 29 Jan 2021 6:29:13pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDLevelMeter.h"
#include "RBDAudioUtilities.h"
#include "RBDInterfaceConstants.h"

LevelMeter::LevelMeter (Parameter::Index parameterIndex,
                        ReallyBasicDelayAudioProcessor& processor)
    : mParameterIndex (parameterIndex), mProcessor (processor)
{
    startTimerHz (15);  // start the timer once we know which paremeter we're metering
}

LevelMeter::~LevelMeter()
{
    
}

void LevelMeter::paint (Graphics& g)
{
    // TODO: Programmatically draw meters for the given number of channels
    // TODO: Add colour to the meters
    // TODO: Add clipping indicators
    
    // Split component width into 3 equal parts: left meter, gap, right meter
    const int meterWidth = getWidth() / 3;
    // Define bounds as <int> for cleaner edges
    Rectangle<int> ch0Bounds = getLocalBounds().withWidth (meterWidth);
    Rectangle<int> ch1Bounds = ch0Bounds.withRightX (getWidth());
    
    // Fill meter background
    g.setColour (RBD::colour6);
    g.fillRoundedRectangle (ch0Bounds.toFloat(), RBD::defaultCornerSize);
    g.fillRoundedRectangle (ch1Bounds.toFloat(), RBD::defaultCornerSize);
    
    // Find the top Y coordinate of the meter fill, clamping it so it doesn't
    //  go negative. If mCh0/1Level is negative, move the Top of the meter
    //  below the Bottom, making it height 0
    const int ch0FillTop = jmax (0, static_cast<int> (getHeight()
                                                      - (getHeight() * mCh0Level)));
    const int ch1FillTop = jmax (0, static_cast<int> (getHeight()
                                                      - (getHeight() * mCh1Level)));
    
    // Set bounds for meter fill
    ch0Bounds.setTop (ch0FillTop);
    ch1Bounds.setTop (ch1FillTop);
    
    // Fill meter fill
    g.setColour (RBD::colour7);
    g.fillRoundedRectangle (ch0Bounds.reduced (2).toFloat(), RBD::defaultCornerSize);
    g.fillRoundedRectangle (ch1Bounds.reduced (2).toFloat(), RBD::defaultCornerSize);
}

void LevelMeter::timerCallback()
{
    float updatedCh0Level = 0.0f;
    float updatedCh1Level = 0.0f;
    
    switch (mParameterIndex)
    {
        case Parameter::InputGain:
            updatedCh0Level = mProcessor.getInputMeterLevel (0);
            updatedCh1Level = mProcessor.getInputMeterLevel (1);
            break;
    
        case Parameter::OutputGain:
            updatedCh0Level = mProcessor.getOutputMeterLevel (0);
            updatedCh1Level = mProcessor.getOutputMeterLevel (1);
            break;
        
        default:
            break;
    }
    
    // TODO: Already smoothing the levels in RBDGain. Do we need to smooth it twice?

    // Immidiately update Channel Level if new Updated Level is higher than the
    //  stored value
    if (updatedCh0Level > mCh0Level)
    {
        mCh0Level = updatedCh0Level;
    }
    else
    {
        mCh0Level = mCh0Level - RBD::meterSmoothingCoef * (mCh0Level - updatedCh0Level);
    }
    
    if (updatedCh1Level > mCh1Level)
    {
        mCh1Level = updatedCh1Level;
    }
    else
    {
        mCh1Level = mCh1Level - RBD::meterSmoothingCoef * (mCh1Level - updatedCh1Level);
    }
    
    // TODO: If we want to disable denormals, use ScopedNoDenormals object
    //  Since this processing occurs not inside the process block, it does not
    //  benefit from disabled denormals there.
    //  However, the threshold used for RBD::denormalise() is much greater than
    //  the smallest normal float value. So, this is not what's happening here.
    // TODO: If we want to ignore small values, calculate threashold better
    //  The threshold should then correspond with the lowest displayed
    //  meter level - minMeterDbLevel, and the "floor" constant (1e-15) in the
    //  denormalise function
    
    mCh0Level = RBD::denormalise (mCh0Level);
    mCh1Level = RBD::denormalise (mCh1Level);

    // FIXME: When minMeterDbLevel is higher than 96, the meter gets "stuck"
    //  Probably level becomes negative too quickly so that the meter stops
    //  being repainted. Probalem gets fixed when forcing a repaint by hovering
    //  over a knob
    
    // Repaint only if values are worth painting (non-zero)
    if (mCh0Level > 0.0f && mCh1Level > 0.0f)
        repaint();
}
