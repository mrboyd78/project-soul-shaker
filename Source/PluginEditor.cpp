#include "PluginEditor.h"

SOULShakerAudioProcessorEditor::SOULShakerAudioProcessorEditor(SOULShakerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    addAndMakeVisible(waveformComboBox);
    waveformComboBox.addItemList({ "Sine", "Saw", "Square", "Triangle" }, 1);
    waveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.apvts, "WAVEFORM", waveformComboBox);

    auto setupSlider = [&](juce::Slider& slider) {
        addAndMakeVisible(slider);
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    };

    setupSlider(attackSlider);
    setupSlider(decaySlider);
    setupSlider(sustainSlider);
    setupSlider(releaseSlider);

    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "RELEASE", releaseSlider);

    addAndMakeVisible(shakerButton);
    shakerButton.onClick = [this] {
        processor.randomizeParameters();
    };

    setSize(400, 300);
}

SOULShakerAudioProcessorEditor::~SOULShakerAudioProcessorEditor() {}

void SOULShakerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void SOULShakerAudioProcessorEditor::resized()
{
    waveformComboBox.setBounds(10, 10, 100, 30);
    attackSlider.setBounds(10, 50, 80, 80);
    decaySlider.setBounds(100, 50, 80, 80);
    sustainSlider.setBounds(190, 50, 80, 80);
    releaseSlider.setBounds(280, 50, 80, 80);
    shakerButton.setBounds(150, 150, 100, 50);
}
