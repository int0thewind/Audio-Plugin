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

  void releaseResources() override;

  void processBlock(juce::AudioBuffer<float>& buffer,
                    juce::MidiBuffer& midiMessages) override;

  /**
   * Call this method to recreate the velvet noise filter using the current
   * parameters.
   */
  void recreateFilter();

  int getNumberOfImpulses() const;
  void setNumberOfImpulses(int _numberOfImpulses);
  int getFilterLengthInMillisecond() const;
  void setFilterLengthInMillisecond(int _filterLengthInMillisecond);
  float getTargetDecayDecibel() const;
  void setTargetDecayDecibel(float _targetDecayDecibel);

 private:
  void createFilter();

  std::unique_ptr<FIRFilter> vnf{nullptr};
  std::unique_ptr<FIRCoefficient> vnfCoefficient{nullptr};

  double sampleRate{};
  unsigned int numberOfImpulses;
  unsigned int filterLengthInMillisecond;
  float targetDecayDecibel;
  bool isDirty = true;
};

#endif  // SOFTVELVET_VELVETNOISEFILTER_H
