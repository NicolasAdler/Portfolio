/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
    Nicolas Adler

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class FourFiltersInParallel_TremoloAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FourFiltersInParallel_TremoloAudioProcessor();
    ~FourFiltersInParallel_TremoloAudioProcessor() override;

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
    void updateCoefficients();
    void resetDelays();
    void processLowpass();
    void processHighpass();
    void processBandPass();
    void processBandStop();
    void nonConstantParametricEQ();
    void allInParallel();

private:
    //==============================================================================
    juce::AudioParameterFloat* gainKnobParameter;
    juce::AudioParameterInt* frequencyKnobParameter;
    juce::AudioParameterFloat*   qKnobParameter;
    juce::AudioParameterInt* menuParameter;
    juce::AudioParameterInt* oscFreqKnobParameter;
    
    double fs = 44100.00;
    
    
    
    juce::Array<float> a1_delay, a2_delay, b1_delay, b2_delay;
    juce::AudioBuffer<float> modBuffer;
    float a0,a1,a2,b1,b2,d0,c0;
    float thetaC, d, betaTop, betaBottom, beta, gamma, mu, epsilon;
    float curretnGain;
    int currentFrequency;
    float currentQ;
    int currentOscFreq;
    int currentMenu;
    float increment_value;
    float osc_Freq;
    float pre_val;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FourFiltersInParallel_TremoloAudioProcessor)
};
