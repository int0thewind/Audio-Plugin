//
// Created by Hanzhi Yin on 3/1/22.
//

#ifndef PLUGIN_DEBUGLOGGERCOMPONENT_H
#define PLUGIN_DEBUGLOGGERCOMPONENT_H

#include "juce_gui_basics/juce_gui_basics.h"

/**
 * A global function to push log message to the logger.
 * Must call this function instead of invoking `juce::Logger::writeToLog()`
 * to reduce unnecessary operations in a release build.
 * @param msg log message to log.
 */
inline static void dlog(juce::StringRef msg) {
#if DEBUG
  juce::String s{};
  s << '[' << juce::Time::getCurrentTime().toString(true, true, true, true)
    << "] " << msg;
  juce::Logger::writeToLog(s);
#else
  juce::ignoreUnused(msg);
#endif
}

#if DEBUG

/**
 * Application-wide logger that receives log message and displays it.
 * You should never create more than one one instance of this class!
 * This class constructor automatically register itself as the application-wide
 * logger, and only one logger is permitted for an application.
 */
class DebugLoggerComponent final : public juce::TextEditor,
                                   private juce::Logger {
 public:
  /**
   * Logger with display constructor, constructs an instance of it
   * and register the logger to the application.
   * @param componentName the name of the logger component
   */
  explicit DebugLoggerComponent(juce::StringRef componentName);
  ~DebugLoggerComponent() override;

 private:
  void logMessage(const juce::String &message) override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DebugLoggerComponent)
};

#endif  // DEBUG

#endif  // PLUGIN_DEBUGLOGGERCOMPONENT_H
