#include "AudioPluginProcessorEditor.h"

#include "AudioPluginProcessor.h"

AudioPluginProcessorEditor::AudioPluginProcessorEditor(AudioPluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef_(p) {
  this->setSize(this->width, this->height);
  this->setResizable(false, false);

  this->midiInfoExportBtn = new juce::TextButton("MIDI Info Export Button");
  this->midiInfoExportBtn->setButtonText("Export MIDI Info");
  this->midiInfoExportBtn->addListener(this);
  this->addChildComponent(*midiInfoExportBtn);

  this->audioInfoExportBtn = new juce::TextButton("Audio Info Export Button");
  this->audioInfoExportBtn->setButtonText("Export Audio Info");
  this->audioInfoExportBtn->addListener(this);
  this->addChildComponent(*audioInfoExportBtn);

  this->midiInfoBox = new InfoDisplayBox("Midi Info Box");
  this->midiInfoBox->setVisible(true);
  this->addChildComponent(*midiInfoBox);

  this->audioInfoBox = new InfoDisplayBox("Audio Info Box");
  this->audioInfoBox->setVisible(true);
  this->addChildComponent(*audioInfoBox);
}

void AudioPluginProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(this->backgroundColour);
}

void AudioPluginProcessorEditor::resized() {
  auto bound = this->getLocalBounds();
}

AudioPluginProcessorEditor::~AudioPluginProcessorEditor() {
  delete this->midiInfoBox;
  delete this->audioInfoBox;
  delete this->midiInfoExportBtn;
  delete this->audioInfoExportBtn;
}

void AudioPluginProcessorEditor::buttonClicked(juce::Button* button) {
  if (button == nullptr) {
    return;
  } else if (button == this->midiInfoExportBtn) {
    // NOP
  } else if (button == this->audioInfoExportBtn) {
    // NOP
  }
}
