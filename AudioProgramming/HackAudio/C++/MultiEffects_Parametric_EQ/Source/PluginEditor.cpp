/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.
    Nicolas Adler

  ==============================================================================
*/


#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FourFiltersInParallel_TremoloAudioProcessorEditor::FourFiltersInParallel_TremoloAudioProcessorEditor (FourFiltersInParallel_TremoloAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    setResizable(true, true);
    setResizeLimits(400, 200, 1200, 800);
    setSize(400, 200);
    
    addAndMakeVisible(frequencyKnob);
    addAndMakeVisible(frequencyKnobLabel);
    addAndMakeVisible(gainKnob);
    addAndMakeVisible(gainKnobLabel);
    addAndMakeVisible(qKnob);
    addAndMakeVisible(qKnobLabel);
    addAndMakeVisible(menu);
    addAndMakeVisible(oscFreqKnob);
    addAndMakeVisible(oscLabel);
    
    
    menu.setTextWhenNothingSelected("Low Pass");
    menu.addItem("Low Pass", 1);
    menu.addItem("High Pass", 2);
    menu.addItem("Band Pass", 3);
    menu.addItem("Band Stop", 4);
    menu.addItem("Parametric EQ", 5);
    menu.addItem("All in Parallel", 6);
    

    
    gainKnobLabel.setText("Gain", juce::dontSendNotification);
    gainKnobLabel.attachToComponent(&gainKnob, false);
    gainKnobLabel.setJustificationType(juce::Justification::centred);
    gainKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainKnob.setRange(-6.0f,3.0f,0.1);
    gainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    gainKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::lightblue);
    gainKnob.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::lightblue);

    
    frequencyKnobLabel.setText("Cut Frequency", juce::dontSendNotification);
    frequencyKnobLabel.attachToComponent(&frequencyKnob, false);
    frequencyKnobLabel.setJustificationType(juce::Justification::centred);
    frequencyKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    frequencyKnob.setRange(20.0f,24000.0f,10);
    frequencyKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    frequencyKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::lightblue);
    frequencyKnob.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::lightblue);
    
    qKnobLabel.setText("Q-Factor", juce::dontSendNotification);
    qKnobLabel.attachToComponent(&qKnob, false);
    qKnobLabel.setJustificationType(juce::Justification::centred);
    qKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    qKnob.setRange(0.01f,1.0f,0.01f);
    qKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    qKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::lightblue);
    qKnob.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::lightblue);
    
    oscLabel.setText("Tremolo", juce::dontSendNotification);
    oscLabel.attachToComponent(&oscFreqKnob, false);
    oscLabel.setJustificationType(juce::Justification::centred);
    oscFreqKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    oscFreqKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 32);
    oscFreqKnob.setRange(0.02f,20.0f,0.01f);
    oscFreqKnob.setNumDecimalPlacesToDisplay(2);
    oscFreqKnob.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::lightblue);
    oscFreqKnob.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::lightblue);
    
    auto parameterTree = audioProcessor.getParameters();
    
    gainKnobParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(0);
    frequencyKnobParameter = (juce::AudioParameterInt*)parameterTree.getUnchecked(1);
    qKnobParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(2);
    menuParameter = (juce::AudioParameterInt*)parameterTree.getUnchecked(3);
    oscFreqKnobParameter = (juce::AudioParameterInt*)parameterTree.getUnchecked(4);
    
    gainKnob.setRange(gainKnobParameter->range.start,gainKnobParameter->range.end,gainKnobParameter->range.interval);
    auto frequencyKnobRange =frequencyKnobParameter->getRange();
    frequencyKnob.setRange(frequencyKnobRange.getStart(),frequencyKnobRange.getEnd(),1);
    qKnob.setRange(qKnobParameter->range.start,qKnobParameter->range.end,qKnobParameter->range.interval);
    auto oscKnobRange =oscFreqKnobParameter->getRange();
    oscFreqKnob.setRange(oscKnobRange.getStart(),oscKnobRange.getEnd(),0);
    oscFreqKnob.setValue(1.0, juce::NotificationType::dontSendNotification);
    
    gainKnob.setValue(0,juce::NotificationType::dontSendNotification);
    frequencyKnob.setValue(2500,juce::NotificationType::dontSendNotification);
    qKnob.setValue(0.8f,juce::NotificationType::dontSendNotification);
    oscFreqKnob.setValue(5.7f, juce::NotificationType::dontSendNotification);

    gainKnob.onValueChange = [this] {
        *gainKnobParameter = gainKnob.getValue();
    };
    frequencyKnob.onValueChange = [this] {
        *frequencyKnobParameter = frequencyKnob.getValue();
    };
    qKnob.onValueChange = [this] {
        *qKnobParameter = qKnob.getValue();
    };
    menu.onChange = [this] () {
        *menuParameter = menu.getSelectedId();
    };
    oscFreqKnob.onValueChange = [this] {
        *oscFreqKnobParameter = oscFreqKnob.getValue();
    };
    frequencyKnob.setLookAndFeel(&knobLNF);
    gainKnob.setLookAndFeel(&knobLNF);
    qKnob.setLookAndFeel(&knobLNF);
    oscFreqKnob.setLookAndFeel(&knobLNF);
    for (auto* s : { &frequencyKnob, &gainKnob, &qKnob, &oscFreqKnob })
        s->setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                                juce::MathConstants<float>::pi * 2.75f, true);

}

FourFiltersInParallel_TremoloAudioProcessorEditor::~FourFiltersInParallel_TremoloAudioProcessorEditor()
{
    frequencyKnob.setLookAndFeel(nullptr);
    gainKnob.setLookAndFeel(nullptr);
    qKnob.setLookAndFeel(nullptr);
    oscFreqKnob.setLookAndFeel(nullptr);
}

//==============================================================================
void FourFiltersInParallel_TremoloAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(10, 25, 48));
    g.setColour (juce::Colours::whitesmoke);
    g.setFont (15.0f);
    g.drawFittedText ("Filters and Tremolo", getLocalBounds(), juce::Justification::centredTop, 1);
}

void FourFiltersInParallel_TremoloAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(16);

    juce::FlexBox row;
    row.flexDirection = juce::FlexBox::Direction::row;
    row.justifyContent = juce::FlexBox::JustifyContent::spaceBetween; // equal gaps
    row.alignItems = juce::FlexBox::AlignItems::center;

    const int knob = 100;

    auto item = [&](juce::Component& c)
    {
        return juce::FlexItem(c).withWidth(knob).withHeight(knob);
    };

    row.items = { item(frequencyKnob),item(qKnob), item(gainKnob),item(oscFreqKnob) };

    // Split a top area for knobs and a bottom strip for the menu
    auto top = area.removeFromTop(fmax(140, area.getHeight() * 2 / 3));
    row.performLayout(top);

    const int menuW = 150, menuH = 50;
    menu.setBounds(area.getCentreX() - menuW / 2,
                   area.getY() + (area.getHeight() - menuH) / 2,
                   menuW, menuH);
}


