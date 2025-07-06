#include "PluginEditor.h"

SOULShakerAudioProcessorEditor::SOULShakerAudioProcessorEditor(SOULShakerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(400, 300);
}

SOULShakerAudioProcessorEditor::~SOULShakerAudioProcessorEditor() {}

void SOULShakerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText("S.O.U.L. Shaker", getLocalBounds(), juce::Justification::centred, 1);
}

void SOULShakerAudioProcessorEditor::resized() {}
