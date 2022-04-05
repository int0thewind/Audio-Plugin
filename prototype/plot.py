import numpy as np
import matplotlib.pyplot as plt


def plot_freqz(w, h):
    """
    A helper function to acquire a plot directly from `scipy.signal.freqz` function.
    """
    fig, ax1 = plt.subplots()

    ax1.set_title('Digital filter frequency response')
    ax1.plot(w, 20 * np.log10(abs(h)), 'b')
    ax1.set_ylabel('Amplitude [dB]', color='b')
    ax1.set_xlabel('Frequency [rad/sample]')

    ax2 = ax1.twinx()
    angles = np.unwrap(np.angle(h))
    ax2.plot(w, angles, 'g')
    ax2.set_ylabel('Angle (radians)', color='g')
    ax2.grid()
    ax2.axis('tight')


def no_output(*args):
    """
    Helper function to coerce no output in jupyter notebook environment
    """
    pass
