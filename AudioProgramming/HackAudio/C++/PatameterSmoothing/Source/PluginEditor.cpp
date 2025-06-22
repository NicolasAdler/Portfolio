/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainWithSmoothingAudioProcessorEditor::GainWithSmoothingAudioProcessorEditor (GainWithSmoothingAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider.setRange(-25.0f, 12.0f);
    gainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::royalblue);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    gainSlider.setNumDecimalPlacesToDisplay(2);
    auto parameterTree = audioProcessor.getParameters();
    gainSliderParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(0);
    gainSlider.setValue(0.0f, juce::NotificationType::dontSendNotification);
    gainSlider.onValueChange = [this] {
        *gainSliderParameter= gainSlider.getValue();
    };
    
}

GainWithSmoothingAudioProcessorEditor::~GainWithSmoothingAudioProcessorEditor()
{
}

//==============================================================================
void GainWithSmoothingAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("", getLocalBounds(), juce::Justification::centred, 1);
}

void GainWithSmoothingAudioProcessorEditor::resized()
{
    auto x = getWidth()/10;
    auto y = getHeight()/10;
    
    gainSlider.setBounds(x*5,y*2,150,150);
    
}
