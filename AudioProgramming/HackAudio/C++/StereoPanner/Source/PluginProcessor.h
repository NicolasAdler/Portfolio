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
class StereoPanner
{
    public:
    float leftAmp;
    float rightAmp;
    float panValue;
    int panType;
    juce::AudioBuffer<float> leftChannel;
    juce::AudioBuffer<float> rightChannel;
    void setPanValue(float panValue)
    {
        this->panValue = (panValue/200) + 0.5;
    }
    void setAmp(int panType)
    {
        this->panType = panType;
        switch (panType)
        {
            case 1:
                this->leftAmp = 1 - this->panValue;
                this->rightAmp = this->panValue;
                break;
            case 2:
                this->leftAmp = std::sqrt(1 - this->panValue);
                this->rightAmp = std::sqrt(this->panValue);
                break;
            case 3:
                this->leftAmp = std::sin((1 - this->panValue) * (juce::MathConstants<double>::pi / 2));
                this->rightAmp = std::sin(this->panValue * (juce::MathConstants<double>::pi / 2));
                break;
            default:
                this->leftAmp = 1 - this->panValue;
                this->rightAmp = this->panValue;
                break;
        }
    }

    float process(int channel, int sample, juce::AudioBuffer<float> buffer)
    {
        if (channel == 0)
        {
            buffer.setSample(0, sample, leftAmp * buffer.getSample(0, sample));
            return buffer.getSample(channel, sample);
        }
        else if (channel == 1)
        {
            buffer.setSample(1, sample, rightAmp * buffer.getSample(1, sample));
            return buffer.getSample(channel, sample);
        }
    }
};
class StereoPannerWithClassAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    StereoPannerWithClassAudioProcessor();
    ~StereoPannerWithClassAudioProcessor() override;

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

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoPannerWithClassAudioProcessor)
    juce::AudioParameterFloat* stereoPannerParameter;
    StereoPanner stereoPanner;
    juce::AudioParameterInt* pannerTypeParameter;
};
