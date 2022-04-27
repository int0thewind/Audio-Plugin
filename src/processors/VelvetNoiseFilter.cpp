#include "VelvetNoiseFilter.h"

void VelvetNoiseFilter::prepareToPlay(double sampleRate,
                                      int maximumExpectedSamplesPerBlock) {
  dlog(juce::String("VelvetNoiseFilter::prepareToPlay() method called") +
       juce::String(". Sample rate: ") + juce::String(sampleRate, 1) +
       juce::String(". Samples per block: ") +
       juce::String(maximumExpectedSamplesPerBlock));

  this->savedSampleRate = sampleRate;
  this->vnf.prepare({sampleRate,
                     static_cast<juce::uint32>(maximumExpectedSamplesPerBlock),
                     2});
  this->requestToUpdateProcessorSpec();
}

void VelvetNoiseFilter::releaseResources() {
  dlog(juce::String("VelvetNoiseFilter::releaseResources() method called."));
  this->vnf.reset();
}

void VelvetNoiseFilter::processBlock(juce::AudioBuffer<float>& buffer,
                                     juce::MidiBuffer& midiMessages) {
  if (this->isDirty.load()) {
    dlog(
        "VelvetNoiseFilter::processBlock() detected processor spec update "
        "request. Update it.");
    this->updateProcessorSpec();
    dlog("VelvetNoiseFilter::processBlock() update processor spec finished.");
  }
  juce::dsp::AudioBlock<float> block(buffer);
  juce::dsp::ProcessContextReplacing<float> context(block);
  this->vnf.process(context);
}

VelvetNoiseFilter::VelvetNoiseFilter(unsigned int _numberOfImpulses,
                                     unsigned int _filterLengthInMillisecond,
                                     float _targetDecayDecibel)
    : BaseAudioProcessor(),
      numberOfImpulses(_numberOfImpulses),
      filterLengthInMillisecond(_filterLengthInMillisecond),
      targetDecayDecibel(_targetDecayDecibel) {
  dlog("VelvetNoiseFilter new instance created");
}

bool VelvetNoiseFilter::setNumberOfImpulses(size_t _numberOfImpulses) {
  dlog("VelvetNoiseFilter::setNumberOfImpulses() method called.");
  if (_numberOfImpulses != this->numberOfImpulses) {
    this->getCallbackLock().enter();
    this->numberOfImpulses = _numberOfImpulses;
    this->requestToUpdateProcessorSpec();
    this->getCallbackLock().exit();
    dlog(
        "VelvetNoiseFilter::setNumberOfImpulses() successfully set the number "
        "of impulses to " +
        juce::String(_numberOfImpulses) + '.');
    return true;
  }
  dlog("VelvetNoiseFilter::setNumberOfImpulses() value doesn't change from " +
       juce::String(_numberOfImpulses) + '.');
  return false;
}

bool VelvetNoiseFilter::setFilterLengthInMillisecond(
    size_t _filterLengthInMillisecond) {
  if (_filterLengthInMillisecond != this->filterLengthInMillisecond) {
    this->getCallbackLock().enter();
    this->filterLengthInMillisecond = _filterLengthInMillisecond;
    this->requestToUpdateProcessorSpec();
    this->getCallbackLock().exit();
    dlog(
        "VelvetNoiseFilter::setFilterLengthInMillisecond() successfully set "
        "the filter length to " +
        juce::String(_filterLengthInMillisecond) + " ms.");
    return true;
  }
  dlog(
      "VelvetNoiseFilter::setFilterLengthInMillisecond() value doesn't change "
      "from" +
      juce::String(_filterLengthInMillisecond) + " ms.");
  return false;
}

bool VelvetNoiseFilter::setTargetDecayDecibel(float _targetDecayDecibel) {
  if (_targetDecayDecibel != this->targetDecayDecibel) {
    this->getCallbackLock().enter();
    this->targetDecayDecibel = _targetDecayDecibel;
    this->requestToUpdateProcessorSpec();
    this->getCallbackLock().exit();
    dlog(
        "VelvetNoiseFilter::setTargetDecayDecibel() successfully set "
        "the target decay to " +
        juce::String(_targetDecayDecibel) + " dB.");
    return true;
  }
  dlog("VelvetNoiseFilter::setTargetDecayDecibel() value doesn't change from " +
       juce::String(_targetDecayDecibel) + " dB.");
  return false;
}

void VelvetNoiseFilter::updateProcessorSpec() {
  jassert(this->savedSampleRate > .0);

  dlog(juce::String(
      "AudioPluginProcessor::updateProcessorSpec() method called."));

  // Local random object
  juce::Random rnd;

  // Initialise filter
  // Using raw pointer to ensure all numbers are zero.
  const auto filterSize = static_cast<size_t>(
      this->savedSampleRate *
      static_cast<double>(this->filterLengthInMillisecond) / 1000.0);
  auto* filter = static_cast<float*>(calloc(filterSize, sizeof(float)));

  // Get all pending indices to store non-zero points in a VNF
  // Using JUCE Array as we need these handy methods
  juce::Array<int> indices;
  while (indices.size() < static_cast<int>(this->numberOfImpulses)) {
    int i = rnd.nextInt((int)filterSize);
    indices.addIfNotAlreadyThere(i);
  }
  indices.sort();
  jassert((size_t)indices.size() == this->numberOfImpulses);

  // Compute all non-zero value and fill.
  auto alpha =
      static_cast<float>((-1 / static_cast<float>(this->numberOfImpulses)) *
                         log(pow(10.0, this->targetDecayDecibel / 20.0)));
  for (size_t i = 0; i < this->numberOfImpulses; ++i) {
    int index = indices[(int)i];
    filter[index] = static_cast<float>(exp(-alpha * static_cast<float>(i)) *
                                       (rnd.nextBool() ? -1.0 : 1.0));
  }

  // Create filter
  *(this->vnf.state) = FIRCoefficient(filter, filterSize);

  // Free up memory
  free(filter);

  // Set off the flag
  this->isDirty = false;
  dlog(juce::String(
      "AudioPluginProcessor::updateProcessorSpec() new filter created."));
}

const juce::String VelvetNoiseFilter::getName() const {
  return "Velvet Noise Filter";
}
