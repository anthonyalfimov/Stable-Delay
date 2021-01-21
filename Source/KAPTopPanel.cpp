/*
  ==============================================================================

    KAPTopPanel.cpp
    Created: 11 Jan 2021 4:33:47pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPTopPanel.h"

KAPTopPanel::KAPTopPanel (KadenzeAudioPluginAudioProcessor* inProcessor)
    : KAPPanelBase (inProcessor)
{
    setSize (TOP_PANEL_WIDTH, TOP_PANEL_HEIGHT);
    setName ("MainMenu");
    
    const int height = 25;
    const int presetListWidth = 200;
    
    mPresetList = std::make_unique<ComboBox>();
    // TODO: is it a bad practice to use the same Rect object to position all UI elements?
    Rectangle<int> bounds = getLocalBounds().withSizeKeepingCentre (presetListWidth, height);
    mPresetList->setBounds (bounds);
    mPresetList->addListener (this);
    addAndMakeVisible (mPresetList.get());
    updatePresetList();
    
    const int buttonWidth = 55;
    const int buttonStartX = 15;
    
    // TODO: repetitive button initialisation - can we make it more dry? Array of buttons?
    
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

KAPTopPanel::~KAPTopPanel()
{
    
}

void KAPTopPanel::paint (Graphics& g)
{
    KAPPanelBase::paint (g);

//    g.setFont (Font ("Gill Sans", 16.0f, Font::plain));
//    g.drawFittedText ("KADENZE AUDIO PLUGIN", getBounds().withTrimmedRight (10), Justification::centredRight, 1);
}

void KAPTopPanel::buttonClicked (Button* buttonThatWasPressed)
{
    KAPPresetManager* presetManager = mProcessor->getPresetManager();
    
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
    
void KAPTopPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == mPresetList.get())
    {
        KAPPresetManager* presetManager = mProcessor->getPresetManager();
        const int index = mPresetList->getSelectedItemIndex();
        
        presetManager->loadPreset (index);
    }
}
    
void KAPTopPanel::displaySaveAsPopup()
{
    KAPPresetManager* presetManager = mProcessor->getPresetManager();
    
    const String currentPresetName = presetManager->getCurrentPresetName();
    
    AlertWindow saveAsDialogue ("Save As",
                                "Please enter a name for the preset",
                                AlertWindow::NoIcon);
    
    // FIXME: properly position the dialogue window in relation to the plugin
    saveAsDialogue.centreAroundComponent (this, getWidth(), getHeight());
    
    saveAsDialogue.addTextEditor ("presetName", currentPresetName, "Preset name: ");
    saveAsDialogue.addButton ("Save", 0);
    saveAsDialogue.addButton ("Cancel", 1);
    
    if (saveAsDialogue.runModalLoop() == 0) // if exit code is 0
    {
        const String presetName = saveAsDialogue.getTextEditorContents ("presetName");
        presetManager->saveAsPreset (presetName);
        updatePresetList();
    }
}

void KAPTopPanel::updatePresetList()
{
    KAPPresetManager* presetManager = mProcessor->getPresetManager();
    
    mPresetList->clear (dontSendNotification);  // we'll be rebuilding the list, don't alert listeners
    
    const int numPresets = presetManager->getNumberOfPresets();
    
    for (int i = 0; i < numPresets; ++i)
    {
        mPresetList->addItem (presetManager->getPresetName (i), i + 1); // IDs start with 1
    }
    
    mPresetList->setText (presetManager->getCurrentPresetName());
}
