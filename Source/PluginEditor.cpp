#include "PluginEditor.h"

SOULShakerAudioProcessorEditor::SOULShakerAudioProcessorEditor(SOULShakerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    addAndMakeVisible(waveformComboBox);
    waveformComboBox.addItemList({ "Saw", "Square" }, 1);
    waveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.apvts, "WAVEFORM", waveformComboBox);

    auto setupSlider = [&](juce::Slider& slider) {
        addAndMakeVisible(slider);
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    };

    setupSlider(filterCutoffSlider);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "FILTER_CUTOFF", filterCutoffSlider);
    setupSlider(filterResonanceSlider);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "FILTER_RESONANCE", filterResonanceSlider);

    setResizable(true, true);
    setResizeLimits(400, 200, 800, 400);
    setSize(400, 200);
}

SOULShakerAudioProcessorEditor::~SOULShakerAudioProcessorEditor() {}

void SOULShakerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void SOULShakerAudioProcessorEditor::resized()
{
    const int padding = 10;
    const int sliderSize = 80;
    const int comboBoxHeight = 30;

    juce::Rectangle<int> area = getLocalBounds().reduced(padding);

    waveformComboBox.setBounds(area.removeFromLeft(100).withY(area.getCentreY() - comboBoxHeight / 2));
    area.removeFromLeft(padding);
    filterCutoffSlider.setBounds(area.removeFromLeft(sliderSize));
    area.removeFromLeft(padding);
    filterResonanceSlider.setBounds(area.removeFromLeft(sliderSize));
}
