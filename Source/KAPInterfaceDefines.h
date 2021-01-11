/*
  ==============================================================================

    KAPInterfaceDefines.h
    Created: 11 Jan 2021 5:13:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

// TODO: replace with typed constants (in a namespace?)

#define MAIN_PANEL_WIDTH        700
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


