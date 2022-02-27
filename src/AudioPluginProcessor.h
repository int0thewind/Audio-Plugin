#pragma once

#include "juce_audio_processors/juce_audio_processors.h"

class AudioPluginProcessor : public juce::AudioProcessor {
 public:
  AudioPluginProcessor();
  ~AudioPluginProcessor() override;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
  using AudioProcessor::processBlock;

  /** Create a GUI of the plugin when required. */
  juce::AudioProcessorEditor* createEditor() override;
  /** We have a GUI of the plugin. Return true. */
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  /**
   * Returns the number of preset programs the processor supports.
   * NB: some hosts don't cope very well if you tell them there are 0 programs,
   * so this should be at least 1, even if you're not really implementing
   * programs.
   * @return the number of preset programs the processor supports.
   */
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginProcessor)
};
