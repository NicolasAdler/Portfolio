/*
  ==============================================================================

    Smoothing.h
    Created: 16 May 2025 7:01:40pm
    Author:  Nicolas Adler

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
class GainSmoothed
{
    public:
    float gain;
    juce::AudioParameterFloat* gainSliderParameter;
    juce::SmoothedValue<float> smoothedGain;
    const float smoothingTimeInSeconds = 0.5f;
    float sampleRate;
    void setSampleRate(float _sampleRate);
    void setGain(float _gain);
    void process(juce::AudioBuffer<float>& buffer);
};
