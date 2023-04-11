#ifndef SOFTVELVET_DLOG_CPP
#define SOFTVELVET_DLOG_CPP

#include <JuceHeader.h>

static CriticalSection lock;

/**
 * A global function to push log message to the logger.
 * In a production build, this function would do nothing. No logs are recorded.
 * Must call this function instead of invoking `juce::Logger::writeToLog()`
 * to reduce unnecessary operations in a release build.
 * @param msg log message to log.
 */
[[maybe_unused]] inline static void dlog([[maybe_unused]] StringRef msg) {
#if DEBUG
  const ScopedLock scopedLock(lock);
  Logger::writeToLog(msg);
#endif
}

#endif  // SOFTVELVET_DLOG_CPP
