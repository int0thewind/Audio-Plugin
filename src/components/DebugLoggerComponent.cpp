//
// Created by Hanzhi Yin on 3/1/22.
//

#if DEBUG
#include "DebugLoggerComponent.h"

DebugLoggerComponent::DebugLoggerComponent(juce::StringRef componentName)
    : juce::TextEditor(componentName) {
  // Register the global logger
  juce::Logger::setCurrentLogger(this);

  // Set text editor properties
  this->setMultiLine(true);
  this->setReadOnly(true);
  this->setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 12.0,
                           juce::Font::plain));
}

DebugLoggerComponent::~DebugLoggerComponent() {
  // Deregister the global logger
  juce::Logger::setCurrentLogger(nullptr);
}

void DebugLoggerComponent::logMessage(const juce::String& msg) {
  // Text editor has no custom write access
  // The caret in the editor is always at the end
  this->insertTextAtCaret(msg);
}

#endif
