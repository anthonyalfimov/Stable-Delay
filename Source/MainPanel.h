/*
  ==============================================================================

    MainPanel.h
    Created: 11 Jan 2021 4:33:36pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "InterfacePanel.h"
#include "TopPanel.h"
#include "GainPanel.h"
#include "CentrePanel.h"
#include "AboutPanel.h"

class MainPanel  : public InterfacePanel
{
public:
    explicit MainPanel (ReallyBasicDelayAudioProcessor& processor);
    ~MainPanel();

//==============================================================================
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void mouseDown (const MouseEvent& event) override;
    
private:
    std::unique_ptr<TopPanel> mTopPanel;
    std::unique_ptr<GainPanel> mInputGainPanel;
    std::unique_ptr<GainPanel> mOutputGainPanel;
    std::unique_ptr<CentrePanel> mCentrePanel;

//==============================================================================
    const Component* mAboutPanelTrigger = nullptr;
    std::unique_ptr<AboutPanel> mAboutPanel;

//==============================================================================
    Image mBackgroundImage;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};
