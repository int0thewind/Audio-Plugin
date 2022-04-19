//
// Created by Hanzhi Yin on 4/19/22.
//

#ifndef SOFTVELVET_BASEPROCESSOR_CPP
#define SOFTVELVET_BASEPROCESSOR_CPP

#include "juce_audio_processors/juce_audio_processors.h"

/** Abstract and pure audio processor that serves as the skeleton. */
class BaseAudioProcessor : public juce::AudioProcessor {
 public:
  BaseAudioProcessor()
      : AudioProcessor(
            BusesProperties()
                .withInput("Input", juce::AudioChannelSet::stereo())
                .withOutput("Output", juce::AudioChannelSet::stereo())) {}

  juce::AudioProcessorEditor* createEditor() override { return nullptr; }
  bool hasEditor() const override { return false; }

  const juce::String getName() const override { return {}; }
  bool acceptsMidi() const override { return false; }
  bool producesMidi() const override { return false; }
  double getTailLengthSeconds() const override { return 0; }

  int getNumPrograms() override { return 0; }
  int getCurrentProgram() override { return 0; }
  void setCurrentProgram(int) override {}
  const juce::String getProgramName(int) override { return {}; }
  void changeProgramName(int, const juce::String&) override {}

  void getStateInformation(juce::MemoryBlock&) override {}
  void setStateInformation(const void*, int) override {}

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseAudioProcessor)
};

#endif  // SOFTVELVET_BASEPROCESSOR_CPP
