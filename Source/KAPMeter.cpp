/*
  ==============================================================================

    KAPMeter.cpp
    Created: 29 Jan 2021 6:29:13pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPMeter.h"
#include "KAPAudioHelpers.h"
#include "KAPInterfaceDefines.h"

KAPMeter::KAPMeter (KAPParameter inParameter, KadenzeAudioPluginAudioProcessor* inProcessor)
    : mParameter (inParameter), mProcessor (inProcessor)
{
    startTimerHz (15);  // start the timer once we know which paremeter we're metering
}

KAPMeter::~KAPMeter()
{
    
}

void KAPMeter::paint (Graphics& g)
{
    // TODO: Programmatically draw meters for the given number of channels (at least 1 or 2)
    
    // Split component width into 3 equal parts: left meter, gap, right meter
    const int meterWidth = getWidth() / 3;
    // Define bounds as <int> for cleaner edges
    Rectangle<int> ch0Bounds = getLocalBounds().withWidth (meterWidth);
    Rectangle<int> ch1Bounds = ch0Bounds.withRightX (getWidth());
    
    // Fill meter background
    g.setColour (KAP::colour6);
    g.fillRoundedRectangle (ch0Bounds.toFloat(), KAP::defaultCornerSize);
    g.fillRoundedRectangle (ch1Bounds.toFloat(), KAP::defaultCornerSize);
    
    // Find the top Y coordinate of the meter fill, clamping it so it doesn't go negative
    const int ch0FillTop = jmax (0, static_cast<int> (getHeight() - (getHeight() * mCh0Level)));
    const int ch1FillTop = jmax (0, static_cast<int> (getHeight() - (getHeight() * mCh0Level)));
    
    // Set bounds for meter fill
    ch0Bounds.setTop (ch0FillTop);
    ch1Bounds.setTop (ch1FillTop);
    
    // Fill meter fill
    g.setColour (KAP::colour7);
    g.fillRoundedRectangle (ch0Bounds.reduced (2).toFloat(), KAP::defaultCornerSize);
    g.fillRoundedRectangle (ch1Bounds.reduced (2).toFloat(), KAP::defaultCornerSize);
}

void KAPMeter::timerCallback()
{
    float updatedCh0Level = 0.0f;
    float updatedCh1Level = 0.0f;
    
    switch (mParameter)
    {
        case kParameter_InputGain:
            // TODO: Fetch actual input level
            updatedCh0Level = 0.75f;
            updatedCh1Level = 0.7f;
            break;
    
        case kParameter_OutputGain:
            // TODO: Fetch actual output level
            updatedCh0Level = 0.35f;
            updatedCh1Level = 0.3f;
            break;
        
        default:
            break;
    }
    
    // Immidiately update Channel Level if new Updated Level is higher than the stored value
    if (updatedCh0Level > mCh0Level)
    {
        mCh0Level = updatedCh0Level;
    }
    else
    {
        mCh0Level = mCh0Level - KAP::meterSmoothingCoef * (mCh0Level - updatedCh0Level);
    }
    
    if (updatedCh1Level > mCh1Level)
    {
        mCh1Level = updatedCh1Level;
    }
    else
    {
        mCh1Level = mCh1Level - KAP::meterSmoothingCoef * (mCh1Level - updatedCh1Level);
    }
    
    // TODO: If we want no denormals there should be a better JUCE way to achieve this
    //       E.g. the Scoped NoDenormals object
    // TODO: If we want to ignore small values, the threshold can be higher
    
    mCh0Level = KAP::denormalise (mCh0Level);
    mCh1Level = KAP::denormalise (mCh1Level);
    
    // TODO: What is the min value threshold when the meter becomes practically invisible?
    // TODO: We could use here some approximate comparison function here
    
    // Repaint only if values are worth painting (non-zero)
    if (mCh0Level > 0.0f && mCh1Level > 0.0f)
        repaint();
}
