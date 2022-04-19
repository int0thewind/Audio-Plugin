//
// Created by Hanzhi Yin on 4/19/22.
//

#pragma once

#include "juce_core/juce_core.h"
/**
 * A global function to push log message to the logger.
 * In a production build, this function would do nothing. No logs are recorded.
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
