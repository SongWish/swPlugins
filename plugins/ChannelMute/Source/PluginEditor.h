/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ChannelMuteAudioProcessorEditor  : public gin::ProcessorEditor
{
public:
    ChannelMuteAudioProcessorEditor (ChannelMuteAudioProcessor&);
    ~ChannelMuteAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;

private:
    ChannelMuteAudioProcessor& cmProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelMuteAudioProcessorEditor)
};
