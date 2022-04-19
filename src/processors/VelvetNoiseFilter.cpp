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

VelvetNoiseFilter::VelvetNoiseFilter(int _numberOfImpulses,
                                     int _filterLengthInMillisecond,
                                     float _targetDecayDecibel)
    : numberOfImpulses(_numberOfImpulses),
      filterLengthInMillisecond(_filterLengthInMillisecond),
      targetDecayDecibel(_targetDecayDecibel) {
  vnflog("Instance created");
}

VelvetNoiseFilter::~VelvetNoiseFilter() {
  vnflog("Instance is about to destruct.");
  this->releaseResources();
}

juce::Array<float> VelvetNoiseFilter::makeDecayingSample(size_t size,
                                                         float alpha) {
  juce::Random rnd;
  juce::Array<float> samples;
  samples.resize(static_cast<int>(size));
  for (size_t i = 0; i < size; ++i) {
    samples.add(static_cast<float>(exp(-alpha * static_cast<float>(i)) *
                                   (rnd.nextBool() ? -1.0 : 1.0)));
  }
  return samples;
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
  // recreate filter

  int filterSize = static_cast<int>(this->sampleRate *
                                    this->filterLengthInMillisecond / 1000);

  juce::Array<float> filter;
  filter.resize(filterSize);

  float alpha = this->inverseEnergyDecay();

  this->isDirty = false;
  vnflog("Filter created");
}
