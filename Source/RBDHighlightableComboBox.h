/*
  ==============================================================================

    RBDHighlightableComboBox.h
    Created: 10 Mar 2021 10:36:46pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class HighlightableComboBox  : public ComboBox
{
public:
    HighlightableComboBox (const String& componentName = {});
    virtual ~HighlightableComboBox();

//==============================================================================
    /** @internal */
    void mouseEnter (const MouseEvent& event) override;
    /** @internal */
    void mouseExit (const MouseEvent& event) override;
    /** @internal */
    void mouseUp (const MouseEvent& event) override;

private:
    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HighlightableComboBox)
};
