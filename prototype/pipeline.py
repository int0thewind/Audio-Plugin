import numpy as np
import numpy.typing as npt
from shelving import Class_IIR_Shelving
from velvetnoise import velvet_noise
import scipy.signal as signal


def pipeline(data: npt.NDArray, sr, fc, G, Q, Ls, M, LdB, gain) -> npt.NDArray:
    iir = Class_IIR_Shelving(fc=fc, G=G, Q=Q, fs=sr)
    data_iir = iir.filtering(data)

    vn = velvet_noise(sr, Ls, M, LdB)
    data_vn = signal.lfilter(vn, [1], data_iir)

    data_vn *= gain

    return data_vn.astype(data.dtype) + data

