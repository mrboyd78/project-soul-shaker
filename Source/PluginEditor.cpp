#include "PluginEditor.h"

SOULShakerAudioProcessorEditor::SOULShakerAudioProcessorEditor(SOULShakerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    addAndMakeVisible(styleSlider);
    styleSlider.setSliderStyle(juce::Slider::Rotary);
    styleSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    styleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "STYLE", styleSlider);

    setResizable(true, true);
    setResizeLimits(400, 300, 800, 600);
    setSize(400, 300);
}

SOULShakerAudioProcessorEditor::~SOULShakerAudioProcessorEditor() {}

void SOULShakerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void SOULShakerAudioProcessorEditor::resized()
{
    const int dialSize = juce::jmin(getWidth(), getHeight()) - 100;
    styleSlider.setBounds(getWidth() / 2 - dialSize / 2,
                          getHeight() / 2 - dialSize / 2,
                          dialSize,
                          dialSize);
}
