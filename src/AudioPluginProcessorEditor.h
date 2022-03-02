#pragma once

#include "AudioPluginProcessor.h"
#include "InfoDisplayBox.h"

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
  ~AudioPluginProcessorEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;

  void buttonClicked(juce::Button* button) override;

  void pushMessageToMidiInfoBox(const juce::String& message);

 private:
  juce::Colour backgroundColour{0xff323e44};

  int width = 600;

  int height = 400;

  int marginUnit = 4;

  int btnHeight = 24;

  InfoDisplayBox* midiInfoBox;

  InfoDisplayBox* audioInfoBox;

  juce::TextButton* midiInfoClearBtn;

  juce::TextButton* audioInfoClearBtn;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginProcessorEditor)
};
