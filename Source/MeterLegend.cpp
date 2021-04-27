/*
  ==============================================================================

    MeterLegend.cpp
    Created: 31 Mar 2021 3:25:42pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include <set>
#include "MeterLegend.h"
#include "MeterChannel.h"
#include "InterfaceConstants.h"

//==============================================================================
//  MeterLabel

class MeterLegend::MeterLabel  : public Component
{
public:
    explicit MeterLabel (int levelValue, bool isStereo = true);
    const int level;

//==============================================================================
    /** @internal */
    void paint (Graphics& g) override;

private:
    const String mText;
    const bool mIsStereo;

    inline static const int tickLenght = 3;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterLabel)
};

MeterLegend::MeterLabel::MeterLabel (int levelValue, bool isStereo)
    : level (levelValue), mText (abs (levelValue)), mIsStereo (isStereo)
{

}

void MeterLegend::MeterLabel::paint (Graphics& g)
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
    // TODO: Consider using setBufferedToImage() since legend doesn't change

    // Construct a set from the passed list to sort it and remove duplicates
    std::set<int> values (labelLevels);

    for (int value : values)
    {
        if (value > MeterChannel::minLevelInDb && value < MeterChannel::maxLevelInDb)
            addAndMakeVisible (mLabels.add (std::make_unique<MeterLabel> (value,
                                                                          isStereo)));
    }
}

// Dtor must not be inline to be aware of MeterLabel definition
MeterLegend::~MeterLegend() = default;

void MeterLegend::resized()
{
    Rectangle<int> bounds (0, 0, getWidth(), labelHeight);
    const int centreX = bounds.getCentreX();

    for (auto& label : mLabels)
    {
        int centreY = (1.0f - MeterChannel::range.convertTo0to1 (label->level))
                        * getHeight();
        bounds.setCentre (centreX, centreY);
        label->setBounds (bounds);
    }
}
