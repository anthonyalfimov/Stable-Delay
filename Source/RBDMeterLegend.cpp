/*
  ==============================================================================

    RBDMeterLegend.cpp
    Created: 31 Mar 2021 3:25:42pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include <set>
#include "RBDMeterLegend.h"
#include "RBDMeterChannel.h"
#include "RBDInterfaceConstants.h"

//==============================================================================
//  MeterLabel

MeterLabel::MeterLabel (int levelValue, bool isStereo)
    : level (levelValue), mText (abs (levelValue)), mIsStereo (isStereo)
{

}

void MeterLabel::paint (Graphics& g)
{
    const auto bounds = getLocalBounds();
    const int centreY = bounds.getCentreY();
    const int width = bounds.getWidth();

    // Draw label ticks
    g.setColour (RBD::meterLegendColour);
    g.drawHorizontalLine (centreY, 0, tickLenght);  // always draw the left tick

    if (mIsStereo)  // draw the right tick only is stereo configuration
        g.drawHorizontalLine (centreY, width - tickLenght, width);

    // Draw label
    g.setFont (RBD::meterFont);
    g.setColour (RBD::meterLegendColour);
    g.drawText (mText, bounds, Justification::centred);
}

//==============================================================================
//  MeterLegend

MeterLegend::MeterLegend (std::initializer_list<int> labelLevels, bool isStereo)
{
    // Construct a set from the passed list to sort it and remove duplicates
    std::set<int> values (labelLevels);

    for (int value : values)
    {
        if (value > MeterChannel::minLevelInDb && value < MeterChannel::maxLevelInDb)
            addAndMakeVisible (mLabels.add (std::make_unique<MeterLabel> (value,
                                                                          isStereo)));
    }
}

void MeterLegend::resized()
{
    Rectangle<int> bounds (0, 0, getWidth(), 11);
    const int centreX = bounds.getCentreX();

    for (auto& label : mLabels)
    {
        int centreY = (1.0f - MeterChannel::range.convertTo0to1 (label->level))
                        * getHeight();
        bounds.setCentre (centreX, centreY);
        label->setBounds (bounds);
    }
}
