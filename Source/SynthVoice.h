#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    explicit SynthVoice(juce::AudioProcessorValueTreeState& apvts) : apvts(apvts) {}

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void setCurrentPlaybackSampleRate(double newRate) override
    {
        SynthesiserVoice::setCurrentPlaybackSampleRate(newRate);
        adsr.setSampleRate(newRate);
    }

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override
    {
        angle = 0.0;
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        level = velocity;
        adsr.noteOn();
    }

    void stopNote(float, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            adsr.noteOff();
        }
        else
        {
            clearCurrentNote();
            adsr.reset();
        }
    }

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (!isVoiceActive()) return;

        auto* waveformParam = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("WAVEFORM"));
        auto* attackParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("ATTACK"));
        auto* decayParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("DECAY"));
        auto* sustainParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("SUSTAIN"));
        auto* releaseParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("RELEASE"));

        if (waveformParam == nullptr || attackParam == nullptr || decayParam == nullptr || sustainParam == nullptr || releaseParam == nullptr)
        {
            return;
        }

        juce::ADSR::Parameters params;
        params.attack = attackParam->get();
        params.decay = decayParam->get();
        params.sustain = sustainParam->get();
        params.release = releaseParam->get();
        adsr.setParameters(params);

        double angleDelta = 2.0 * juce::MathConstants<double>::pi * frequency / getSampleRate();

        for (int sample = startSample; sample < startSample + numSamples; ++sample)
        {
            float currentSample = 0.0f;
            int currentWaveform = waveformParam->getIndex();

            switch (currentWaveform)
            {
            case 0: currentSample = static_cast<float>(std::sin(angle)); break;
            case 1: currentSample = static_cast<float>(angle / juce::MathConstants<double>::pi - 1.0); break;
            case 2: currentSample = static_cast<float>(angle < juce::MathConstants<double>::pi ? 1.0 : -1.0); break;
            case 3: currentSample = static_cast<float>(std::asin(std::sin(angle)) * (2.0 / juce::MathConstants<double>::pi)); break;
            }

            float adsrSample = adsr.getNextSample();
            currentSample *= level * adsrSample;

            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, sample, currentSample);
            }

            angle += angleDelta;
            if (angle > 2.0 * juce::MathConstants<double>::pi)
                angle -= 2.0 * juce::MathConstants<double>::pi;

            if (!adsr.isActive())
            {
                clearCurrentNote();
                angle = 0.0;
                break;
            }
        }
    }

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::ADSR adsr;

    double frequency = 0.0;
    double level = 0.0;
    double angle = 0.0;
};