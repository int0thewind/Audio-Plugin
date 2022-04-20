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

  this->addParameter(this->lowShelfCutoffFreqParameter);
  this->addParameter(this->lowShelfAttenuationDecibelParameter);
  this->addParameter(this->vnfNumberOfImpulsesParameter);
  this->addParameter(this->vnfFilterLengthInMillisecondParameter);
  this->addParameter(this->vnfTargetDecayDecibelParameter);
  this->lowShelfCutoffFreqParameter->addListener(this);
  this->lowShelfAttenuationDecibelParameter->addListener(this);
  this->vnfFilterLengthInMillisecondParameter->addListener(this);
  this->vnfNumberOfImpulsesParameter->addListener(this);
  this->vnfTargetDecayDecibelParameter->addListener(this);

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

  this->mainProcessor->setPlayConfigDetails(this->getMainBusNumInputChannels(),
                                            this->getMainBusNumOutputChannels(),
                                            sampleRate, samplesPerBlock);
  this->mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);

  this->initialiseGraph();
}

void AudioPluginProcessor::releaseResources() {
  this->mainProcessor->releaseResources();
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

  for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  this->mainProcessor->processBlock(buffer, midiMessages);
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

void AudioPluginProcessor::initialiseGraph() {
  this->mainProcessor->clear();
  this->audioInputNode =
      this->mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(
          AudioGraphIOProcessor::audioInputNode));
  this->audioOutputNode =
      this->mainProcessor->addNode((std::make_unique<AudioGraphIOProcessor>(
          AudioGraphIOProcessor::audioOutputNode)));
  this->midiInputNode =
      this->mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(
          AudioGraphIOProcessor::midiInputNode));
  this->midiOutputNode =
      this->mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(
          AudioGraphIOProcessor::midiOutputNode));

  this->lowShelfNode =
      this->mainProcessor->addNode(std::make_unique<LowShelfFilter>(
          this->lowShelfCutoffFreqParameter->get(),
          this->lowShelfAttenuationDecibelParameter->get()));
  this->vnfNode =
      this->mainProcessor->addNode(std::make_unique<VelvetNoiseFilter>(
          this->vnfNumberOfImpulsesParameter->get(),
          this->vnfFilterLengthInMillisecondParameter->get(),
          this->vnfTargetDecayDecibelParameter->get()));

  // Connect Audio Nodes
  for (int channel = 0; channel < 2; ++channel) {
    this->mainProcessor->addConnection({{this->audioInputNode->nodeID, channel},
                                        {this->lowShelfNode->nodeID, channel}});
    this->mainProcessor->addConnection({{this->lowShelfNode->nodeID, channel},
                                        {this->vnfNode->nodeID, channel}});
    this->mainProcessor->addConnection(
        {{this->vnfNode->nodeID, channel},
         {this->audioOutputNode->nodeID, channel}});
  }
  // Connect MIDI Nodes
  this->mainProcessor->addConnection(
      {{this->midiInputNode->nodeID,
        juce::AudioProcessorGraph::midiChannelIndex},
       {this->midiOutputNode->nodeID,
        juce::AudioProcessorGraph::midiChannelIndex}});
}

void AudioPluginProcessor::parameterValueChanged(int parameterIndex, float) {
  // TODO: log all behaviours!
  if (parameterIndex ==
      this->lowShelfCutoffFreqParameter->getParameterIndex()) {
    ((LowShelfFilter *)this->lowShelfNode->getProcessor())
        ->setCutoffFreq(this->lowShelfCutoffFreqParameter->get());
  } else if (parameterIndex ==
             this->lowShelfAttenuationDecibelParameter->getParameterIndex()) {
    ((LowShelfFilter *)this->lowShelfNode->getProcessor())
        ->setAttenuationDecibel(
            this->lowShelfAttenuationDecibelParameter->get());
  } else if (parameterIndex ==
             this->vnfFilterLengthInMillisecondParameter->getParameterIndex()) {
    ((VelvetNoiseFilter *)this->vnfNode->getProcessor())
        ->setFilterLengthInMillisecond(static_cast<size_t>(
            this->vnfFilterLengthInMillisecondParameter->get()));
  } else if (parameterIndex ==
             this->vnfNumberOfImpulsesParameter->getParameterIndex()) {
    ((VelvetNoiseFilter *)this->vnfNode->getProcessor())
        ->setNumberOfImpulses(
            static_cast<size_t>(this->vnfNumberOfImpulsesParameter->get()));
  } else if (parameterIndex ==
             this->vnfTargetDecayDecibelParameter->getParameterIndex()) {
    ((VelvetNoiseFilter *)this->vnfNode->getProcessor())
        ->setTargetDecayDecibel(this->vnfTargetDecayDecibelParameter->get());
  }
}
void AudioPluginProcessor::parameterGestureChanged(int parameterIndex,
                                                   bool gestureIsStarting) {}
void AudioPluginProcessor::handleAsyncUpdate() {
  // TODO: should I do async update?
}
