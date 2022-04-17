#pragma once

#include "AudioPluginProcessor.h"
#include "components/DebugLoggerComponent.h"

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
#if DEBUG
  std::unique_ptr<juce::Button> clearLogBtn =
      std::make_unique<juce::TextButton>("Clear Log");

  std::unique_ptr<DebugLoggerComponent> loggerWithDisplay =
      std::make_unique<DebugLoggerComponent>("Logger with Display");

  const int debugWidth = 400;
#endif

  const juce::Colour backgroundColor{0xff323e44};

  const int width = 800;

  const int height = 600;

  const int marginUnit = 8;

  const int buttonWidth = 120;

  const int buttonHeight = 24;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginProcessorEditor)
};
