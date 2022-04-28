from utils import print_audio_info
from pipeline import pipeline
import os, requests, json, time, logging
import tempfile
import soundfile as sf

import numpy as np
import numpy.typing as npt
import scipy.signal as signal
import scipy.io as sio
import matplotlib.pyplot as plt

import streamlit as st

############### Load demo files ###############################################
hihat_file = './audio/hihat.wav'
hihat_sr, hihat_data = sio.wavfile.read(hihat_file)
hihat_data = hihat_data.T

snare_file = './audio/snare.wav'
snare_sr, snare_data = sio.wavfile.read(snare_file)
snare_data = snare_data.T

############### Introduction ##################################################
st.set_page_config(
    page_title="VNF Conversion",
    page_icon="✳️",
    layout="wide",
)

st.image(
    './img/cmu_logo.png',
    caption='Copyright Ruibin Yuan & Hanzhi Yin @2022',
    width=150)

st.title('One-to-many Sample Conversion with Velvet Noise Filter')
st.markdown("We apply a Velvet Noise Filter(VNF) to avoid the machine gun effect when repeating the same percussive sample, by introducing variation to the higher frequencies.  \n"
            "The system contains three parts, a low-shelf filter, a velvet noise filter, and a gain. The filtered signal and the original signal are combined together as the final output.  \n"
            "See the [paper](https://dafx2020.mdw.ac.at/proceedings/papers/DAFx20in21_paper_22.pdf) for details. "
            "You can also see our [notion page](https://int0thewind.notion.site/Prototype-Demo-760dc5ccb0ce4a78ae37eb858fd544ae) for some listening examples.")
st.image(
    './img/vnf-pipeline.png',
    caption='The System Pipeline',
    width=1100,)
col_1, col_2 = st.columns(2)
col_2.image(
    './img/velvet-noise.png',
    caption='Time-domain Response of a Velvet Noise Filter',
    width=550,)
col_1.image(
    './img/low-shelf.png',
    caption='Frequency Response of a Low-Shelf Filter',
    width=550,)

###############################################################################
def get_audio_data(input_audio_type):
    
    return data, sr

st.markdown("#### Input An Audio File")
input_audio_type = st.selectbox(
     'Input a short audio File, and it will be cut down to 0.15s. You can either select a demo file or upload your own.',
     ('hihat-demo', 'snare-demo', 'I want to upload my own audio file'))

if input_audio_type == 'hihat-demo':
    data_in, sr_in = hihat_data, hihat_sr
elif input_audio_type == 'snare-demo':
    data_in, sr_in = snare_data, snare_sr
else:
    uploaded_file = st.file_uploader('Upload your own audio file', type='wav')
    if uploaded_file is not None:
        sr_in, data_in = sio.wavfile.read(uploaded_file)
        data_in = data_in.T

###############################################################################
st.markdown("#### Select The Hyperparameters")
st.markdown("There are 7 hyperparameters to tune: fc, G, Q, Ls, M, LdB, gw.")
col1, col2 = st.columns(2)
fc = col1.slider('fc',
                0,
                500,
                50,
                help=f'Center frequency of the low shelf filter, in Hz. It attenuate the lower frequencies. Default is 50Hz')
G = col2.slider('G',
                -50.0,
                0.0,
                -20.0,
                help=f'Level off gain of the low shelf filter, in dB. Default is -20dB.')
Q = col1.slider('Q',
                0.0,
                10.0,
                2.0,
                help=f'Q factor of the low shelf filter. Bigger Q will result in a narrower transition, but also a high resonance (a peak). Default is 2.')
Ls = col2.slider('Ls',
                0,
                60,
                30,
                help=f'VNF length in ms. Default is 30ms.')
M = col1.slider('M',
                0,
                10,
                8,
                help=f'Number of none zero impulses. Default is 8.')
LdB = col2.slider('LdB',
                -100,
                0,
                -20,
                help=f'The target decay in dB. Default is -20dB.')
gain = col1.slider('gw',
                0.01,
                0.99,
                0.5,
                help=f'The gain after the VNF. Default is 0.5.')

###############################################################################
st.markdown("#### Run The Pipeline")
audio_length = st.slider('Sample Length',
                        0.01,
                        1.0,
                        0.1,
                        help=f'The length of the audio to be cut down to. Default is 0.1s.')
data_cut = data_in[:, :int(sr_in * audio_length)]
st.write(f'One sample will be cut down to {audio_length}s, and repeat 50 times.')
submit = st.button('Run')
if submit:
    with st.spinner("Running..."):
        data_out_all = pipeline(data_cut, sr_in, fc, G, Q, Ls, M, LdB, gain)
        data_org_all = data_cut.copy()
        for _ in range(50):
            data_out_all = np.hstack([data_out_all, pipeline(data_cut, sr_in, fc, G, Q, Ls, M, LdB, gain)])
            data_org_all = np.hstack([data_org_all, data_cut])

    with tempfile.TemporaryDirectory() as tmpdirname:
        st.markdown('##### Original Audio')
        org_audio_file = os.path.join(tmpdirname, 'org.wav')
        sf.write(org_audio_file, data_org_all.T, sr_in)
        st.audio(org_audio_file, format='audio/wav')

        st.markdown('##### VNF Audio')
        vnf_audio_file = os.path.join(tmpdirname, 'vnf.wav')
        sf.write(vnf_audio_file, data_out_all.T, sr_in)
        st.audio(vnf_audio_file, format='audio/wav')

