#pragma once

#include <JuceHeader.h>

#if defined(HAVE_CHOWDSP_WDF_SINGLE_HEADER)
  // include the downloaded single header
  #include "chowdsp_wdf/chowdsp_wdf.h"
  #define TF_USE_WDF 1
#else
  #define TF_USE_WDF 0
#endif

namespace tfw
{

// A very small wrapper that exposes a consistent API: prepare, reset, processSample
class TransistorFilterWrapper
{
public:
    TransistorFilterWrapper() = default;

    void prepare (const juce::dsp::ProcessSpec& spec)
    {
#if TF_USE_WDF
        sampleRate = spec.sampleRate;
        // Example: if you want to use a simple WDF ladder constructed from primitives:
        // We can't implement a full transistor ladder here (it's large); instead, show
        // how to prepare WDF elements (resistor/capacitor) as building blocks.
        // Users can expand this to the exact ladder topology needed.
        // e.g. capacitor.prepare(sampleRate); resistor.setResistanceValue(...);
        // For now, we demonstrate the presence of the WDF header and retain it.
        // No runtime work required for this minimal example.
#else
        ladder.reset();
        ladder.prepare(spec);
#endif
    }

    void reset()
    {
#if TF_USE_WDF
        // reset WDF states if you built them (not shown in this small wrapper)
#else
        ladder.reset();
#endif
    }

    float processSample(float in)
    {
#if TF_USE_WDF
        // Minimal example: feed the input as a voltage source into a one-port
        // and read output. A real ladder wiring will be more complex.
        // Return input passthrough for now (placeholder that proves compilation).
        return in;
#else
        return ladder.processSample(in);
#endif
    }

    void setCutoff (float cutoff)
    {
#if !TF_USE_WDF
        ladder.setCutoffFrequencyHz(cutoff);
#endif
    }

    void setResonance (float resonance)
    {
#if !TF_USE_WDF
        ladder.setResonance(resonance);
#endif
    }

private:
#if TF_USE_WDF
    double sampleRate = 44100.0;
    // Add WDF nodes here when building a real ladder (e.g. chowdsp::wdft::CapacitorT<float> c1{...};)
#else
    juce::dsp::LadderFilter<float> ladder;
#endif
};

} // namespace tfw