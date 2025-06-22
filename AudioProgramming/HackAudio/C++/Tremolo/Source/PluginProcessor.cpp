/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LFOAudioProcessor::LFOAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    lfoFreqParameter = new juce::AudioParameterFloat(juce::ParameterID("1", 1), "LFO", 0.01f,5.0f,2.50f);
    addParameter(lfoFreqParameter);
    sinOsc.setSampleRate(getSampleRate());
    sinOsc.setFrequency(2.50f);
    float mod = 0.0f;
}

LFOAudioProcessor::~LFOAudioProcessor()
{
}

//==============================================================================
const juce::String LFOAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LFOAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LFOAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LFOAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LFOAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LFOAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LFOAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LFOAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LFOAudioProcessor::getProgramName (int index)
{
    return {};
}

void LFOAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LFOAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sinOsc.setSampleRate(getSampleRate());
    sinOsc.setFrequency(2.50f);
}

void LFOAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LFOAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LFOAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto totalNumSamples = buffer.getNumSamples();
    float floatPreVal = pre_val;
    float inc = *lfoFreqParameter / getSampleRate();
    modBuffer.setSize(totalNumInputChannels, totalNumSamples, false, true, true);
    sinOsc.setFrequency(*lfoFreqParameter);
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
        for(uint32_t i = 0; i < totalNumSamples; ++i)
        {
            float sinSample = sinOsc.process();
            for(int channel = 0; channel<totalNumOutputChannels ; channel++)
            {
                buffer.setSample(channel, i, buffer.getSample(channel, i)* sinSample);
            }
                
                
        }
     
        
}

//==============================================================================
bool LFOAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LFOAudioProcessor::createEditor()
{
    return new LFOAudioProcessorEditor (*this);
}

//==============================================================================
void LFOAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LFOAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LFOAudioProcessor();
}
