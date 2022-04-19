//
// Created by Hanzhi Yin on 4/19/22.
//

#include "VelvetNoiseFilter.h"

void VelvetNoiseFilter::prepareToPlay(double _sampleRate,
                                      int maximumExpectedSamplesPerBlock) {
  this->sampleRate = _sampleRate;
}

void VelvetNoiseFilter::releaseResources() {}

void VelvetNoiseFilter::processBlock(juce::AudioBuffer<float>& buffer,
                                     juce::MidiBuffer& midiMessages) {}

VelvetNoiseFilter::VelvetNoiseFilter(unsigned int _numberOfImpulses,
                                     unsigned int _filterLengthInMillisecond,
                                     float _targetDecayDecibel)
    : BaseAudioProcessor(),
      numberOfImpulses(_numberOfImpulses),
      filterLengthInMillisecond(_filterLengthInMillisecond),
      targetDecayDecibel(_targetDecayDecibel) {
  vnflog("Instance created");
}

int VelvetNoiseFilter::getNumberOfImpulses() const { return numberOfImpulses; }

void VelvetNoiseFilter::setNumberOfImpulses(int _numberOfImpulses) {
  this->numberOfImpulses = _numberOfImpulses;
  this->recreateFilter();
}

int VelvetNoiseFilter::getFilterLengthInMillisecond() const {
  return filterLengthInMillisecond;
}

void VelvetNoiseFilter::setFilterLengthInMillisecond(
    int _filterLengthInMillisecond) {
  this->filterLengthInMillisecond = _filterLengthInMillisecond;
  this->recreateFilter();
}

float VelvetNoiseFilter::getTargetDecayDecibel() const {
  return targetDecayDecibel;
}

void VelvetNoiseFilter::setTargetDecayDecibel(float _targetDecayDecibel) {
  this->targetDecayDecibel = _targetDecayDecibel;
  this->recreateFilter();
}

void VelvetNoiseFilter::recreateFilter() {
  vnflog("Recreate filter request received.");
  this->isDirty = true;
}

void VelvetNoiseFilter::createFilter() {
  jassert(this->sampleRate > .0);

  // Local random object
  juce::Random rnd;

  // Initialise filter
  const auto filterSize = static_cast<size_t>(
      this->sampleRate * this->filterLengthInMillisecond / 1000);
  auto* filter = static_cast<float*>(calloc(filterSize, sizeof(float)));

  // Get all pending indices to store non-zero points in a VNF
  juce::Array<int> indices;
  while (indices.size() < static_cast<int>(this->numberOfImpulses)) {
    int i = rnd.nextInt((int)filterSize);
    indices.addIfNotAlreadyThere(i);
  }
  indices.sort();
  jassert((size_t)indices.size() == this->numberOfImpulses);

  auto alpha =
      static_cast<float>((-1 / static_cast<float>(this->numberOfImpulses)) *
                         log(pow(10.0, this->targetDecayDecibel / 20.0)));
  for (size_t i = 0; i < this->numberOfImpulses; ++i) {
    int index = indices[(int)i];
    filter[index] = static_cast<float>(exp(-alpha * static_cast<float>(i)) *
                                       (rnd.nextBool() ? -1.0 : 1.0));
  }

  // TODO: how to use the raw pointer `filter`?

  isDirty = false;
  vnflog("Filter created");
}
