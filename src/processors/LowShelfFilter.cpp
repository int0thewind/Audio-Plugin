//
// Created by Hanzhi Yin on 4/19/22.
//

#include "LowShelfFilter.h"
const juce::String LowShelfFilter::getName() const {
  return "Low Shelf Filter";
}

void LowShelfFilter::prepareToPlay(double sampleRate,
                                   int maximumExpectedSamplesPerBlock) {
  this->savedSampleRate = sampleRate;
  this->lowShelf.prepare(
      {sampleRate, static_cast<u_int32_t>(maximumExpectedSamplesPerBlock), 2});
}

void LowShelfFilter::releaseResources() { this->lowShelf.reset(); }

void LowShelfFilter::processBlock(juce::AudioBuffer<float>& buffer,
                                  juce::MidiBuffer& midiMessages) {
  if (this->isDirty.load()) this->updateProcessorSpec();
  juce::dsp::AudioBlock<float> block(buffer);
  juce::dsp::ProcessContextReplacing<float> context(block);
  this->lowShelf.process(context);
}

LowShelfFilter::LowShelfFilter(float _cutoffFreq, float _attenuationDecibel)
    : cutoffFreq(_cutoffFreq), attenuationDecibel(_attenuationDecibel) {}

void LowShelfFilter::updateProcessorSpec() {
  this->lowShelf.state = IIRCoefficient::makeLowShelf(
      this->savedSampleRate, this->cutoffFreq, 5, this->attenuationDecibel);
}
bool LowShelfFilter::setCutoffFreq(float _cutoffFreq) {
  if (_cutoffFreq == this->cutoffFreq) {
    this->getCallbackLock().enter();
    this->cutoffFreq = _cutoffFreq;
    this->requestToUpdateProcessorSpec();
    this->getCallbackLock().exit();
    return true;
  }
  return false;
}
bool LowShelfFilter::setAttenuationDecibel(float _attenuationDecibel) {
  if (_attenuationDecibel == this->attenuationDecibel) {
    this->getCallbackLock().enter();
    LowShelfFilter::attenuationDecibel = _attenuationDecibel;
    this->requestToUpdateProcessorSpec();
    this->getCallbackLock().exit();
    return true;
  }
  return false;
}
