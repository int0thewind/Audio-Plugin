//
// Created by Hanzhi Yin on 4/19/22.
//

#include "LowShelfFilter.h"
const juce::String LowShelfFilter::getName() const {
  return "Low Shelf Filter";
}

void LowShelfFilter::prepareToPlay(double sampleRate,
                                   int maximumExpectedSamplesPerBlock) {
  dlog(juce::String("LowShelfFilter::prepareToPlay() method called") +
       juce::String(". Sample rate: ") + juce::String(sampleRate, 1) +
       juce::String(". Samples per block: ") +
       juce::String(maximumExpectedSamplesPerBlock));

  this->savedSampleRate = sampleRate;
  this->lowShelf.prepare(
      {sampleRate, static_cast<u_int32_t>(maximumExpectedSamplesPerBlock), 2});
  this->requestToUpdateProcessorSpec();
}

void LowShelfFilter::releaseResources() {
  dlog(juce::String("LowShelfFilter::releaseResources() method called."));
  this->lowShelf.reset();
}

void LowShelfFilter::processBlock(juce::AudioBuffer<float>& buffer,
                                  juce::MidiBuffer& midiMessages) {
  if (this->isDirty.load()) {
    dlog(
        "LowShelfFilter::processBlock() detected processor spec update "
        "request. Update it.");
    this->updateProcessorSpec();
    dlog("LowShelfFilter::processBlock() update processor spec finished.");
  }
  juce::dsp::AudioBlock<float> block(buffer);
  juce::dsp::ProcessContextReplacing<float> context(block);
  this->lowShelf.process(context);
}

LowShelfFilter::LowShelfFilter(float _cutoffFreq, float _attenuationDecibel)
    : BaseAudioProcessor(),
      cutoffFreq(_cutoffFreq),
      attenuationDecibel(_attenuationDecibel) {
  dlog("LowShelfFilter new instance created");
}

void LowShelfFilter::updateProcessorSpec() {
  dlog("LowShelfFilter::updateProcessorSpec() method called.");
  this->lowShelf.state = IIRCoefficient::makeLowShelf(
      this->savedSampleRate, this->cutoffFreq, 5,
      juce::Decibels::decibelsToGain(this->attenuationDecibel));
  this->isDirty = false;
  dlog("LowShelfFilter::updateProcessorSpec() new filter created.");
}

bool LowShelfFilter::setCutoffFreq(float _cutoffFreq) {
  if (_cutoffFreq != this->cutoffFreq) {
    this->getCallbackLock().enter();
    this->cutoffFreq = _cutoffFreq;
    this->requestToUpdateProcessorSpec();
    this->getCallbackLock().exit();
    dlog(
        "LowShelfFilter::setCutoffFreq() successfully set "
        "the cutoff frequency to " +
        juce::String(_cutoffFreq) + " Hz.");
    return true;
  }
  dlog("LowShelfFilter::setCutoffFreq() value doesn't change from " +
       juce::String(_cutoffFreq) + " Hz.");
  return false;
}

bool LowShelfFilter::setAttenuationDecibel(float _attenuationDecibel) {
  if (_attenuationDecibel != this->attenuationDecibel) {
    this->getCallbackLock().enter();
    this->attenuationDecibel = _attenuationDecibel;
    this->requestToUpdateProcessorSpec();
    this->getCallbackLock().exit();
    dlog(
        "LowShelfFilter::setAttenuationDecibel() successfully set "
        "the attenuation to " +
        juce::String(_attenuationDecibel) + " dB.");
    return true;
  }
  dlog("LowShelfFilter::setAttenuationDecibel() value doesn't change from " +
       juce::String(_attenuationDecibel) + " dB.");
  return false;
}
