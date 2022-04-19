//
// Created by Hanzhi Yin on 4/19/22.
//

#ifndef SOFTVELVET_VELVETNOISEFILTER_H
#define SOFTVELVET_VELVETNOISEFILTER_H

#include "../dlog.cpp"
#include "BaseAudioProcessor.cpp"
#include "juce_dsp/juce_dsp.h"

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

  /**
   * Call this method to request to recreate the velvet noise filter using the
   * current parameters.
   */
  inline void requestToRecreateFilter();

  size_t getNumberOfImpulses() const;
  void setNumberOfImpulses(size_t _numberOfImpulses);
  size_t getFilterLengthInMillisecond() const;
  void setFilterLengthInMillisecond(size_t _filterLengthInMillisecond);
  float getTargetDecayDecibel() const;
  void setTargetDecayDecibel(float _targetDecayDecibel);

 private:
  void createFilter();

  juce::dsp::ProcessorDuplicator<FIRFilter, FIRCoefficient> vnf;

  double savedSampleRate{};
  size_t numberOfImpulses;
  size_t filterLengthInMillisecond;
  float targetDecayDecibel;
  std::atomic<bool> isDirty = true;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VelvetNoiseFilter)
};

#endif  // SOFTVELVET_VELVETNOISEFILTER_H
