/*
  ==============================================================================

    ParameterTags.h
    Created: 31 Aug 2022 8:18:07pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ParameterTag
{
public:
    ParameterTag (StringRef identifier, int versionHint)
        : index (tagList.size()), id (identifier), version (versionHint)
    {
        tagList.push_back (this);
    }

    const size_t index;
    const StringRef id;
    const int version;

    static const auto getListSize() { return tagList.size(); }
    static const ParameterTag& at (size_t index) { return *tagList[index]; }

private:
    inline static std::vector<ParameterTag*> tagList {};

/*  For constexpr number of parameters, declare a static const and return it in
    getListSize() method:

    static const size_t listSize;
*/

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE (ParameterTag);
};

namespace Tag
{
    // List of plugin parameters:
    inline const ParameterTag inputDrive        ("InputDrive", 1);
    inline const ParameterTag delayTime         ("Time", 1);
    inline const ParameterTag feedback          ("Feedback", 1);
    inline const ParameterTag invertFeedback    ("InvertFeedback", 1);
    inline const ParameterTag dryWet            ("Mix", 1);
    inline const ParameterTag fxType            ("Type", 1);
    inline const ParameterTag outputGain        ("OutputGain", 1);
    inline const ParameterTag modulationRate    ("ModulationRate", 1);
    inline const ParameterTag modulationDepth   ("ModulationDepth", 1);
    inline const ParameterTag stereoSpread      ("StereoSpread", 1);
}

/*  For constexpr number of parameters, count all declared parameter tags:

 constexpr size_t ParameterTag::listSize = weir::countArguments (Tag::InputDrive,
                                                                 Tag::DelayTime,
                                                                 ...);
*/
