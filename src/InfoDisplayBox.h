//
// Created by Hanzhi Yin on 3/1/22.
//

#ifndef PLUGIN_INFODISPLAYBOX_H
#define PLUGIN_INFODISPLAYBOX_H

#include "juce_gui_basics/juce_gui_basics.h"

class InfoDisplayBox : public juce::ListBox,
                       private juce::ListBoxModel,
                       private juce::AsyncUpdater {
 public:
  ~InfoDisplayBox() override;
  explicit InfoDisplayBox(const juce::String &componentName);
  inline int getNumRows() override;

  void addMessage(const juce::String &message);
  void clearMessages();

 private:
  void handleAsyncUpdate() override;
  void paintListBoxItem(int rowNumber, juce::Graphics &g, int width, int height,
                        bool rowIsSelected) override;

  juce::StringArray messages;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfoDisplayBox)
};

#endif  // PLUGIN_INFODISPLAYBOX_H
