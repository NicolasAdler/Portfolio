/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.
    Nicolas Adler

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ModernKnobLNF : public juce::LookAndFeel_V4
{
public:
    ModernKnobLNF()
    {
        setColour (juce::Slider::rotarySliderFillColourId, juce::Colour(0xff9ad7e5));
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x33222a2e));
        setColour (juce::Slider::thumbColourId, juce::Colour(0xff9ad7e5));
        setColour (juce::Label::textColourId, juce::Colours::whitesmoke);
    }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider& slider) override
    {
        auto bounds   = juce::Rectangle<float>(x, y, width, height).reduced(6.0f);
        auto radius   = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;
        auto centre   = bounds.getCentre();
        auto lineW    = juce::jmax(2.0f, radius * 0.10f);
        auto r        = radius - lineW * 0.5f;

        // angles
        auto startAng = rotaryStartAngle;
        auto endAng   = rotaryEndAngle;
        auto angle    = startAng + sliderPosProportional * (endAng - startAng);

        // base ring
        g.setColour (slider.findColour(juce::Slider::rotarySliderOutlineColourId));
        g.drawEllipse (centre.x - r, centre.y - r, r * 2.0f, r * 2.0f, lineW);

        // value arc
        g.setColour (slider.findColour(juce::Slider::rotarySliderFillColourId));
        juce::Path valueArc;
        valueArc.addCentredArc (centre.x, centre.y, r, r, 0.0f, startAng, angle, true);
        g.strokePath (valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // thumb
        auto thumbRadius = lineW * 1.2f;
        auto thumbPoint  = centre.getPointOnCircumference (r, angle);
        g.setColour (slider.findColour(juce::Slider::thumbColourId));
        g.fillEllipse (thumbPoint.x - thumbRadius, thumbPoint.y - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f);

        g.setColour (juce::Colours::black.withAlpha(0.15f));
        g.drawEllipse (centre.x - (r - lineW), centre.y - (r - lineW), (r - lineW) * 2.0f, (r - lineW) * 2.0f, 1.0f);
    }

    juce::Font getLabelFont (juce::Label& l) override { return { juce::Font::getDefaultSansSerifFontName(), 16.0f, juce::Font::plain }; }

    void drawLabel (juce::Graphics& g, juce::Label& l) override
    {
        g.fillAll (juce::Colour(0x22ffffff));
        g.setColour (l.findColour(juce::Label::textColourId));
        g.setFont (getLabelFont(l));
        auto r = l.getLocalBounds().reduced(6, 4);
        g.drawFittedText (l.getText(), r, juce::Justification::centredLeft, 1);
        g.setColour (juce::Colours::white.withAlpha(0.2f));
        g.drawRoundedRectangle (l.getLocalBounds().toFloat().reduced(0.5f), 6.0f, 1.0f);
    }
};



class FourFiltersInParallel_TremoloAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FourFiltersInParallel_TremoloAudioProcessorEditor (FourFiltersInParallel_TremoloAudioProcessor&);
    ~FourFiltersInParallel_TremoloAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FourFiltersInParallel_TremoloAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FourFiltersInParallel_TremoloAudioProcessorEditor)
    ModernKnobLNF knobLNF;
    juce::Slider gainKnob;
    juce::Slider frequencyKnob;
    juce::Slider qKnob;
    juce::ComboBox menu;
    juce::Slider oscFreqKnob;
    
    juce::Label frequencyKnobLabel;
    juce::Label gainKnobLabel;
    juce::Label qKnobLabel;
    juce::Label menuLabel;
    juce::Label oscLabel;
    
    juce::AudioParameterFloat* gainKnobParameter;
    juce::AudioParameterInt* frequencyKnobParameter;
    juce::AudioParameterFloat*   qKnobParameter;
    juce::AudioParameterInt* menuParameter;
    juce::AudioParameterInt* oscFreqKnobParameter;
};
