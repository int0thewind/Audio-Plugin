#if JucePlugin_Build_AU || JucePlugin_Build_Standalone

#include "src/AudioPluginProcessor.h"
/**
 * The designated main routine of the plugin lifecycle, this function would be
 * invoked by the JUCE library code when the program starts. The returned audio
 * processor would be utilized by JUCE to start an instance of our plugin.
 * @return a new instance of our plugin.
 */
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new AudioPluginProcessor();
}

#else

#include "src/MainApplication.cpp"
START_JUCE_APPLICATION(MainApplication)

#endif
