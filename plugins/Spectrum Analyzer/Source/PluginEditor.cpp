/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : slAudioProcessorEditor (p), processor (p)
{
    addAndMakeVisible (&scopeL);
    addAndMakeVisible (&scopeR);
    
    thread.addTimeSliceClient (&scopeL);
    thread.addTimeSliceClient (&scopeR);
    thread.startThread();
    
    scopeL.setColour (drow::Spectroscope::traceColourId, Colours::white.overlaidWith (Colours::blue.withAlpha (0.3f)));
    scopeR.setColour (drow::Spectroscope::traceColourId, Colours::white.overlaidWith (Colours::yellow.withAlpha (0.3f)));
    
    for (slParameter* pp : p.getPluginParameters())
    {
        ParamComponent* pc;
        
        if (pp->getUid() == PARAM_TRIGGER_MODE || pp->getUid() == PARAM_TRIGGER_CHANNEL)
            pc = new Select (pp);
        else if (pp->isOnOff())
            pc = new Switch (pp);
        else
            pc = new Knob (pp);
        
        addAndMakeVisible (pc);
        controls.add (pc);
    }
    
    setGridSize (7, 4);
    makeResizable (getWidth(), getHeight(), 2000, 1500);
    
    for (auto pp : processor.getPluginParameters())
        pp->addListener (this);
    
    updateScope();
}

PluginEditor::~PluginEditor()
{
    for (auto pp : processor.getPluginParameters())
        pp->removeListener (this);
    
    ScopedLock sl (processor.lock);
    processor.editor = nullptr;
}

//==============================================================================

Rectangle<int> PluginEditor::getGridArea (int x, int y, int w, int h)
{
    return Rectangle<int> (getWidth() - inset - 2 * cx + x * cx, headerHeight + y * cy + inset, w * cx, h * cy);
}

void PluginEditor::resized()
{
    slAudioProcessorEditor::resized();

    scopeL.setBounds (inset, headerHeight + inset, getWidth() - 2 * cx - inset, getHeight() - headerHeight - 2 * inset);
    scopeR.setBounds (inset, headerHeight + inset, getWidth() - 2 * cx - inset, getHeight() - headerHeight - 2 * inset);
    
    componentForId (PARAM_SAMPLES_PER_PIXEL)->setBounds (getGridArea (0, 0));
    componentForId (PARAM_VERTICAL_ZOOM)->setBounds (getGridArea (0, 1));
    componentForId (PARAM_VERTICAL_OFFSET_L)->setBounds (getGridArea (0, 2));
    componentForId (PARAM_VERTICAL_OFFSET_R)->setBounds (getGridArea (0, 3));
    componentForId (PARAM_TRIGGER_CHANNEL)->setBounds (getGridArea (1, 0));
    componentForId (PARAM_TRIGGER_MODE)->setBounds (getGridArea (1, 1));
    componentForId (PARAM_TRIGGER_LEVEL)->setBounds (getGridArea (1, 2));
    componentForId (PARAM_TRIGGER_POS)->setBounds (getGridArea (1, 3));
}

void PluginEditor::updateScope()
{
}
