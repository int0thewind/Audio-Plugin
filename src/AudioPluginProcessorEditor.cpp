#include "AudioPluginProcessorEditor.h"

#include "AudioPluginProcessor.h"

AudioPluginProcessorEditor::AudioPluginProcessorEditor(AudioPluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef_(p) {
  this->midiInfoExportBtn = new juce::TextButton("MIDI Info Export Button");
  this->midiInfoExportBtn->setVisible(true);
  this->midiInfoExportBtn->setButtonText("Export MIDI Info");
  this->midiInfoExportBtn->addListener(this);
  this->addChildComponent(*midiInfoExportBtn);

  this->midiInfoClearBtn = new juce::TextButton("MIDI Info Clear Button");
  this->midiInfoClearBtn->setVisible(true);
  this->midiInfoClearBtn->setButtonText("Clear MIDI Info");
  this->midiInfoClearBtn->addListener(this);
  this->addChildComponent(*midiInfoClearBtn);

  this->audioInfoExportBtn = new juce::TextButton("Audio Info Export Button");
  this->audioInfoExportBtn->setVisible(true);
  this->audioInfoExportBtn->setButtonText("Export Audio Info");
  this->audioInfoExportBtn->addListener(this);
  this->addChildComponent(*audioInfoExportBtn);

  this->audioInfoClearBtn = new juce::TextButton("Audio Info Clear Button");
  this->audioInfoClearBtn->setVisible(true);
  this->audioInfoClearBtn->setButtonText("Clear Audio Info");
  this->audioInfoClearBtn->addListener(this);
  this->addChildComponent(*audioInfoClearBtn);

  this->midiInfoBox = new InfoDisplayBox("Midi Info Box");
  this->midiInfoBox->setVisible(true);
  this->addChildComponent(*midiInfoBox);

  this->audioInfoBox = new InfoDisplayBox("Audio Info Box");
  this->audioInfoBox->setVisible(true);
  this->addChildComponent(*audioInfoBox);

  this->setSize(this->width, this->height);
  this->setResizable(false, false);
}

void AudioPluginProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(this->backgroundColour);
}

void AudioPluginProcessorEditor::resized() {
  int infoDisplayBoxWidth = (this->width - this->marginUnit * 3) / 2;

  juce::Rectangle<int> bound = this->getLocalBounds();
  bound.removeFromBottom(this->marginUnit);
  bound.removeFromTop(this->marginUnit);
  bound.removeFromLeft(this->marginUnit);
  bound.removeFromRight(this->marginUnit);

  juce::Rectangle<int> midiBound = bound.removeFromLeft(infoDisplayBoxWidth);
  this->midiInfoExportBtn->setBounds(
      midiBound.removeFromBottom(this->btnHeight));
  midiBound.removeFromBottom(this->marginUnit);
  this->midiInfoClearBtn->setBounds(
      midiBound.removeFromBottom(this->btnHeight));
  this->midiInfoBox->setBounds(midiBound);

  juce::Rectangle<int> audioBound = bound.removeFromRight(infoDisplayBoxWidth);
  this->audioInfoExportBtn->setBounds(
      audioBound.removeFromBottom(this->btnHeight));
  audioBound.removeFromBottom(this->marginUnit);
  this->audioInfoClearBtn->setBounds(
      audioBound.removeFromBottom(this->btnHeight));
  this->audioInfoBox->setBounds(audioBound);
}

AudioPluginProcessorEditor::~AudioPluginProcessorEditor() {
  delete this->midiInfoBox;
  delete this->audioInfoBox;
  delete this->midiInfoExportBtn;
  delete this->midiInfoClearBtn;
  delete this->audioInfoExportBtn;
  delete this->audioInfoExportBtn;
}

void AudioPluginProcessorEditor::buttonClicked(juce::Button* button) {
  if (button == nullptr) {
    return;
  } else if (button == this->midiInfoExportBtn) {
    AudioPluginProcessorEditor::exportStringToFile(
        this->midiInfoBox->getMessages());
  } else if (button == this->audioInfoExportBtn) {
    AudioPluginProcessorEditor::exportStringToFile(
        this->audioInfoBox->getMessages());
  } else if (button == this->midiInfoClearBtn) {
    this->midiInfoBox->clearMessages();
  } else if (button == this->audioInfoClearBtn) {
    this->audioInfoBox->clearMessages();
  }
}

void AudioPluginProcessorEditor::exportStringToFile(
    const juce::String& content) {
  juce::FileChooser fileChooser = juce::FileChooser("Export");
  using Flags = juce::FileBrowserComponent::FileChooserFlags;
  int chooserFlags =
      Flags::saveMode | Flags::warnAboutOverwriting | Flags::canSelectFiles;
  auto callbackFunc = [content](const juce::FileChooser& fc) {
    juce::File file = fc.getResult();
    file.replaceWithText(content, true, true, "\n");
  };
  fileChooser.launchAsync(chooserFlags, callbackFunc);
}
