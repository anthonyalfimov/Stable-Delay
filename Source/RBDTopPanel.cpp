/*
  ==============================================================================

    RBDTopPanel.cpp
    Created: 11 Jan 2021 4:33:47pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDTopPanel.h"

TopPanel::TopPanel (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    setSize (RBD::topPanelWidth, RBD::topPanelHeight);
    setName ("MainMenu");
    
    const int height = 25;
    const int presetListWidth = 200;
    
    mPresetList = std::make_unique<ComboBox>();
    Rectangle<int> bounds
    = getLocalBounds().withSizeKeepingCentre (presetListWidth, height);
    mPresetList->setBounds (bounds);
    mPresetList->addListener (this);
    addAndMakeVisible (mPresetList.get());
    updatePresetList();
    
    const int buttonWidth = 65;
    const int buttonStartX = 10;
    
    // TODO: Repetitive button initialisation - can we make it more dry?
    //  OwnedArray of buttons and StringArray of labels?
    
    mNewPreset = std::make_unique<TextButton>();
    bounds.setWidth (buttonWidth);
    bounds.setX (buttonStartX);
    mNewPreset->setBounds (bounds);
    mNewPreset->setButtonText ("NEW");
    mNewPreset->addListener (this);
    addAndMakeVisible (mNewPreset.get());
    
    mSavePreset = std::make_unique<TextButton>();
    bounds.translate (buttonWidth, 0);
    mSavePreset->setBounds (bounds);
    mSavePreset->setButtonText ("SAVE");
    mSavePreset->addListener (this);
    addAndMakeVisible (mSavePreset.get());
    
    mSaveAsPreset = std::make_unique<TextButton>();
    bounds.translate (buttonWidth, 0);
    mSaveAsPreset->setBounds (bounds);
    mSaveAsPreset->setButtonText ("SAVE AS");
    mSaveAsPreset->addListener (this);
    addAndMakeVisible (mSaveAsPreset.get());
}

TopPanel::~TopPanel()
{
    
}

void TopPanel::paint (Graphics& g)
{
    InterfacePanel::paint (g);

    // TODO: Projucer uses a Label object for every label. Should we do that too?
    g.setFont (RBD::font2);
    g.setColour (RBD::textNormalColour);
    g.drawText ("Really Basic Delay", getLocalBounds().withTrimmedRight (10),
                Justification::centredRight);
}

void TopPanel::buttonClicked (Button* buttonThatWasPressed)
{
    PresetManager* presetManager = mProcessor.getPresetManager();
    
    if (buttonThatWasPressed == mNewPreset.get())
    {
        presetManager->createNewPreset();
        updatePresetList();
    }
    else if (buttonThatWasPressed == mSavePreset.get())
    {
        if (presetManager->isCurrentPresetSaved())
        {
            presetManager->savePreset();
        }
        else
        {
            displaySaveAsPopup();
        }
    }
    else if (buttonThatWasPressed == mSaveAsPreset.get())
    {
        displaySaveAsPopup();
    }
}
    
void TopPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == mPresetList.get())
    {
        PresetManager* presetManager = mProcessor.getPresetManager();
        const int index = mPresetList->getSelectedItemIndex();
        
        presetManager->loadPreset (index);
    }
}
    
void TopPanel::displaySaveAsPopup()
{
    PresetManager* presetManager = mProcessor.getPresetManager();
    
    const String currentPresetName = presetManager->getCurrentPresetName();
    
    AlertWindow saveAsDialogue ("Save As",
                                "Please enter a name for the preset",
                                AlertWindow::NoIcon);
    
    // FIXME: Properly position the dialogue window in relation to the plugin
    saveAsDialogue.centreAroundComponent (this, getWidth(), getHeight());
    
    saveAsDialogue.addTextEditor ("presetName", currentPresetName, "Preset name: ");
    saveAsDialogue.addButton ("Save", 0);
    saveAsDialogue.addButton ("Cancel", 1);

    #warning Jules said to always avoid modal loops in a plugin
    //  Does this apply here? Why should we avoid it? What's the alternative?
    if (saveAsDialogue.runModalLoop() == 0) // if exit code is 0
    {
        const String presetName = saveAsDialogue.getTextEditorContents ("presetName");
        presetManager->saveAsPreset (presetName);
        updatePresetList();
    }
}

void TopPanel::updatePresetList()
{
    PresetManager* presetManager = mProcessor.getPresetManager();
    
    mPresetList->clear (dontSendNotification);  // we'll be rebuilding the list, don't alert listeners
    
    const int numPresets = presetManager->getNumberOfPresets();
    
    for (int i = 0; i < numPresets; ++i)
    {
        // IDs start with 1:
        mPresetList->addItem (presetManager->getPresetName (i), i + 1);
    }

    // This also sets the comboBox selected item, if it's a valid Item Text
    mPresetList->setText (presetManager->getCurrentPresetName());
}
