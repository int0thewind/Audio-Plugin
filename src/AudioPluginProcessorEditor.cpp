#include "AudioPluginProcessorEditor.h"

#include "AudioPluginProcessor.h"

AudioPluginProcessorEditor::AudioPluginProcessorEditor(AudioPluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef_(p) {
  this->setSize(600, 400);
  this->setResizable(false, false);

  this->midiInfoBox = new InfoDisplayBox("Midi Info Box");
  this->audioInfoBox = new InfoDisplayBox("Audio Info Box");
}

void AudioPluginProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(this->backgroundColour);
}

void AudioPluginProcessorEditor::resized() {}
AudioPluginProcessorEditor::~AudioPluginProcessorEditor() {
  delete this->midiInfoBox;
  delete this->audioInfoBox;
}
