/*
  ==============================================================================

    SaturationModule.h
    Created: 12 Mar 2021 6:09:24pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include "DspModule.h"

// TODO: Consider templating the Saturation Module class
//  If we don't need to switch the saturation curve during runtime, we can
//  template this class so that instantiations are created only for curves
//  we use. This will avoid runtime checks of the curve type.

enum class SaturationCurve
{
    /**
     Fast rational approximation of tanh(x) saturation curve.
     The most shallow curve, resulting in most level loss.
    */
    alpha,
    /**
     The simplest rational saturation surve with intermediate
     steepness and gentle wave folding (single fold only).
    */
    beta,
    /**
     The steepest saturation curve, resulting in the least level loss.
     Flat top, so no folding. Intended for output soft clipping.
    */
    gamma
};

class SaturationModule  : public DspModule
{
public:
    SaturationModule() = default;

//==============================================================================
    void setState (SaturationCurve curveToUse);
    
//==============================================================================

    void process (const float* inAudio, float* outAudio,
                  int numSamplesToRender) override;
    void reset() override;

private:
    SaturationCurve mMode = SaturationCurve::beta;

//==============================================================================
    // Saturation Curves

    float saturateAlpha (float x)
    {
        return jlimit (-1.0f, 1.0f, x * (27 + x * x) / (27 + 9 * x * x));
    }

    float saturateBeta (float x)
    {
        return 4 * x / (x * x + 4);
    }

    float saturateGamma (float x)
    {
        if (x > 1.5f)
            return 1.0f;
        else if (x < -1.5f)
            return -1.0f;
        else
            return x * (1 - x * x / 6.75f);
    }

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturationModule)
};
