/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LFOAudioProcessorEditor::LFOAudioProcessorEditor (LFOAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
    addAndMakeVisible(lfoFreqLabel);
    addAndMakeVisible(lfoFreqSlider);
    lfoFreqLabel.setText("LFO 13", juce::dontSendNotification);
    lfoFreqLabel.attachToComponent(&lfoFreqSlider, false);
    lfoFreqSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lfoFreqSlider.setRange(0.01f, 5.0f);
    lfoFreqSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::royalblue);
    lfoFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    lfoFreqSlider.setNumDecimalPlacesToDisplay(2);
    auto parameterTree = audioProcessor.getParameters();
    lfoFreqParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(0);
    lfoFreqSlider.setValue(2.50f, juce::NotificationType::dontSendNotification);
    lfoFreqSlider.onValueChange = [this] {
        *lfoFreqParameter= lfoFreqSlider.getValue();
    };
}

LFOAudioProcessorEditor::~LFOAudioProcessorEditor()
{
}

//==============================================================================
void LFOAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("", getLocalBounds(), juce::Justification::centred, 1);
}

void LFOAudioProcessorEditor::resized()
{
    auto x = getWidth()/10;
    auto y = getHeight()/10;
    
    lfoFreqSlider.setBounds(x*0,y,150,150);
    
    
}
