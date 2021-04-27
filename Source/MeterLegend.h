/*
  ==============================================================================

    MeterLegend.h
    Created: 31 Mar 2021 3:25:42pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MeterLegend  : public Component
{
public:
    MeterLegend (std::initializer_list<int> labelLevels, bool isStereo = true);
    ~MeterLegend(); // dtor must be not inline to be aware of MeterLabel definition
    
//==============================================================================
    /** @internal */
    void resized() override;

private:
    class MeterLabel;
    OwnedArray<MeterLabel> mLabels;

//==============================================================================
    inline static constexpr int labelHeight = 11;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterLegend)
};
