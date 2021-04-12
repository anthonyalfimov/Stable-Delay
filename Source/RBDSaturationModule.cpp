/*
  ==============================================================================

    RBDSaturationModule.cpp
    Created: 12 Mar 2021 6:09:24pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RBDSaturationModule.h"

SaturationModule::SaturationModule()
{

}

SaturationModule::~SaturationModule()
{

}


void SaturationModule::process (const float* inAudio, float* outAudio,
                                int numSamplesToRender)
{
    switch (mMode)
    {
        case SaturationCurve::alpha:
            for (int i = 0; i < numSamplesToRender; ++i)
                outAudio[i] = saturateAlpha (inAudio[i]);
            break;

        case SaturationCurve::beta:
            for (int i = 0; i < numSamplesToRender; ++i)
                outAudio[i] = saturateBeta (inAudio[i]);
            break;

        case SaturationCurve::gamma:
            for (int i = 0; i < numSamplesToRender; ++i)
                outAudio[i] = saturateGamma (inAudio[i]);
            break;

        default:
            jassertfalse;
            FloatVectorOperations::copy (outAudio, inAudio, numSamplesToRender);
            break;
    }
}

void SaturationModule::reset()
{

}


void SaturationModule::setState (SaturationCurve curveToUse)
{
    mMode = curveToUse;
}

