/*
  ==============================================================================

    ClickableLabel.h
    Created: 6 May 2021 10:50:28pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ClickableLabel  : public Label
{
public:
    ClickableLabel (const String& componentName = String(),
                    const String& labelText = String());

//==============================================================================
    /**
        You can assign a lambda to this callback object to have it called when
        the label is clicked.
    */
    std::function<void()> onClick;

protected:
//==============================================================================
    /** @internal */
    void mouseDown (const MouseEvent& event) override;
};
