#ifndef SOFTVELVET_MAINCOMPONENT_H
#define SOFTVELVET_MAINCOMPONENT_H

#include "AudioPluginProcessor.h"
#include "JuceHeader.h"

class MainComponent final : public AudioAppComponent,
                            public Button::Listener,
                            public Timer,
                            public ChangeListener {
 public:
  void changeListenerCallback(ChangeBroadcaster *source) override;
  MainComponent();
  ~MainComponent() override;

  void timerCallback() override;
  void buttonClicked(juce::Button *button) override;

  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void releaseResources() override;
  void getNextAudioBlock(
      const juce::AudioSourceChannelInfo &bufferToFill) override;

  void resized() override;

  enum TransportState {
    Stopped,   // When no file is selected
    Loading,   // When the audio is stopped
    Starting,  // When the process is starting
    Playing,   // When it is playing
    Pausing,   // When the audio is pausing
    Paused,    // When the audio is paused
  };

 private:
  const int margin = 8;

  const int buttonWidth = 120;

  const int buttonHeight = 24;

  std::unique_ptr<DrawablePath> playButtonPath =
      std::make_unique<DrawablePath>();
  std::unique_ptr<DrawablePath> pauseButtonPath =
      std::make_unique<DrawablePath>();
  std::unique_ptr<DrawableButton> playPauseButton =
      std::make_unique<DrawableButton>("Play Pause Button",
                                       DrawableButton::ButtonStyle::ImageRaw);

  std::unique_ptr<TextButton> openFileButton =
      std::make_unique<TextButton>("Open Audio File");

  std::unique_ptr<TextButton> audioSettingButton =
      std::make_unique<TextButton>("Audio Setting");

  std::unique_ptr<Label> transportStateLabel =
      std::make_unique<Label>("Transport State Label");

  std::unique_ptr<Label> cpuUsageLabel =
      std::make_unique<Label>("CPU Usage Label");
  const String cpuUsagePrefix{"CPU Usage: "};
  const char cpuUsageSuffix{'%'};

  std::unique_ptr<AudioPluginProcessor> processor =
      std::make_unique<AudioPluginProcessor>();
  std::unique_ptr<AudioProcessorEditor> processorEditor = nullptr;

  std::unique_ptr<AudioFormatManager> formatManager =
      std::make_unique<AudioFormatManager>();
  std::unique_ptr<AudioFormatReaderSource> readerSource;
  std::unique_ptr<AudioTransportSource> transportSource =
      std::make_unique<AudioTransportSource>();
  TransportState state = Stopped;

  std::unique_ptr<FileChooser> fileChooser = nullptr;

  void updateTransportState(TransportState newState);
  void updateCpuUsageLabel();
  void updateTransportStateLabel();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

#endif  // SOFTVELVET_MAINCOMPONENT_H
