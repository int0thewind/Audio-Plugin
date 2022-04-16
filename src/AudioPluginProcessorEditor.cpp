#include "AudioPluginProcessorEditor.h"

#include "AudioPluginProcessor.h"

AudioPluginProcessorEditor::AudioPluginProcessorEditor(AudioPluginProcessor& p)
    : AudioProcessorEditor(&p) {
#if DEBUG
  this->addAndMakeVisible(this->clearLogBtn.get());
  this->clearLogBtn->addListener(this);
  this->addAndMakeVisible(this->loggerWithDisplay.get());
#endif

  const int realWidth =
#if DEBUG
      this->width + this->debugWidth + this->marginUnit * 3;
#else
      this->width + this->marginUnit * 2;
#endif
  this->setSize(realWidth, this->height);
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

#if DEBUG
  juce::Rectangle<int> loggerBound =
      wholeBound.removeFromRight(this->debugWidth);

  juce::Rectangle<int> bottomBound =
      loggerBound.removeFromBottom(this->buttonHeight);
  this->clearLogBtn->setBounds(bottomBound.removeFromRight(this->buttonWidth));

  loggerBound.removeFromBottom(this->marginUnit);
  this->loggerWithDisplay->setBounds(loggerBound);

  wholeBound.removeFromRight(this->marginUnit);
#endif

  jassert(wholeBound.getWidth() == this->width);
  jassert(wholeBound.getHeight() == this->height);
}

void AudioPluginProcessorEditor::buttonClicked(juce::Button* button) {
#if DEBUG
  if (button == this->clearLogBtn.get()) {
    this->loggerWithDisplay->clear();
  }
#endif
}
