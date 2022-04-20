//
// Created by Hanzhi Yin on 4/19/22.
//

#include "VelvetNoiseFilter.h"

void VelvetNoiseFilter::prepareToPlay(double sampleRate,
                                      int maximumExpectedSamplesPerBlock) {
  this->savedSampleRate = sampleRate;
  this->vnf.prepare(
      {sampleRate, static_cast<u_int32_t>(maximumExpectedSamplesPerBlock), 2});
  this->requestToUpdateProcessorSpec();
}

void VelvetNoiseFilter::releaseResources() { this->vnf.reset(); }

void VelvetNoiseFilter::processBlock(juce::AudioBuffer<float>& buffer,
                                     juce::MidiBuffer& midiMessages) {
  if (this->isDirty.load()) {
    this->updateProcessorSpec();
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
  vnflog("Instance created");
}

bool VelvetNoiseFilter::setNumberOfImpulses(size_t _numberOfImpulses) {
  if (_numberOfImpulses != this->numberOfImpulses) {
    this->getCallbackLock().enter();
    this->numberOfImpulses = _numberOfImpulses;
    this->requestToUpdateProcessorSpec();
    this->getCallbackLock().exit();
    return true;
  }
  return false;
}

bool VelvetNoiseFilter::setFilterLengthInMillisecond(
    size_t _filterLengthInMillisecond) {
  if (_filterLengthInMillisecond != this->filterLengthInMillisecond) {
    this->getCallbackLock().enter();
    this->filterLengthInMillisecond = _filterLengthInMillisecond;
    this->requestToUpdateProcessorSpec();
    this->getCallbackLock().exit();
    return true;
  }
  return false;
}

bool VelvetNoiseFilter::setTargetDecayDecibel(float _targetDecayDecibel) {
  bool result = false;
  if (_targetDecayDecibel != this->targetDecayDecibel) {
    this->getCallbackLock().enter();
    this->targetDecayDecibel = _targetDecayDecibel;
    this->requestToUpdateProcessorSpec();
    this->getCallbackLock().exit();
    result = true;
  }
  return result;
}

void VelvetNoiseFilter::updateProcessorSpec() {
  jassert(this->savedSampleRate > .0);

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
  delete filter;
  //  filter = nullptr;

  // Set off the flag
  this->isDirty = false;
  vnflog("Filter created");
}

const juce::String VelvetNoiseFilter::getName() const {
  return "Velvet Noise Filter";
}
