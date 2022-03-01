//
// Created by Hanzhi Yin on 3/1/22.
//

#include "InfoDisplayBox.h"
InfoDisplayBox::InfoDisplayBox(const juce::String &componentName)
    : ListBox(componentName, this) {}

InfoDisplayBox::~InfoDisplayBox() = default;

void InfoDisplayBox::handleAsyncUpdate() {
  this->updateContent();
  this->scrollToEnsureRowIsOnscreen(this->getNumRows());
  this->repaint();
}

int InfoDisplayBox::getNumRows() { return this->messages.size(); }

void InfoDisplayBox::paintListBoxItem(int rowNumber, juce::Graphics &g,
                                      int width, int height,
                                      bool rowIsSelected) {
  juce::ignoreUnused(rowIsSelected);

  if (rowNumber > this->getNumRows()) {
    return;
  }

  auto textBound = juce::Rectangle<int>(4, 0, width, height);
  g.setColour(juce::Colours::white);
  g.setFont(15.0);
  g.drawText(this->messages[rowNumber], textBound,
             juce::Justification::centredLeft);
}

void InfoDisplayBox::addMessage(const juce::String &message) {
  this->messages.add(juce::String(message));
  this->triggerAsyncUpdate();
}

void InfoDisplayBox::clearMessages() {
  this->messages.clear();
  this->triggerAsyncUpdate();
}