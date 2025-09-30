#include "PluginProcessor.h"
#include "PluginEditor.h"

SOULShakerAudioProcessor::SOULShakerAudioProcessor()
    : apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}
SOULShakerAudioProcessor::~SOULShakerAudioProcessor() {}

void SOULShakerAudioProcessor::prepareToPlay(double, int) {}
void SOULShakerAudioProcessor::releaseResources() {}

void SOULShakerAudioProcessor::processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) {}

juce::AudioProcessorEditor* SOULShakerAudioProcessor::createEditor() { return new SOULShakerAudioProcessorEditor(*this); }
bool SOULShakerAudioProcessor::hasEditor() const { return true; }

const juce::String SOULShakerAudioProcessor::getName() const { return "S.O.U.L. Shaker"; }
bool SOULShakerAudioProcessor::acceptsMidi() const { return true; }
bool SOULShakerAudioProcessor::producesMidi() const { return false; }
bool SOULShakerAudioProcessor::isMidiEffect() const { return false; }
double SOULShakerAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int SOULShakerAudioProcessor::getNumPrograms() { return 1; }
int SOULShakerAudioProcessor::getCurrentProgram() { return 0; }
void SOULShakerAudioProcessor::setCurrentProgram(int) {}
const juce::String SOULShakerAudioProcessor::getProgramName(int) { return {}; }
void SOULShakerAudioProcessor::changeProgramName(int, const juce::String&) {}

void SOULShakerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SOULShakerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout SOULShakerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterChoice>("STYLE", "Style",
        juce::StringArray{ "Clean", "Vintage", "Warm", "Saturated", "Driven", "Crisp", "Edgy", "Fat" }, 0));

    return layout;
}
