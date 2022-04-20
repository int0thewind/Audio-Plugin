//
// Created by Hanzhi Yin on 4/19/22.
//

#ifndef SOFTVELVET_VELVETNOISEFILTER_H
#define SOFTVELVET_VELVETNOISEFILTER_H

#include "../dlog.cpp"
#include "BaseAudioProcessor.cpp"

using FIRFilter = juce::dsp::FIR::Filter<float>;
using FIRCoefficient = juce::dsp::FIR::Coefficients<float>;

inline static void vnflog(juce::StringRef msg) {
  dlog("==VelvetNoiseFilter== " + msg);
}

class VelvetNoiseFilter final : public BaseAudioProcessor {
 public:
  explicit VelvetNoiseFilter(unsigned int, unsigned int, float);

  void prepareToPlay(double, int) override;

  const juce::String getName() const override;

  void releaseResources() override;

  void processBlock(juce::AudioBuffer<float>& buffer,
                    juce::MidiBuffer& midiMessages) override;

  bool setNumberOfImpulses(size_t _numberOfImpulses);
  bool setFilterLengthInMillisecond(size_t _filterLengthInMillisecond);
  bool setTargetDecayDecibel(float _targetDecayDecibel);

 private:
  void updateProcessorSpec() override;

  juce::dsp::ProcessorDuplicator<FIRFilter, FIRCoefficient> vnf;

  double savedSampleRate{};
  size_t numberOfImpulses;
  size_t filterLengthInMillisecond;
  float targetDecayDecibel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VelvetNoiseFilter)
};

#endif  // SOFTVELVET_VELVETNOISEFILTER_H
