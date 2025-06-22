/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoPannerWithClassAudioProcessorEditor::StereoPannerWithClassAudioProcessorEditor (StereoPannerWithClassAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
    addAndMakeVisible(stereoPannerLabel);
    addAndMakeVisible(stereoPannerSlider);
    stereoPannerLabel.setText("Stereo Panner", juce::dontSendNotification);
    stereoPannerLabel.attachToComponent(&stereoPannerSlider, false);
    stereoPannerSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    stereoPannerSlider.setRange(-95.0f, 95.0f);
    stereoPannerSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::royalblue);
    stereoPannerSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    stereoPannerSlider.setNumDecimalPlacesToDisplay(2);
    auto parameterTree = audioProcessor.getParameters();
    stereoPannerParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(0);
    stereoPannerSlider.setValue(0.0f, juce::NotificationType::dontSendNotification);
    stereoPannerSlider.onValueChange = [this] {
        *stereoPannerParameter= stereoPannerSlider.getValue();
    };
    
    pannerType.setTextWhenNothingSelected("Linear");
    pannerType.addItem("Linear", 1);
    pannerType.addItem("Square Root", 2);
    pannerType.addItem("Sine", 3);
    pannerTypeParameter = (juce::AudioParameterInt*)parameterTree.getUnchecked(1);
    
    addAndMakeVisible(pannerType);
    
    pannerType.onChange = [this] () {
        *pannerTypeParameter = pannerType.getSelectedId();
    };
}

StereoPannerWithClassAudioProcessorEditor::~StereoPannerWithClassAudioProcessorEditor()
{
}

//==============================================================================
void StereoPannerWithClassAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("", getLocalBounds(), juce::Justification::centred, 1);
}

void StereoPannerWithClassAudioProcessorEditor::resized()
{
    auto x = getWidth()/10;
    auto y = getHeight()/10;
    
    stereoPannerSlider.setBounds(x*5,y*2,150,150);
    pannerType.setBounds(x, 5*y, 150, 50);
}
