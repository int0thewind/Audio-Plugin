//
// Created by Hanzhi Yin on 4/27/22.
//

#include "MainComponent.h"

#include <memory>

MainComponent::MainComponent() {
  // Drawable initialisation
  Path playPath;
  playPath.addTriangle(0.0, 0.0, 0.0, 100.0, 86.6f, 50.0);
  this->playButtonPath->setPath(playPath);
  this->playButtonPath->setFill(Colours::white);

  Path pausePath;
  pausePath.addRectangle(0, 0, 42, 100);
  pausePath.addRectangle(58, 0, 42, 100);
  this->pauseButtonPath->setPath(pausePath);
  this->pauseButtonPath->setFill(Colours::white);

  // Components initialisation
  this->addAndMakeVisible(this->playPauseButton.get());
  this->playPauseButton->setVisible(true);
  this->playPauseButton->addListener(this);
  this->playPauseButton->setEnabled(false);

  this->addAndMakeVisible(this->openFileButton.get());
  this->openFileButton->setVisible(true);
  this->openFileButton->addListener(this);

  this->addAndMakeVisible(this->audioSettingButton.get());
  this->audioSettingButton->setVisible(true);
  this->audioSettingButton->addListener(this);

  this->addAndMakeVisible(this->cpuUsageLabel.get());
  this->cpuUsageLabel->setJustificationType(Justification::bottomRight);
  this->cpuUsageLabel->setVisible(true);

  this->addAndMakeVisible(this->transportStateLabel.get());
  this->transportStateLabel->setJustificationType(Justification::bottomRight);
  this->transportStateLabel->setVisible(true);

  this->processorEditor.reset(this->processor->createEditorIfNeeded());
  jassert(this->processorEditor != nullptr);
  this->addAndMakeVisible(this->processorEditor.get());
  this->processorEditor->setVisible(true);

  // Format manager initialisation
  this->formatManager->registerBasicFormats();

  // Transport
  this->updateTransportState(Stopped);
  this->transportSource->addChangeListener(this);

  // Finally, configure this
  this->setVisible(true);
  const int editorWidth = this->processorEditor->getWidth();
  const int editorHeight = this->processorEditor->getHeight();
  this->setSize(editorWidth + this->margin * 4,
                editorHeight + this->margin * 6 + this->buttonWidth);

  this->setAudioChannels(2, 2);

  // Timer registration
  this->startTimer(1000);
}

MainComponent::~MainComponent() {
  this->shutdownAudio();
  this->processor->editorBeingDeleted(this->processor->getActiveEditor());
}

void MainComponent::timerCallback() { this->updateCpuUsageLabel(); }

void MainComponent::prepareToPlay(int samplesPerBlockExpected,
                                  double sampleRate) {
  this->processor->prepareToPlay(sampleRate, samplesPerBlockExpected);
  this->transportSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::releaseResources() {
  this->processor->releaseResources();
  this->transportSource->releaseResources();
}

void MainComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo& bufferToFill) {
  if (this->readerSource == nullptr) {
    bufferToFill.clearActiveBufferRegion();
    return;
  }
  jassert(bufferToFill.startSample == 0);
  this->transportSource->getNextAudioBlock(bufferToFill);
  MidiBuffer tempMidiBuffer;
  this->processor->processBlock(*(bufferToFill.buffer), tempMidiBuffer);
}

void MainComponent::resized() {
  Rectangle<int> localBound = this->getLocalBounds();
  localBound.removeFromBottom(this->margin * 2);
  localBound.removeFromTop(this->margin * 2);
  localBound.removeFromLeft(this->margin * 2);
  localBound.removeFromRight(this->margin * 2);

  Rectangle<int> topBound =
      localBound.removeFromTop(this->buttonWidth + this->margin * 2);
  this->processorEditor->setBounds(localBound);

  topBound.removeFromLeft(this->margin * 2);
  this->playPauseButton->setBounds(topBound.removeFromLeft(this->buttonWidth));
  topBound.removeFromLeft(this->margin);

  Rectangle<int> componentBound = topBound.removeFromLeft(this->buttonWidth);

  this->openFileButton->setBounds(
      componentBound.removeFromBottom(this->buttonHeight));
  componentBound.removeFromBottom(this->margin);
  this->audioSettingButton->setBounds(
      componentBound.removeFromBottom(this->buttonHeight));
  componentBound.removeFromBottom(this->margin);
  this->cpuUsageLabel->setBounds(
      componentBound.removeFromBottom(this->buttonHeight));
  componentBound.removeFromBottom(this->margin);
  this->transportStateLabel->setBounds(
      componentBound.removeFromBottom(this->buttonHeight));
}

void MainComponent::updateTransportState(TransportState newState) {
  this->state = newState;
  this->updateTransportStateLabel();
  // Definite states. Change GUI
  if (newState == Stopped) {
    this->playPauseButton->setImages(this->playButtonPath.get());
    this->playPauseButton->setEnabled(false);
    this->openFileButton->setEnabled(true);
  } else if (newState == Playing) {
    this->playPauseButton->setImages(this->pauseButtonPath.get());
    this->playPauseButton->setEnabled(true);
    this->openFileButton->setEnabled(false);
  } else if (newState == Paused) {
    this->playPauseButton->setImages(this->playButtonPath.get());
    this->playPauseButton->setEnabled(true);
    this->openFileButton->setEnabled(true);

  }
  // Potential state. Change transport source behavior
  else if (newState == Starting) {
    this->playPauseButton->setEnabled(false);
    this->openFileButton->setEnabled(false);
    this->transportSource->start();
  } else if (newState == Pausing) {
    this->playPauseButton->setEnabled(false);
    this->openFileButton->setEnabled(false);
    this->transportSource->stop();
  } else if (newState == Loading) {
    this->playPauseButton->setEnabled(false);
    this->openFileButton->setEnabled(false);
  } else {
    jassertfalse;
  }
}

void MainComponent::buttonClicked(Button* button) {
  if (button == this->playPauseButton.get()) {
    if (this->state == Playing) {
      this->updateTransportState(Pausing);
    } else if (this->state == Paused) {
      this->updateTransportState(Starting);
    } else {
      jassertfalse;
    }
  } else if (button == this->openFileButton.get()) {
    updateTransportState(Loading);
    this->fileChooser = std::make_unique<FileChooser>(
        "Select an Audio File", File{},
        this->formatManager->getWildcardForAllFormats());

    this->fileChooser->launchAsync(
        FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
        [this](const FileChooser& fc) {
          File file = fc.getResult();
          if (file != File{}) {
            AudioFormatReader* reader =
                this->formatManager->createReaderFor(file);
            if (reader != nullptr) {
              auto* newSource = new AudioFormatReaderSource(reader, true);
              this->transportSource->setSource(newSource, 0, nullptr,
                                               reader->sampleRate);
              this->readerSource.reset(newSource);
              this->updateTransportState(Paused);
              return;
            }
          }
          this->updateTransportState(Stopped);
        });
  } else if (button == this->audioSettingButton.get()) {
    std::unique_ptr<AudioDeviceSelectorComponent> audioSettingsPanel =
        std::make_unique<AudioDeviceSelectorComponent>(
            deviceManager, 0, 2, 0, 2, true, false, true, false);
    audioSettingsPanel->setSize(500, 270);
    DialogWindow::LaunchOptions dialogWindow;
    dialogWindow.useNativeTitleBar = true;
    dialogWindow.resizable = false;
    dialogWindow.dialogTitle = "Audio Settings";
    dialogWindow.dialogBackgroundColour = Colours::black;
    dialogWindow.content.setOwned(audioSettingsPanel.release());
    dialogWindow.launchAsync();
  }
}

void MainComponent::changeListenerCallback(ChangeBroadcaster* source) {
  if (source == this->transportSource.get()) {
    if (this->transportSource->isPlaying()) {
      this->updateTransportState(Playing);
    } else {
      if (this->state == Playing) {
        // Keep looping!
        this->processor->requestToUpdateProcessorSpec();
        this->transportSource->setPosition(0.0);
        this->transportSource->start();
      } else if (this->state == Pausing) {
        this->updateTransportState(Paused);
      }
    }
  } else {
    jassertfalse;
  }
}

void MainComponent::updateCpuUsageLabel() {
  double cpuUsage = this->deviceManager.getCpuUsage();
  this->cpuUsageLabel->setText(
      this->cpuUsagePrefix + String(cpuUsage, 2) + this->cpuUsageSuffix,
      dontSendNotification);
}

void MainComponent::updateTransportStateLabel() {
  String s;
  if (this->state == Stopped)
    s = "Stopped";
  else if (this->state == Loading)
    s = "Loading";
  else if (this->state == Paused)
    s = "Paused";
  else if (this->state == Starting)
    s = "Starting";
  else if (this->state == Playing)
    s = "Playing";
  else if (this->state == Pausing)
    s = "Pausing";
  else
    jassertfalse;

#if DEBUG
  s += " DBG";
#endif
  this->transportStateLabel->setText(s, dontSendNotification);
}
