/*
  ==============================================================================

    TitleComponent.h
    Created: 27 May 2021 4:51:55pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MouseEventInvoker.h"
#include "AboutPanel.h"

class TitleComponent  : public Component
{
public:
    TitleComponent();

//==============================================================================
    /** @internal */
    void resized() override;

private:
    MouseEventInvoker mLabelMouseEventInvoker;
    Label mTitleLabel { "title", "Really Basic Delay" };

    std::unique_ptr<PopupComponent> mAboutPanel;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TitleComponent)
};
