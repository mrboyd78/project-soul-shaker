#pragma once

#include "SynthSound.h"
#include "third_party/TransistorFilterWrapper.h"
#include <juce_audio_processors/juce_audio_processors.h>

class SynthVoice : public juce::SynthesiserVoice
{
public:
    explicit SynthVoice (juce::AudioProcessorValueTreeState& apvts) : apvts (apvts) {}

    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SynthSound*> (sound) != nullptr;
    }

    void setCurrentPlaybackSampleRate (double newRate) override
    {
        SynthesiserVoice::setCurrentPlaybackSampleRate (newRate);
        adsr.setSampleRate(newRate);

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = newRate;
        spec.maximumBlockSize = 512;
        spec.numChannels = 1;

        filter.prepare (spec);
    }

    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override
    {
        angle = 0.0;
        frequency = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        level = velocity;
        adsr.noteOn();
    }

    void stopNote (float, bool allowTailOff) override
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

    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (!isVoiceActive())
            return;

        auto* waveformParam = dynamic_cast<juce::AudioParameterChoice*> (apvts.getParameter ("WAVEFORM"));
        auto* cutoffParam = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("FILTER_CUTOFF"));
        auto* resonanceParam = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("FILTER_RESONANCE"));
        auto* attackParam = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("ATTACK"));
        auto* decayParam = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("DECAY"));
        auto* sustainParam = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("SUSTAIN"));
        auto* releaseParam = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("RELEASE"));

        if (!waveformParam || !cutoffParam || !resonanceParam || !attackParam || !decayParam || !sustainParam || !releaseParam)
        {
            return;
        }

        juce::ADSR::Parameters adsrParams;
        adsrParams.attack = attackParam->get();
        adsrParams.decay = decayParam->get();
        adsrParams.sustain = sustainParam->get();
        adsrParams.release = releaseParam->get();
        adsr.setParameters (adsrParams);

        filter.setCutoff (cutoffParam->get());
        filter.setResonance (resonanceParam->get());

        double angleDelta = 2.0 * juce::MathConstants<double>::pi * frequency / getSampleRate();
        int waveform = waveformParam->getIndex();

        for (int sample = startSample; sample < startSample + numSamples; ++sample)
        {
            float currentSample = 0.0f;
            if (waveform == 0) // Saw
                currentSample = 1.0f - (2.0f * static_cast<float> (angle / (2.0 * juce::MathConstants<double>::pi)));
            else // Square
                currentSample = (angle < juce::MathConstants<double>::pi) ? 1.0f : -1.0f;

            currentSample = filter.processSample (currentSample);

            float adsrSample = adsr.getNextSample();
            currentSample *= level * adsrSample;

            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample (channel, sample, currentSample);
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
    tfw::TransistorFilterWrapper filter;
    juce::ADSR adsr;

    double frequency = 0.0;
    double level = 0.0;
    double angle = 0.0;
};