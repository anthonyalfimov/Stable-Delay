/*
  ==============================================================================

    KAPInterfaceDefines.h
    Created: 11 Jan 2021 5:13:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// TODO: replace with typed constants (int) in a namespace

#define MAIN_PANEL_WIDTH        750
#define MAIN_PANEL_HEIGHT       300

#define TOP_PANEL_WIDTH         MAIN_PANEL_WIDTH
#define TOP_PANEL_HEIGHT        45

// NB! using defines inside expressions can lead to unexpected results due to precedence!

#define GAIN_PANEL_WIDTH        100
#define GAIN_PANEL_HEIGHT       MAIN_PANEL_HEIGHT - TOP_PANEL_HEIGHT

#define CENTER_PANEL_WIDTH      MAIN_PANEL_WIDTH - (GAIN_PANEL_WIDTH * 2)
#define CENTER_PANEL_HEIGHT     GAIN_PANEL_HEIGHT

#define CENTER_PANEL_MENU_BAR_WIDTH     CENTER_PANEL_WIDTH
#define CENTER_PANEL_MENU_BAR_HEIGHT    35

#define FX_PANEL_WIDTH          CENTER_PANEL_WIDTH
#define FX_PANEL_HEIGHT         CENTER_PANEL_HEIGHT - CENTER_PANEL_MENU_BAR_HEIGHT

namespace KAP
{
    // TODO: descriptive colour names
    // const is static by default, explicitely state it to highlight intent
    const static Colour colour1 = Colour (105, 105, 105);
    const static Colour colour2 = Colour (0, 0, 0).withAlpha (0.0f);
    const static Colour colour3 = Colour (0, 0, 0).withAlpha (0.3f);
    const static Colour colour4 = Colour (0, 0, 0).withAlpha (0.6f);
    const static Colour colour5 = Colour (105, 105, 105).withAlpha (0.3f);
    const static Colour colour6 = Colour (0, 0, 0).withAlpha (0.8f);
    const static Colour colour7 = Colour (125, 125, 125);
    
    // TODO: embed the font file as a binary into the plugis for cross-compatibility
    // This accesses a system font:
    const static Font font1 = Font ("Helvetica Neue", 12.0f, Font::bold);
    const static Font font2 = Font ("Helvetica Neue", 22.0f, Font::bold);
    const static Font font3 = Font ("Helvetica Neue", 48.0f, Font::bold);
}
