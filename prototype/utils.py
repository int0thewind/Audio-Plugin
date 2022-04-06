import numpy.typing as npt
import numpy as np

import IPython.display
import numpy as np
import json
from scipy.io.wavfile import write

def print_audio_info(pretext: str, audio_data: npt.NDArray, sr: float):
    print(f'{pretext} sample.'
          f'Sample rate {sr}.'
          f'Data type {audio_data.dtype}.'
          f'Has {audio_data.ndim}'
          f'Has {np.max(audio_data.shape)} samples')


def Audio(audio: np.ndarray, sr: int):
    """
    Use instead of IPython.display.Audio as a workaround for VS Code.
    `audio` is an array with shape (channels, samples) or just (samples,) for mono.
    """

    if np.ndim(audio) == 1:
        channels = [audio.tolist()]
    else:
        channels = audio.tolist()

    return IPython.display.HTML("""
        <script>
            if (!window.audioContext) {
                window.audioContext = new AudioContext();
                window.playAudio = function(audioChannels, sr) {
                    const buffer = audioContext.createBuffer(audioChannels.length, audioChannels[0].length, sr);
                    for (let [channel, data] of audioChannels.entries()) {
                        buffer.copyToChannel(Float32Array.from(data), channel);
                    }
            
                    const source = audioContext.createBufferSource();
                    source.buffer = buffer;
                    source.connect(audioContext.destination);
                    source.start();
                }
            }
        </script>
        <button onclick="playAudio(%s, %s)">Play</button>
    """ % (json.dumps(channels), sr))

def write_audio(filename, sr, audio):
    # data = np.random.uniform(-1,1,44100)
    # 44100 random samples between -1 and 1
    # scaled = np.int16(data/np.max(np.abs(data)) * 32767)
    write(filename, sr, audio)
