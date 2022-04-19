#include "AudioPluginProcessor.h"

#include "dlog.cpp"

AudioPluginProcessor::AudioPluginProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ) {
#if DEBUG
  // Register the logger to the application
  juce::Logger::setCurrentLogger(this->logger.get());
#endif

  this->addParameter(this->vnfNumberOfImpulses);
  this->addParameter(this->vnfFilterLengthInMillisecond);
  this->addParameter(this->vnfTargetDecayDecibel);

  dlog(juce::String{"Plugin is loaded by "} +
       AudioPluginProcessor::getWrapperTypeDescription(
           juce::PluginHostType::getPluginLoadedAs()));
}

AudioPluginProcessor::~AudioPluginProcessor() {
#if DEBUG
  // Deregister the application-wide logger before quitting the plugin.
  juce::Logger::setCurrentLogger(nullptr);
#endif
}

const juce::String AudioPluginProcessor::getName() const {
  return JucePlugin_Name;
}

bool AudioPluginProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool AudioPluginProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool AudioPluginProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double AudioPluginProcessor::getTailLengthSeconds() const { return 0.0; }

int AudioPluginProcessor::getNumPrograms() { return 1; }

int AudioPluginProcessor::getCurrentProgram() { return 0; }

void AudioPluginProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String AudioPluginProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void AudioPluginProcessor::changeProgramName(int index,
                                             const juce::String &newName) {
  juce::ignoreUnused(index, newName);
}

void AudioPluginProcessor::prepareToPlay(double sampleRate,
                                         int samplesPerBlock) {
  dlog(juce::String("`prepareToPlay` method called."));
  dlog(juce::String("Sample rate: ") + juce::String(sampleRate, 1));
  dlog(juce::String("Samples per block: ") + juce::String(samplesPerBlock));
}

void AudioPluginProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool AudioPluginProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void AudioPluginProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                        juce::MidiBuffer &midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;
  int totalNumInputChannels = this->getTotalNumInputChannels();
  int totalNumOutputChannels = this->getTotalNumOutputChannels();

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  // Make sure to reset the state if your inner loop is processing
  // the samples and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.
  for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    float *channelData = buffer.getWritePointer(channel);
    juce::ignoreUnused(channelData);
    // ...do something to the data...
  }
}

bool AudioPluginProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor *AudioPluginProcessor::createEditor() {
  return new juce::GenericAudioProcessorEditor(*this);
}

void AudioPluginProcessor::getStateInformation(juce::MemoryBlock &destData) {
  std::unique_ptr<juce::XmlElement> xmlState =
      // use the plugin name as the tag name
      std::make_unique<juce::XmlElement>(this->getName());
  // TODO: pass audio parameters to the XML element
  AudioPluginProcessor::copyXmlToBinary(*xmlState, destData);
}

void AudioPluginProcessor::setStateInformation(const void *data,
                                               int sizeInBytes) {
  std::unique_ptr<juce::XmlElement> xmlState =
      AudioPluginProcessor::getXmlFromBinary(data, sizeInBytes);

  if (xmlState != nullptr) {  // initial data block may have no data
    // sanity check: if the data does not belong to our plugin
    if (xmlState->hasTagName(this->getName())) {
      // TODO: restore audio parameters from the XML element
    }
  }
}
