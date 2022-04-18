#include "AudioPluginProcessorEditor.h"

#include "AudioPluginProcessor.h"

AudioPluginProcessorEditor::AudioPluginProcessorEditor(AudioPluginProcessor& p)
    : AudioProcessorEditor(&p) {
  this->setSize(this->width, this->height);
  this->setResizable(false, false);
}

void AudioPluginProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(this->backgroundColor);
}

void AudioPluginProcessorEditor::resized() {
  juce::Rectangle<int> wholeBound = this->getLocalBounds();

  // Slice margins from the whole bound
  wholeBound.removeFromLeft(this->marginUnit);
  wholeBound.removeFromRight(this->marginUnit);
  wholeBound.removeFromTop(this->marginUnit);
  wholeBound.removeFromBottom(this->marginUnit);
}
