/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Oscillator
{
    public:
    Oscillator() = default;
    ~Oscillator() = default;
    Oscillator(const Oscillator& osc) = default;
    float sample = 0;
    float sampleRate = 0;
    float inc = 0;
    float mod = 0;
    float frequency = 0;
    const float pi = 3.14159265358979;
    virtual void setSampleRate(float sampleRate)
    {
        this->sampleRate = sampleRate;
    }
    virtual void setFrequency(float frequency)
    {
        this->frequency = frequency;
        this->inc = frequency / sampleRate;

    }
    virtual float process() = 0;
    virtual void stop()
    {
        mod = 0.0f;
        sample = 0.0f;
    }
};
class SineOscillator : public Oscillator
{
public:
    float process() override
    {
        if (mod >= 1.0f)
            mod = 0.0f;
        sample = sinf(2.0f * pi * mod);
        mod += inc;
        return sample;
    }
};


class LFOAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    LFOAudioProcessor();
    ~LFOAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void processTremolo(juce::AudioBuffer<float>(buffer));
    

private:
    //==============================================================================
    juce::AudioParameterFloat* lfoFreqParameter;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOAudioProcessor)
    SineOscillator sinOsc;
    juce::AudioBuffer<float> modBuffer;
    float pre_val;
    float mod;
};
