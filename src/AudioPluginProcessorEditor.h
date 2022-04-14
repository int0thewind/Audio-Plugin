#pragma once

#include "AudioPluginProcessor.h"
#include "components/InfoDisplayBox.h"

/** The GUI of the plugin. Can be considered as the front-end of the plugin. */
class AudioPluginProcessorEditor : public juce::AudioProcessorEditor,
                                   public juce::Button::Listener {
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

  void buttonClicked(juce::Button* button) override;

 private:
  juce::Colour backgroundColour{0xff323e44};

  uint16_t width = 800;

  uint16_t height = 600;

  uint8_t marginUnit = 4;

  uint8_t btnHeight = 24;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginProcessorEditor)
};
