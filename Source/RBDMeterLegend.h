/*
  ==============================================================================

    RBDMeterLegend.h
    Created: 31 Mar 2021 3:25:42pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// TODO: Consider using setBufferedToImage() since legend doesn't change

class MeterLabel  : public Component
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
    
    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterLabel)
};

class MeterLegend  : public Component
{
public:
    MeterLegend (std::initializer_list<int> labelLevels, bool isStereo = true);
    
//==============================================================================
    /** @internal */
    void resized() override;

private:
    OwnedArray<MeterLabel> mLabels;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterLegend)
};
