#pragma once

#include "AudioPluginProcessor.h"

class AudioPluginProcessorEditor : public juce::AudioProcessorEditor {
 public:
  explicit AudioPluginProcessorEditor(AudioPluginProcessor&);
  ~AudioPluginProcessorEditor() override = default;

  void paint(juce::Graphics&) override;
  void resized() override;

 private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  AudioPluginProcessor& processorRef_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginProcessorEditor)
};
