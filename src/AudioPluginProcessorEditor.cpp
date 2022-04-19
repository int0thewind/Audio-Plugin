#include "AudioPluginProcessorEditor.h"

#include "AudioPluginProcessor.h"

AudioPluginProcessorEditor::AudioPluginProcessorEditor(AudioPluginProcessor& p)
    : AudioProcessorEditor(&p) {
  this->setSize(this->width, this->height);
  this->setResizable(false, false);
}

void AudioPluginProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(this->backgroundColor);
#if DEBUG
  g.setFont(12.0);
  g.setColour(juce::Colours::red);
  g.drawText("DEBUG", 0, 0, 36, 12, juce::Justification::topLeft);
#endif
}

void AudioPluginProcessorEditor::resized() {
  juce::Rectangle<int> wholeBound = this->getLocalBounds();

  // Slice margins from the whole bound
  wholeBound.removeFromLeft(this->marginUnit);
  wholeBound.removeFromRight(this->marginUnit);
  wholeBound.removeFromTop(this->marginUnit);
  wholeBound.removeFromBottom(this->marginUnit);
}
