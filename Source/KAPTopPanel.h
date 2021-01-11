/*
  ==============================================================================

    KAPTopPanel.h
    Created: 11 Jan 2021 4:33:47pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPPanelBase.h"

class KAPTopPanel  : public KAPPanelBase
{
public:
    KAPTopPanel (KadenzeAudioPluginAudioProcessor*);
    ~KAPTopPanel();
    
    void paint (Graphics&) override;
    
private:
    
};
