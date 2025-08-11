/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
    Nicolas Adler

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FourFiltersInParallel_TremoloAudioProcessor::FourFiltersInParallel_TremoloAudioProcessor()
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
    gainKnobParameter = new juce::AudioParameterFloat(juce::ParameterID("1", 1), "Gain Knob", -6.0f,3.0f,0);
    double fs = 48000;
    frequencyKnobParameter = new juce::AudioParameterInt(juce::ParameterID("2", 2), "Frequency Knob", 0,(float)fs/2.0,2000);
    qKnobParameter = new juce::AudioParameterFloat(juce::ParameterID("3", 3), "Q Knob", 0.5f,2.0f,1.0f);
    menuParameter = new juce::AudioParameterInt(juce::ParameterID("4", 4), "Filter Type", 1,5,1);
    oscFreqKnobParameter = new juce::AudioParameterInt(juce::ParameterID("5", 5), "Frequency Knob", 0.01f,20.0f,10.0f);
    
    addParameter(gainKnobParameter);
    addParameter(frequencyKnobParameter);
    addParameter(qKnobParameter);
    addParameter(menuParameter);
    addParameter(oscFreqKnobParameter);
    
    updateCoefficients();
    
    curretnGain = *gainKnobParameter;
    currentFrequency = *frequencyKnobParameter;
    currentQ = *qKnobParameter;
    currentMenu = *menuParameter;
    currentOscFreq = *oscFreqKnobParameter;
    
    auto numInputChannels = getTotalNumInputChannels();
    for (int i = 0; i < numInputChannels; i++) {
        a1_delay.add(0.0f);
        a2_delay.add(0.0f);
        b1_delay.add(0.0f);
        b2_delay.add(0.0f);
    }
}

FourFiltersInParallel_TremoloAudioProcessor::~FourFiltersInParallel_TremoloAudioProcessor()
{
}

//==============================================================================
const juce::String FourFiltersInParallel_TremoloAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FourFiltersInParallel_TremoloAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FourFiltersInParallel_TremoloAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FourFiltersInParallel_TremoloAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FourFiltersInParallel_TremoloAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FourFiltersInParallel_TremoloAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FourFiltersInParallel_TremoloAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FourFiltersInParallel_TremoloAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FourFiltersInParallel_TremoloAudioProcessor::getProgramName (int index)
{
    return {};
}

void FourFiltersInParallel_TremoloAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FourFiltersInParallel_TremoloAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    updateCoefficients();
    resetDelays();
}

void FourFiltersInParallel_TremoloAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FourFiltersInParallel_TremoloAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FourFiltersInParallel_TremoloAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto totalNumSamples = buffer.getNumSamples();
    
    modBuffer.setSize(totalNumInputChannels, totalNumSamples, false, true, true);
    updateCoefficients();
    
    if(curretnGain != *gainKnobParameter || currentFrequency != *frequencyKnobParameter || currentQ != *qKnobParameter || currentMenu != *menuParameter || currentOscFreq != *oscFreqKnobParameter){
        updateCoefficients();
        curretnGain = *gainKnobParameter;
        currentFrequency = *frequencyKnobParameter;
        currentQ = *qKnobParameter;
        currentMenu = *menuParameter;
        currentOscFreq = *oscFreqKnobParameter;
        increment_value = currentOscFreq / fs;
    }

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        auto* modData = modBuffer.getWritePointer(channel);
        float floatPreVal = pre_val;
        for(int i = 0; i<totalNumSamples; i++) {
            float z = (a0*channelData[i]) + (a1*a1_delay.getUnchecked(channel)) + (a2*a2_delay.getUnchecked(channel)) - (b1*b1_delay.getUnchecked(channel)) - (a2*a2_delay.getUnchecked(channel));
            modData[i] = sin(2.0 * juce::MathConstants<double>::pi * floatPreVal);
            floatPreVal += increment_value;
            if (floatPreVal >= 1.0) {
                floatPreVal = 0.0;
            }
            float c = channelData[i];
            b2_delay.set(channel, b1_delay[channel]);
            b1_delay.set(channel, channelData[i]);
            a2_delay.set(channel, a1_delay[channel]);
            a1_delay.set(channel, channelData[i]);
            
            channelData[i] = (c * d0 + z * c0)*modData[i];

        }
        if (channel == 0) {
            pre_val = floatPreVal;
        }
    }
}

//==============================================================================
bool FourFiltersInParallel_TremoloAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FourFiltersInParallel_TremoloAudioProcessor::createEditor()
{
    return new FourFiltersInParallel_TremoloAudioProcessorEditor (*this);
}

//==============================================================================
void FourFiltersInParallel_TremoloAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FourFiltersInParallel_TremoloAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FourFiltersInParallel_TremoloAudioProcessor();
}


void FourFiltersInParallel_TremoloAudioProcessor::updateCoefficients() {
//    float gainKnobvalue = *gainKnobParameter;
//    int frequerncyKnobValue = *frequencyKnobParameter;
//    float qKnobValue = *qKnobParameter;
    int menuValue = *menuParameter;
    if(curretnGain != *gainKnobParameter || currentFrequency != *frequencyKnobParameter || currentQ != *qKnobParameter || currentMenu != *menuParameter || currentOscFreq != *oscFreqKnobParameter){
        curretnGain = *gainKnobParameter;
        currentFrequency = *frequencyKnobParameter;
        currentQ = *qKnobParameter;
        currentMenu = *menuParameter;
        currentOscFreq = *oscFreqKnobParameter;
        increment_value = currentOscFreq / fs;
    }
    
    if(menuValue == 1){
        processLowpass();
    }
    else if(menuValue == 2){
        processHighpass();
    }
    else if(menuValue == 3){
        processBandPass();
        
    }
    else if(menuValue == 4){
        processBandStop();
        
    }
    else if(menuValue == 5){
        nonConstantParametricEQ();
    }
    else if(menuValue == 6){
        allInParallel();
    }
    else{
        menuValue = 1;
    }
}

void FourFiltersInParallel_TremoloAudioProcessor::processLowpass(){
    float gainKnobvalue = *gainKnobParameter;
    int frequerncyKnobValue = *frequencyKnobParameter;
    float qKnobValue = *qKnobParameter;
    
    thetaC = (2.0*M_PI*frequerncyKnobValue)/fs;
    d  = 1/qKnobValue;
    betaTop = (1-((d/2.0)*sin(thetaC)));
    mu = pow(10,gainKnobvalue/20.0);
    betaBottom =(1+((d/2.0)*sin(thetaC)));
    beta = 0.5 * (betaTop/betaBottom);
    gamma = (0.5 + beta)*cos(thetaC);
    a0 = (0.5+beta-gamma)/2.0;
    a1 = (0.5+beta-gamma);
    a2 = (0.5+beta-gamma)/2.0;
    b1 = -2*gamma;
    b2 = 2*beta;
    c0 = mu - 1.0;
    d0 = 1.0;
    
}
void FourFiltersInParallel_TremoloAudioProcessor::processHighpass() {
    float gainKnobvalue = *gainKnobParameter;
    int frequerncyKnobValue = *frequencyKnobParameter;
    float qKnobValue = *qKnobParameter;
    thetaC = (2.0*M_PI*frequerncyKnobValue)/fs;
    d  = 1/qKnobValue;
    mu = pow(10,gainKnobvalue/20.0);
    betaTop = (1-((d/2)*sin(thetaC)));
    betaBottom =(1+((d/2)*sin(thetaC)));
    beta = 0.5 * (betaTop/betaBottom);
    gamma = (0.5 + beta)*cos(thetaC);
    a0 = (0.5+beta+gamma)/2.0;
    a1 = -1.0*(0.5+beta+gamma);
    a2 = (0.5+beta+gamma)/2.0;
    b1 = -2*gamma;
    b2 = 2*beta;
    c0 = mu - 1.0;
    d0 = 1.0;
}

void FourFiltersInParallel_TremoloAudioProcessor::processBandPass() {
    float gainKnobvalue = *gainKnobParameter;
    int frequerncyKnobValue = *frequencyKnobParameter;
    float qKnobValue = *qKnobParameter;
    thetaC = (2.0*M_PI*frequerncyKnobValue)/fs;
    mu = pow(10,gainKnobvalue/20.0);
    betaTop = (1.0-tan(thetaC/(2.0*qKnobValue)));
    betaBottom =(1.0+tan(thetaC/(2.0*qKnobValue)));
    beta = 0.5 * (betaTop/betaBottom);
    gamma = (0.5+beta)*cos(thetaC);
    a0 = 0.5-beta;
    a1 = 0.0;
    a2 = -1.0*(0.5-beta);
    b1 = -2*gamma;
    b2 = 2*beta;
    c0 = mu - 1.0;
    d0 = 1.0;
}
void FourFiltersInParallel_TremoloAudioProcessor::processBandStop() {
    float gainKnobvalue = *gainKnobParameter;
    int frequerncyKnobValue = *frequencyKnobParameter;
    float qKnobValue = *qKnobParameter;
    thetaC = (2.0*M_PI*frequerncyKnobValue)/fs;
    mu = pow(10,gainKnobvalue/20.0);
    betaTop = (1.0-tan(thetaC/(2.0*qKnobValue)));
    betaBottom =(1.0+tan(thetaC/(2.0*qKnobValue)));
    beta = 0.5 * (betaTop/betaBottom);
    gamma = (0.5+beta)*cos(thetaC);
    a0 = 0.5+beta;
    a1 = -2.0*gamma;
    a2 = 0.5+beta;
    b1 = -2.0*gamma;
    b2 = 2.0*beta;
    c0 = mu - 1.0;
    d0 = 1.0;
    
}
void FourFiltersInParallel_TremoloAudioProcessor::nonConstantParametricEQ() {
    float gainKnobvalue = *gainKnobParameter;
    int frequerncyKnobValue = *frequencyKnobParameter;
    float qKnobValue = *qKnobParameter;
    thetaC = (2.0*M_PI*frequerncyKnobValue)/fs;
    mu = pow(10,gainKnobvalue/20.0);
    epsilon = 4.0/(1+mu);
    beta = 0.5 * (1-epsilon*tan(thetaC/2*qKnobValue)) * (1/((1+epsilon*tan(thetaC/2*qKnobValue))));
    gamma = (0.5+beta)*cos(thetaC);
    a0 = 0.5-beta;
    a1 = 0.0;
    a2 = -(0.5-beta);
    b1 = -2*gamma;
    b2 = 2*beta;
    c0 = mu - 1.0;
    d0 = 1.0;
    
}
void FourFiltersInParallel_TremoloAudioProcessor::allInParallel() {
    float a0sum = 0.0f, a1sum = 0.0f, a2sum = 0.0f, b1sum = 0.0f, b2sum = 0.0f, c0sum = 0.0f;

    processLowpass();
    a0sum += a0; a1sum += a1; a2sum += a2;
    b1sum += b1; b2sum += b2; c0sum += c0;

    processHighpass();
    a0sum += a0; a1sum += a1; a2sum += a2;
    b1sum += b1; b2sum += b2; c0sum += c0;

    processBandPass();
    a0sum += a0; a1sum += a1; a2sum += a2;
    b1sum += b1; b2sum += b2; c0sum += c0;

    processBandStop();
    a0sum += a0; a1sum += a1; a2sum += a2;
    b1sum += b1; b2sum += b2; c0sum += c0;
    // By adding all the coefficent values together, after

    a0 = a0sum; a1 = a1sum; a2 = a2sum;
    b1 = b1sum; b2 = b2sum; c0 = c0sum;
}

void FourFiltersInParallel_TremoloAudioProcessor::resetDelays() {
    auto numInputChannels = getTotalNumInputChannels();
    for (int i = 0; i < numInputChannels; i++) {
        a1_delay.set(i, 0.0f);
        a2_delay.set(i, 0.0f);
        b1_delay.set(i, 0.0f);
        b2_delay.set(i, 0.0f);
    }
    
}
