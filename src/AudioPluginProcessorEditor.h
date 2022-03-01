#pragma once

#include "AudioPluginProcessor.h"
#include "InfoDisplayBox.h"

/** The GUI of the plugin. Can be considered as the front-end of the plugin. */
class AudioPluginProcessorEditor : public juce::AudioProcessorEditor {
 public:
  /**
   * The constructor of the plugin GUI.
   * A related plugin processor should be passed in, to be managed by the GUI.
   */
  explicit AudioPluginProcessorEditor(AudioPluginProcessor&);

  /** The default destructor of the GUI. */
  ~AudioPluginProcessorEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;

 private:
  /**
   * A reference of the plugin processor the GUI is managing.
   * This reference is provided as a quick way for your editor to access the
   * processor object that created it.
   */
  AudioPluginProcessor& processorRef_;

  juce::Colour backgroundColour{0xff323e44};

  int width = 600;

  int height = 400;

  InfoDisplayBox* midiInfoBox;

  InfoDisplayBox* audioInfoBox;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginProcessorEditor)
};
