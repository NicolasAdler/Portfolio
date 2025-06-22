///*
//  ==============================================================================
//
//    Delay.h
//    Created: 22 Jun 2025 1:44:45am
//    Author:  Nicolas Adler
//
//  ==============================================================================
//*/


#pragma once
#include <JuceHeader.h>
#include <vector>

class Delay
{
public:
    float sampleRate = 44100.0f;
    std::vector<float> circualrBuffer;
    std::vector<float> y;

    float delayTime = 0.0f;
    float maxDelayTime = 2.0f;
    int maxDelaySamples = 0;

    float delaySamples = 0.0f;

    int write = 0;

    juce::AudioParameterFloat* delayKnobParameter = nullptr;

    juce::LinearSmoothedValue<float> smoothedDelayTime;

    void setSampleRate(float _sampleRate);
    void setDelay(float _delayTime);
    void updateDelay();
    void process(juce::AudioBuffer<float>& buffer);
};
