/*
  ==============================================================================

    RBDCenterPanelMenuBar.cpp
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDCentrePanelMenuBar.h"
#include "RBDParameters.h"

CentrePanelMenuBar::CentrePanelMenuBar (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    setSize (RBD::centrePanelMenuBarWidth, RBD::centrePanelMenuBarHeight);
    setName ("CenterMenu");

    // MARK: Initial ComboBox update happens when attachment is created
    //  In this case, attachment is created in the ParameterComboBox constructor
    //  before any items get added to it. Therefore, the initial update doesn't
    //  succeed. For it to work, either the attachment should be created after
    //  the items are added, or it should be manually updated (which is what
    //  we do).

    mFxTypeComboBox = std::make_unique<ParameterComboBox> (mProcessor.parameters,
                                                           Parameter::FxType);
    const int width = 90;
    mFxTypeComboBox->setBounds (getWidth() - width, 0, width, getHeight());
    
    mFxTypeComboBox->addItem ("DELAY", static_cast<int> (FxTypeID::Delay));
    mFxTypeComboBox->addItem ("CHORUS", static_cast<int> (FxTypeID::Chorus));

    // TODO: Why use getParameters() if we have access to state ValueTree?
    auto& parameters = mProcessor.getParameters();

#warning Calling getValue() returns a normalised 0to1 value!
    const auto selectedTypeID
    = floatToFxTypeID (parameters[Parameter::FxType]->getValue());
    mFxTypeComboBox->setSelectedId (static_cast<int> (selectedTypeID),
                                    dontSendNotification);
    addAndMakeVisible (mFxTypeComboBox.get());
}

CentrePanelMenuBar::~CentrePanelMenuBar()
{
    
}

void CentrePanelMenuBar::paint (Graphics& g)
{
    InterfacePanel::paint (g);

    // TODO: Projucer uses a Label object for every label. Should we do too?
    // TODO: Enable build stamp only for the debug version, or remove later

    g.setFont (RBD::font1);
    g.setColour (RBD::colour1);
    auto buildDate = Time::getCompilationDate();
    g.drawText ("Build: " + String (buildDate.getHours()) + ":"
                + String (buildDate.getMinutes()) + ", "
                + String (buildDate.getDayOfMonth()) + " "
                + buildDate.getMonthName (true) + " "
                + String (buildDate.getYear()),
                getLocalBounds().withTrimmedLeft (10), Justification::centredLeft);
}

void CentrePanelMenuBar::addFxTypeComboBoxListener (ComboBox::Listener* inListener)
{
    mFxTypeComboBox->addListener (inListener);
}

void CentrePanelMenuBar::removeFxTypeComboBoxListener (ComboBox::Listener* inListener)
{
    mFxTypeComboBox->removeListener (inListener);
}
