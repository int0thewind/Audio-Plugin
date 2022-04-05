#include "AudioPluginProcessorEditor.h"

#include "AudioPluginProcessor.h"

AudioPluginProcessorEditor::AudioPluginProcessorEditor(AudioPluginProcessor& p)
    : AudioProcessorEditor(&p) {
  this->midiInfoClearBtn =
      std::make_unique<juce::TextButton>("MIDI Info Clear Button");
  this->midiInfoClearBtn->setVisible(true);
  this->midiInfoClearBtn->setButtonText("Clear MIDI Info");
  this->midiInfoClearBtn->addListener(this);
  this->addChildComponent(*midiInfoClearBtn);

  this->audioInfoClearBtn =
      std::make_unique<juce::TextButton>("Audio Info Clear Button");
  this->audioInfoClearBtn->setVisible(true);
  this->audioInfoClearBtn->setButtonText("Clear Audio Info");
  this->audioInfoClearBtn->addListener(this);
  this->addChildComponent(*audioInfoClearBtn);

  this->midiInfoBox = std::make_unique<InfoDisplayBox>("Midi Info Box");
  this->midiInfoBox->setVisible(true);
  this->addChildComponent(*midiInfoBox);

  this->audioInfoBox = std::make_unique<InfoDisplayBox>("Audio Info Box");
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
  this->midiInfoClearBtn->setBounds(
      midiBound.removeFromBottom(this->btnHeight));
  this->midiInfoBox->setBounds(midiBound);

  juce::Rectangle<int> audioBound = bound.removeFromRight(infoDisplayBoxWidth);
  this->audioInfoClearBtn->setBounds(
      audioBound.removeFromBottom(this->btnHeight));
  this->audioInfoBox->setBounds(audioBound);
}

void AudioPluginProcessorEditor::buttonClicked(juce::Button* button) {
  if (button == nullptr)
    return;
  else if (button == this->midiInfoClearBtn.get())
    this->midiInfoBox->clearMessages();
  else if (button == this->audioInfoClearBtn.get())
    this->audioInfoBox->clearMessages();
}

void AudioPluginProcessorEditor::pushMessageToMidiInfoBox(
    const juce::String& message) {
  this->midiInfoBox->addMessage(message);
}
