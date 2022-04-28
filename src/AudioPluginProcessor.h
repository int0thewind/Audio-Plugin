#pragma once

#include "JuceHeader.h"
#include "dlog.cpp"
#include "processors/LowShelfFilter.h"
#include "processors/VelvetNoiseFilter.h"

using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
using NodeAndChannel = juce::AudioProcessorGraph::NodeAndChannel;
using Node = juce::AudioProcessorGraph::Node;
using Connection = juce::AudioProcessorGraph::Connection;

class AudioPluginProcessor final
    : public juce::AudioProcessor,
      private juce::AudioProcessorParameter::Listener {
 public:
  AudioPluginProcessor();
  ~AudioPluginProcessor() override;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  /** Create a GUI of the plugin when required. */
  juce::AudioProcessorEditor* createEditor() override;
  /** We have a GUI of the plugin. Return true. */
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  /**
   * Returns the number of preset programs the processor supports.
   * NB: some hosts don't cope very well if you tell them there are 0 programs,
   * so this should be at least 1, even if you're not really implementing
   * programs.
   * @return the number of preset programs the processor supports.
   */
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  void requestToUpdateProcessorSpec();

 private:
#if DEBUG
  /**
   * A application-wide file logger.
   * In a release build, it would be a null pointer. No log recorded.
   * On macOS, the log file would be in `~/Library/Logs/SoftVelvet/`
   * On Windows, it would be in `C:\\Documents and
   * Settings\\username\\Application Data\\SoftVelvet`
   * @see dlog
   */
  std::unique_ptr<juce::FileLogger> logger{
      juce::FileLogger::createDateStampedLogger(
          this->getName(), "runtime-log", ".log",
          "New Instance of SoftVelvet Audio Plugin Initialised")};
#endif
  void parameterValueChanged(int parameterIndex, float newValue) override;
  void parameterGestureChanged(int, bool) override {}

  std::unique_ptr<juce::AudioProcessorGraph> audioProcessorGraph =
      std::make_unique<juce::AudioProcessorGraph>();
  Node::Ptr audioInputNode;
  Node::Ptr audioOutputNode;
  Node::Ptr midiInputNode;
  Node::Ptr midiOutputNode;
  Node::Ptr lowShelfNode;
  Node::Ptr vnfNode;

  // All audio parameters should be raw pointers as the
  // `AudioProcessor::addParameter()` manages all added audio parameters

  juce::AudioParameterFloat* lowShelfCutoffFreqParameter =
      new juce::AudioParameterFloat("low-shelf-cutoff-freq",
                                    "Low Shelf Filter Cutoff Frequency", 25,
                                    200, 50);
  juce::AudioParameterFloat* lowShelfAttenuationDecibelParameter =
      new juce::AudioParameterFloat("low-shelf-attenuation",
                                    "Low Shelf Filter Attenuation in Decibel",
                                    -60, 0, -20);
  juce::AudioParameterInt* vnfNumberOfImpulsesParameter =
      new juce::AudioParameterInt("vnf-num-impulses",
                                  "Velvet Noise Filter Number of Impulses", 1,
                                  20, 8);
  juce::AudioParameterInt* vnfFilterLengthInMillisecondParameter =
      new juce::AudioParameterInt("vnf-filter-length",
                                  "Velvet Noise Filter Length in ms", 1, 10, 4);
  juce::AudioParameterFloat* vnfTargetDecayDecibelParameter =
      new juce::AudioParameterFloat(
          "vnf-filter-target-decay",
          "Velvet Noise Filter Target Decay in Decibel", -60, 0, -20);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginProcessor)
};
