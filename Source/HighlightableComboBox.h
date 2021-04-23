/*
  ==============================================================================

    HighlightableComboBox.h
    Created: 10 Mar 2021 10:36:46pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class HighlightableComboBox  : public ComboBox
{
public:
    explicit HighlightableComboBox (const String& componentName = {});
    virtual ~HighlightableComboBox() = default;

//==============================================================================
    /** @internal */
    void mouseEnter (const MouseEvent& event) override;
    /** @internal */
    void mouseExit (const MouseEvent& event) override;
    /** @internal */
    void mouseUp (const MouseEvent& event) override;

private:
//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HighlightableComboBox)
};
