///*
//  ==============================================================================
//
//    Delay.cpp
//    Created: 22 Jun 2025 1:44:56am
//    Author:  Nicolas Adler
//
//  ==============================================================================

#include "Delay.h"
#include <JuceHeader.h>

void Delay::setSampleRate(float _sampleRate)
{
    sampleRate = _sampleRate;
    maxDelayTime = 2.0f;
    maxDelaySamples = (int)(sampleRate * maxDelayTime);

    circualrBuffer.resize(maxDelaySamples, 0.0f);
    y.resize(maxDelaySamples, 0.0f);

    delayTime = *delayKnobParameter;
    delaySamples = delayTime * sampleRate;

    write = 0;

    smoothedDelayTime.reset(sampleRate, 0.05f);
    smoothedDelayTime.setCurrentAndTargetValue(delaySamples);
}

void Delay::setDelay(float _delayTime)
{
    delayTime = _delayTime;
    delaySamples = delayTime * sampleRate;
    smoothedDelayTime.setCurrentAndTargetValue(delaySamples);
}

void Delay::updateDelay()
{
    delayTime = *delayKnobParameter;
    smoothedDelayTime.setTargetValue(delayTime * sampleRate);
}

void Delay::process(juce::AudioBuffer<float>& buffer)
{
    int totalNumSamples = buffer.getNumSamples();

    if (delayTime != *delayKnobParameter)
    {
        updateDelay();
    }

    for (int s = 0; s < totalNumSamples; ++s)
    {
        // Get the next smoothed delay in samples
        float currentDelay = smoothedDelayTime.getNextValue();

        circualrBuffer[write] = buffer.getSample(0, s);

        int readIndex1 = (int)std::floor(write - currentDelay);
        int readIndex2 = readIndex1 + 1;

        if (readIndex1 < 0) readIndex1 += maxDelaySamples;
        if (readIndex2 < 0) readIndex2 += maxDelaySamples;

        readIndex1 = readIndex1 % maxDelaySamples;
        readIndex2 = readIndex2 % maxDelaySamples;

        float frac = (write - currentDelay) - std::floor(write - currentDelay);

        float sample0 = circualrBuffer[readIndex1];
        float sample1 = circualrBuffer[readIndex2];

        float interpolatedSample = sample0 * (1.0f - frac) + sample1 * frac;

        float wetSample = buffer.getSample(0, s) + 0.75f * interpolatedSample;
        buffer.setSample(0, s, wetSample);
        buffer.setSample(1, s, wetSample);

        write = (write + 1) % maxDelaySamples;
    }
}
