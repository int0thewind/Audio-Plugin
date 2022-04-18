#pragma once

#include "AudioPluginProcessor.h"

/** The GUI of the plugin. Can be considered as the front-end of the plugin. */
class AudioPluginProcessorEditor : public juce::AudioProcessorEditor {
 public:
  /**
   * The constructor of the plugin GUI.
   * A related plugin processor should be passed in, to be managed by the GUI.
   */
  explicit AudioPluginProcessorEditor(AudioPluginProcessor&);

  /** The default destructor of the GUI. */
  ~AudioPluginProcessorEditor() override = default;

  void paint(juce::Graphics&) override;
  void resized() override;

 private:
  const juce::Colour backgroundColor{0xff323e44};

  const int width = 800;

  const int height = 600;

  const int marginUnit = 8;

  //  const int btnWidth = 120;

  //  const int btnHeight = 24;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginProcessorEditor)
};
