#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"

class SOULShakerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SOULShakerAudioProcessorEditor(SOULShakerAudioProcessor&);
    ~SOULShakerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SOULShakerAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SOULShakerAudioProcessorEditor)
};
