/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

using namespace gin;

String enableTextFunction (const Parameter&, float v)
{
    return v == 0.0f ? "Samples" : "Time";
}

//==============================================================================
CompensatedDelayAudioProcessor::CompensatedDelayAudioProcessor()
{
    mode    = addExtParam ("mode",    "Mode",    "", "",   {   0.0f,     1.0f, 1.0f, 1.0f},    0.0f, 0.0f, enableTextFunction);
    time    = addExtParam ("time",    "Time",    "", "ms", {   0.0f,  1000.0f, 0.0f, 1.0f},    1.0f, {0.2f, SmoothingType::eased});
    samples = addExtParam ("samples", "Samples", "", "",   {   0.0f, 44100.0f, 1.0f, 1.0f},   50.0f, {0.2f, SmoothingType::eased});
    
    time->conversionFunction = [] (float in) { return in / 1000.0f; };
    
    mode->addListener (this);
    time->addListener (this);
    samples->addListener (this);
    
    updateLatency();
}

CompensatedDelayAudioProcessor::~CompensatedDelayAudioProcessor()
{
}

void CompensatedDelayAudioProcessor::updateLatency()
{
    auto sampPos = samples->getUserValueInt();
    auto timePos = time->getUserValue() / 1000.0;
    
    if (mode->getUserValueInt() == 0)
        setLatencySamples (sampPos);
    else
        setLatencySamples (roundToInt (timePos * getSampleRate()));
}

//==============================================================================
void CompensatedDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    gin::Processor::prepareToPlay (sampleRate, samplesPerBlock);
    
    int ch = getTotalNumInputChannels();
    double sr = getSampleRate();
     
    delayLine.setSize (ch, std::max (1.1, sr / 44100.0 + 0.1), sr);
    
    updateLatency();
}

void CompensatedDelayAudioProcessor::reset()
{
    gin::Processor::reset();
    
    delayLine.clear();
}

void CompensatedDelayAudioProcessor::numChannelsChanged ()
{
    int ch = getTotalNumInputChannels();
    double sr = getSampleRate();
    
    delayLine.setSize (ch, std::max (1.1, sr / 44100.0 + 0.1), sr);
}

void CompensatedDelayAudioProcessor::releaseResources()
{
}

void CompensatedDelayAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    int numSamples = buffer.getNumSamples();
    int ch = buffer.getNumChannels();
    
    ScratchBuffer scratch (ch, numSamples);
    
    for (int s = 0; s < numSamples; s++)
    {
        auto sampPos = samples->getProcValue (1);
        auto timePos = time->getProcValue (1);
        
        for (int c = 0; c < ch; c++)
        {
            if (mode->getUserValueInt() == 0)
            {
                if (sampPos == 0)
                    *scratch.getWritePointer (c, s) = *buffer.getReadPointer (c, s);
                else
                    *scratch.getWritePointer (c, s) = delayLine.readSample (c, int (sampPos));
            }
            else
            {
                if (timePos == 0)
                    *scratch.getWritePointer (c, s) = *buffer.getReadPointer (c, s);
                else
                    *scratch.getWritePointer (c, s) = delayLine.readLinear (c, timePos);
            }
        
            delayLine.write (c, *buffer.getReadPointer (c, s));
        }
        delayLine.writeFinished();
    }
    
    for (int c = 0; c < ch; c++)
        buffer.copyFrom (c, 0, scratch, c, 0, numSamples);
}

//==============================================================================
bool CompensatedDelayAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* CompensatedDelayAudioProcessor::createEditor()
{
    return new CompensatedDelayAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompensatedDelayAudioProcessor();
}
