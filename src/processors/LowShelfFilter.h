//
// Created by Hanzhi Yin on 4/19/22.
//

#ifndef SOFTVELVET_LOWSHELFFILTER_H
#define SOFTVELVET_LOWSHELFFILTER_H

#include "../dlog.cpp"
#include "BaseAudioProcessor.cpp"

using IIRFilter = juce::dsp::IIR::Filter<float>;
using IIRCoefficient = juce::dsp::IIR::Coefficients<float>;

class LowShelfFilter final : public BaseAudioProcessor {
 public:
  explicit LowShelfFilter(float, float);
  const juce::String getName() const override;
  void prepareToPlay(double sampleRate,
                     int maximumExpectedSamplesPerBlock) override;
  void releaseResources() override;
  void processBlock(juce::AudioBuffer<float>& buffer,
                    juce::MidiBuffer& midiMessages) override;
  bool setCutoffFreq(float cutoffFreq);
  bool setAttenuationDecibel(float _attenuationDecibel);

 private:
  void updateProcessorSpec() override;

  juce::dsp::ProcessorDuplicator<IIRFilter, IIRCoefficient> lowShelf;

  double savedSampleRate{};
  float cutoffFreq;
  float attenuationDecibel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LowShelfFilter)
};

#endif  // SOFTVELVET_LOWSHELFFILTER_H
