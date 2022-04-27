#ifndef SOFTVELVET_DLOG_CPP
#define SOFTVELVET_DLOG_CPP

#include "juce_core/juce_core.h"

static juce::CriticalSection lock;

/**
 * A global function to push log message to the logger.
 * In a production build, this function would do nothing. No logs are recorded.
 * Must call this function instead of invoking `juce::Logger::writeToLog()`
 * to reduce unnecessary operations in a release build.
 * @param msg log message to log.
 */
[[maybe_unused]] inline static void dlog([[maybe_unused]] juce::StringRef msg) {
#if DEBUG
  const juce::ScopedLock scopedLock(lock);
  juce::Logger::writeToLog(msg);
#endif
}

#endif  // SOFTVELVET_DLOG_CPP
