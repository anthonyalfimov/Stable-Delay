/*
  ==============================================================================

    KAPMainPanel.h
    Created: 11 Jan 2021 4:33:36pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPPanelBase.h"
#include "KAPTopPanel.h"
#include "KAPGainPanel.h"
#include "KAPCenterPanel.h"

class KAPMainPanel  : public KAPPanelBase
{
public:
    KAPMainPanel (KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPMainPanel();
    
private:
    std::unique_ptr<KAPTopPanel> mTopPanel;
    std::unique_ptr<KAPGainPanel> mInputGainPanel;
    std::unique_ptr<KAPGainPanel> mOutputGainPanel;
    std::unique_ptr<KAPCenterPanel> mCenterPanel;
};
