import numpy.typing as npt
import numpy as np


def print_audio_info(pretext: str, audio_data: npt.NDArray, sr: float):
    print(f'{pretext} sample. '
          f'Sample rate {sr}. '
          f'Data type {audio_data.dtype}. '
          f'Has {audio_data.ndim} channels. '
          f'Has {np.max(audio_data.shape)} samples.')
