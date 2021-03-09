/*
  ==============================================================================

    RBDPluginLookAndFeel.cpp
    Created: 9 Mar 2021 4:23:02pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "RBDPluginLookAndFeel.h"

PluginLookAndFeel::PluginLookAndFeel()
{
    // TODO: What's the difference from loading the image from file vs embedding it?
    // TODO: Kadenze image file size was smaller. Does it matter?
    //  Should we compress it more?

    // Store image assets
    mKnobImage = ImageCache::getFromMemory (BinaryData::RBD_Knob_80_png,
                                              BinaryData::RBD_Knob_80_pngSize);

    // Label colours
    setColour (Label::backgroundColourId, Colour (0x00000000));
    setColour (Label::textColourId, RBD::textNormalColour);
    setColour (Label::outlineColourId, RBD::controlOutlineColour);

    // ComboBox colours
    setColour (ComboBox::backgroundColourId, RBD::controlNormalColour);
    setColour (ComboBox::outlineColourId, RBD::controlOutlineColour);
    setColour (ComboBox::arrowColourId, RBD::textNormalColour);
    setColour (ComboBox::textColourId, RBD::textNormalColour);

    // Popup colours (for comboBox popup)
    setColour (PopupMenu::backgroundColourId, RBD::popupBgColour);

    // TextButton colours
    setColour (TextButton::buttonColourId, RBD::textNormalColour);
    setColour (TextButton::textColourOnId, RBD::textNormalColour);
    setColour (TextButton::textColourOffId, RBD::textNormalColour);
}

PluginLookAndFeel::~PluginLookAndFeel()
{

}

//= LABELS =====================================================================

void PluginLookAndFeel::drawLabel (Graphics& g, Label& label)
{
    // MARK: Not handling editable labels
    
    auto alpha = label.isEnabled() ? 1.0f : 0.5f;
    const Font font (getLabelFont (label));

    g.setColour (label.findColour (Label::textColourId).withMultipliedAlpha (alpha));
    g.setFont (font);

    const auto textBounds = label.getBorderSize()
                                 .subtractedFrom (label.getLocalBounds());

    int maxNumberOfLines
    = jmax (1, static_cast<int> (static_cast<float> (textBounds.getHeight())
                                 / font.getHeight()));

    g.drawFittedText (label.getText(), textBounds, label.getJustificationType(),                        maxNumberOfLines, 1.0f /* don't squeeze my text! */);
}

//= BUTTONS ====================================================================

Font PluginLookAndFeel::getTextButtonFont (TextButton& /*button*/,
                                           int /*buttonHeight*/)
{
    return RBD::mainFont;
}

void PluginLookAndFeel::drawButtonBackground (Graphics& g, Button& button,
                                              const Colour& /*backgroundColour*/,
                                              bool shouldDrawButtonAsHighlighted,
                                              bool shouldDrawButtonAsDown)
{
    Colour fillColour = RBD::controlNormalColour;

    if (shouldDrawButtonAsDown)
    {
        fillColour = RBD::controlActiveColour;
    }
    else if (shouldDrawButtonAsHighlighted)
    {
        fillColour = RBD::controlHoverColour;
    }

    const float cornerSize = 6.0f;

    #warning Non-namespaced Rectangle might be a problem on Windows
    const Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced (0.5f);

    g.setColour (fillColour);
    g.fillRoundedRectangle (bounds.reduced (1.0f), cornerSize);
}

//= POPUP MENUS ================================================================

void PluginLookAndFeel::drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                                           bool /*isSeparator*/, bool /*isActive*/,
                                           bool isHighlighted, bool isTicked,
                                           bool /*hasSubMenu*/, const String& text,
                                           const String& /*shortcutKeyText*/,
                                           const Drawable* /*icon*/,
                                           const Colour* /*passedTextColour*/)
{
    Rectangle<int> bounds (area);       // create editable copy of passed bounds to adjust them
    bounds.removeFromBottom (1);
    Colour fillColour = isHighlighted ? RBD::controlActiveColour
                                      : RBD::popupItemBgColour;

    g.setColour (fillColour);
    g.fillRect (bounds);

    Colour textColour = isTicked ? RBD::textActiveColour : RBD::textNormalColour;
    g.setColour (textColour);
    g.setFont (RBD::mainFont);

    // TODO: Check the bounds for the text - do they make sense?
    bounds.setLeft (10);
    bounds.setY (1);

    g.drawFittedText (text, bounds, Justification::left, 1);
}

//= COMBOBOXES =================================================================

Font PluginLookAndFeel::getComboBoxFont (ComboBox& /*comboBox*/)
{
    return RBD::mainFont;
}

// TODO: Use passed button data when drawing ComboBox
//  We will probably need to set the button dimensions and position when
//  creating the ComboBox
void PluginLookAndFeel::drawComboBox (Graphics& g, int width, int height,
                                      bool /*isButtonDown*/,
                                      int /*buttonX*/, int /*buttonY*/,
                                      int /*buttonW*/, int /*buttonH*/,
                                      ComboBox& comboBox)
{
    const Rectangle<float> comboBoxBounds (0.0f, 0.0f, width, height);

    // TODO: Change highlight condition to mouseOver
    const Colour comboBoxColour = comboBox.isPopupActive()
                                    ? RBD::controlHoverColour
                                    : RBD::controlNormalColour;
    g.setColour (comboBoxColour);
    g.fillRoundedRectangle (comboBoxBounds, RBD::defaultCornerSize);

    const Rectangle<float> arrowBounds (comboBoxBounds.withWidth (20.0f)
                                            .withRightX (width - 10.0f));

    // Create arrow shape
    Path arrow;
    arrow.startNewSubPath (arrowBounds.getX() + 3.0f,
                           arrowBounds.getCentreY() - 2.0f);
    arrow.lineTo (arrowBounds.getCentreX(), arrowBounds.getCentreY() + 2.0f);
    arrow.lineTo (arrowBounds.getRight() - 3.0f, arrowBounds.getCentreY() - 2.0f);


    const Colour arrowColour
    = comboBox.findColour (ComboBox::arrowColourId)
                          .withAlpha (comboBox.isPopupActive() ? 0.9f : 0.5f);
    g.setColour (arrowColour);
    g.strokePath (arrow, PathStrokeType (2.0f));
}

//= SLIDERS ====================================================================

void PluginLookAndFeel::drawRotarySlider (Graphics& g,
                                          int x, int y, int width, int height,
                                          float sliderPosProportional,
                                          float /*rotaryStartAngle*/,
                                          float /*rotaryEndAngle*/,
                                          Slider& /*slider*/)
{
    const int numFrames = mKnobImage.getHeight() / mKnobImage.getWidth();
    const int frameIndex = static_cast<int> (std::ceil (sliderPosProportional * (numFrames - 1)));

    const int diameter = jmin (width, height);
    auto bounds = Rectangle<int> (x, y, width, height).withSizeKeepingCentre (diameter, diameter);

    g.setOpacity (1.0f);    // Make sure the image is drawn opaque
    g.drawImage (mKnobImage,  // Image
                 bounds.getX(), // Destination X
                 bounds.getY(), // Destination Y
                 bounds.getWidth(), // Destination width
                 bounds.getHeight(),    // Destination height
                 0, // Source X
                 frameIndex * mKnobImage.getWidth(),  // Source Y
                 mKnobImage.getWidth(),   // Source width
                 mKnobImage.getWidth());  // Source height
}


