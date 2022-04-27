//
// Created by Hanzhi Yin on 4/27/22.
//

#include "MainComponent.h"

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
  this->playPauseButtonUpdate();

  this->addAndMakeVisible(this->openFileButton.get());
  this->openFileButton->setVisible(true);
  this->openFileButton->addListener(this);

  this->addAndMakeVisible(this->audioSettingButton.get());
  this->audioSettingButton->setVisible(true);
  this->audioSettingButton->addListener(this);

  this->addAndMakeVisible(this->cpuUsageLabel.get());
  this->cpuUsageLabel->setJustificationType(Justification::bottomRight);
  this->cpuUsageLabel->setVisible(true);

  this->processorEditor.reset(this->processor->createEditorIfNeeded());
  jassert(this->processorEditor != nullptr);
  this->addAndMakeVisible(this->processorEditor.get());
  this->processorEditor->setVisible(true);

  // Format manager initialisation
  this->formatManager->registerBasicFormats();

  // Timer registration
  this->startTimer(1000);

  // Finally, configure this
  this->setVisible(true);
  const int editorWidth = this->processorEditor->getWidth();
  const int editorHeight = this->processorEditor->getHeight();
  this->setSize(editorWidth + this->margin * 4,
                editorHeight + this->margin * 6 + this->buttonWidth);
}

MainComponent::~MainComponent() {
  this->shutdownAudio();
  this->processor->editorBeingDeleted(this->processor->getActiveEditor());
}

void MainComponent::timerCallback() { this->updateCpuUsageLabel(); }

void MainComponent::buttonClicked(Button *button) {
  if (button == this->playPauseButton.get()) {
  } else if (button == this->openFileButton.get()) {
  } else if (button == this->audioSettingButton.get()) {
    this->openAudioSettings();
  }
}
void MainComponent::prepareToPlay(int samplesPerBlockExpected,
                                  double sampleRate) {
  this->processor->prepareToPlay(sampleRate, samplesPerBlockExpected);
}

void MainComponent::releaseResources() { this->processor->releaseResources(); }

void MainComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo &bufferToFill) {}

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
}

void MainComponent::playPauseButtonUpdate() {
  if (state.get() == Stopped) {
    this->playPauseButton->setImages(this->playButtonPath.get());
    this->playPauseButton->setEnabled(false);
  } else if (state.get() == Starting) {
    this->playPauseButton->setImages(this->playButtonPath.get());
    this->playPauseButton->setEnabled(false);
  } else if (state.get() == Playing) {
    this->playPauseButton->setImages(this->pauseButtonPath.get());
    this->playPauseButton->setEnabled(true);
  } else if (state.get() == Paused) {
    this->playPauseButton->setImages(this->pauseButtonPath.get());
    this->playPauseButton->setEnabled(true);
  } else if (state.get() == Stopping) {
    this->playPauseButton->setImages(this->pauseButtonPath.get());
    this->playPauseButton->setEnabled(false);
  }
}

void MainComponent::updateCpuUsageLabel() {
  double cpuUsage = this->deviceManager.getCpuUsage();
  this->cpuUsageLabel->setText(
      this->cpuUsagePrefix + String(cpuUsage, 2) + this->cpuUsageSuffix,
      dontSendNotification);
}

void MainComponent::openAudioSettings() {
  std::unique_ptr<AudioDeviceSelectorComponent> audioSettingsPanel =
      std::make_unique<AudioDeviceSelectorComponent>(
          this->deviceManager, 0, 2, 0, 2, true, false, true, false);
  audioSettingsPanel->setSize(500, 270);
  juce::DialogWindow::LaunchOptions dialogWindow;
  dialogWindow.useNativeTitleBar = true;
  dialogWindow.resizable = false;
  dialogWindow.dialogTitle = "Audio Settings";
  dialogWindow.dialogBackgroundColour = Colours::black;
  dialogWindow.content.setOwned(audioSettingsPanel.release());
  dialogWindow.launchAsync();
}
