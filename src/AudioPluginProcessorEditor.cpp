#include "AudioPluginProcessorEditor.h"

#include "AudioPluginProcessor.h"

AudioPluginProcessorEditor::AudioPluginProcessorEditor(
    AudioPluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef_(p) {
  juce::ignoreUnused(processorRef_);
  this->setSize(400, 300);
}

void AudioPluginProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  g.setColour(juce::Colours::white);
  g.setFont(15.0F);
  g.drawFittedText("Hello World!", getLocalBounds(),
                   juce::Justification::centred, 1);
}

void AudioPluginProcessorEditor::resized() {}
