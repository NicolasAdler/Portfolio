/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class StereoPannerWithClassAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    StereoPannerWithClassAudioProcessorEditor (StereoPannerWithClassAudioProcessor&);
    ~StereoPannerWithClassAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoPannerWithClassAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoPannerWithClassAudioProcessorEditor)
    
    juce::AudioParameterFloat* stereoPannerParameter;
    juce::Slider stereoPannerSlider;
    juce::Label stereoPannerLabel;
    
    juce::ComboBox pannerType;
    juce::AudioParameterInt* pannerTypeParameter;
};
