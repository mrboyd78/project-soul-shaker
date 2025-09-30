#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthSound.h"

SOULShakerAudioProcessor::SOULShakerAudioProcessor()
    : apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    synth.addSound(new SynthSound());
    for (int i = 0; i < 5; ++i)
    {
        synth.addVoice(new SynthVoice(apvts));
    }
}

SOULShakerAudioProcessor::~SOULShakerAudioProcessor() {}

void SOULShakerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->setCurrentPlaybackSampleRate(sampleRate);
        }
    }
}
void SOULShakerAudioProcessor::releaseResources() {}

void SOULShakerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

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

    layout.add(std::make_unique<juce::AudioParameterChoice>("WAVEFORM", "Waveform",
        juce::StringArray{ "Sine", "Saw", "Square", "Triangle" }, 0));

    layout.add(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 1.0f, 0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 1.0f, 0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 3.0f, 0.4f));

    return layout;
}

void SOULShakerAudioProcessor::randomizeParameters()
{
    juce::Random random;

    if (auto* waveformParam = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("WAVEFORM")))
    {
        int numChoices = waveformParam->choices.size();
        if (numChoices > 0)
        {
            int randomChoice = random.nextInt(numChoices);
            waveformParam->setValueNotifyingHost((float)randomChoice / (float)(numChoices - 1));
        }
    }

    apvts.getParameter("ATTACK")->setValueNotifyingHost(random.nextFloat());
    apvts.getParameter("DECAY")->setValueNotifyingHost(random.nextFloat());
    apvts.getParameter("SUSTAIN")->setValueNotifyingHost(random.nextFloat());
    apvts.getParameter("RELEASE")->setValueNotifyingHost(random.nextFloat());
}
