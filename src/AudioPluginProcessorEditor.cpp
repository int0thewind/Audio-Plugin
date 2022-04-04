#include "AudioPluginProcessorEditor.h"

#include "AudioPluginProcessor.h"

AudioPluginProcessorEditor::AudioPluginProcessorEditor(AudioPluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef_(p) {
  juce::ignoreUnused(processorRef_);
  this->setSize(400, 300);
}

void AudioPluginProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(this->backgroundColour);
}

void AudioPluginProcessorEditor::resized() {}
