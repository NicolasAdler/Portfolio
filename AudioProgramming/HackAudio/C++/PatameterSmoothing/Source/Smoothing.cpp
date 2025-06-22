/*
  ==============================================================================

    Smoothing.cpp
    Created: 16 May 2025 7:01:45pm
    Author:  Nicolas Adler

  ==============================================================================
*/

#include "Smoothing.h"
#include "PluginProcessor.h"

void GainSmoothed::setSampleRate(float _sampleRate)
{
    sampleRate = _sampleRate;
    smoothedGain.reset(sampleRate, smoothingTimeInSeconds);
    smoothedGain.setCurrentAndTargetValue(*gainSliderParameter);
}

void GainSmoothed::setGain(float _gain)
{
    smoothedGain.setTargetValue(_gain);
}

void GainSmoothed::process(juce::AudioBuffer<float>& buffer)
{
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    if (smoothedGain.getTargetValue() != *gainSliderParameter)
    {
        smoothedGain.setTargetValue(*gainSliderParameter);
    }

    for (int ch = 0; ch < numChannels; ch++)
    {
        for (int s = 0; s < numSamples; s++)
        {
            float currentGainInDb = smoothedGain.getNextValue();
            float currentGain = juce::Decibels::decibelsToGain(currentGainInDb);
            buffer.setSample(ch, s, currentGain * buffer.getSample(ch, s));

        }
    }
}
