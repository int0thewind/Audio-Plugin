#ifndef SOFTVELVET_BASEPROCESSOR_CPP
#define SOFTVELVET_BASEPROCESSOR_CPP

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

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

  /** Call this method to request the audio processor to be updated. */
  inline void requestToUpdateProcessorSpec() { this->isDirty = true; }

 protected:
  /** Special atomic variable denoting that the processor needs to be updated.
   */
  std::atomic<bool> isDirty = true;

 private:
  virtual void updateProcessorSpec() = 0;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseAudioProcessor)
};

#endif  // SOFTVELVET_BASEPROCESSOR_CPP
