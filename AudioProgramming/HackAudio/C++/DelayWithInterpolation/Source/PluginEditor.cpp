/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayWithInterpolationAudioProcessorEditor::DelayWithInterpolationAudioProcessorEditor (DelayWithInterpolationAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
        
        auto parameterTree = audioProcessor.getParameters();
        delayKnobSlider.setRange(0.01f, 2.0f);
        delayKnobSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        delayKnobSlider.setColour(juce::Slider::thumbColourId,
                                      juce::Colour::fromRGBA(0, 0, 0, 0)); // fully transparent
        delayKnobSlider.setColour(juce::Slider::rotarySliderFillColourId,
                                      juce::Colour::fromRGB(199,170,96));
        delayKnobSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
        delayKnobParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(0);
        addAndMakeVisible(delayKnobSlider);
        delayKnobSlider.setValue(1.5);
        
        delayKnobSlider.onValueChange = [this] ()
        {
            *delayKnobParameter = delayKnobSlider.getValue();
        };
        delayKnobSlider.setNumDecimalPlacesToDisplay(2);
}

DelayWithInterpolationAudioProcessorEditor::~DelayWithInterpolationAudioProcessorEditor()
{
}

//==============================================================================
void DelayWithInterpolationAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("", getLocalBounds(), juce::Justification::centred, 1);
    
    juce::Colour myBackground = juce::Colour::fromRGB(20, 20, 20); // a dark bluish tone
    g.fillAll(myBackground);
}

void DelayWithInterpolationAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto width = this->getLocalBounds().getWidth();
    auto height = this->getLocalBounds().getHeight();

    auto x = width / 10;
    auto y = height / 10;
        
    delayKnobSlider.setBounds(x*3.5, 1*y, 120, 120);
    
}
