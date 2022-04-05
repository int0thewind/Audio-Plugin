# https://www.dsprelated.com/showcode/170.php

import matplotlib.pyplot as plt
import numpy as np
from scipy import signal


class Class_IIR_Shelving(object):
    def __init__(self, fc=2500, G=2.0, Q=1.0, fs=48000, shelf_type='Base_Shelf'):
        """
        %
        % Derive coefficients for a shelving filter with a given amplitude and
        % cutoff frequency.  All coefficients are calculated as described in 
        % Zolzer's DAFX book (p. 60 - 65).  
        %
        % Usage:     [B,A] = shelving(G, Fc, Fs, Q, type);
        %
        %            G is the logrithmic gain (in dB)
        %            Fc is the center frequency (in Hz)
        %            Fs is the sampling rate (in Hz)
        %            Q adjusts the slope be replacing the sqrt(2) term
        %            type is a character string defining filter type
        %                 Choices are: 'Base_Shelf' or 'Treble_Shelf'
        %
        % Author: Ruibin Yuan, 04/05/2022
        % Modified from: sparafucile17, 08/22/05
        %
        """
        # design iir Shelving filter
        # initalize
        self.fc = fc
        self.fs = fs
        self.G = G
        self.Q = Q
        support_shelf_type= ['Base_Shelf', 'Treble_Shelf']
        assert shelf_type in support_shelf_type, 'shelf_type must be in {}'.format(support_shelf_type)
        self.shelf_type = shelf_type
        self.b, self.a = self.set_shelving()

    def filtering(self, xin):
    	# process filtering, using scipy
    	# input xin
    	# output filtered xin
        return signal.lfilter(self.b, self.a, xin)
        
    def f_show(self, worN=1024):
        # draw frequency response, using scipy
        w, h = signal.freqz(self.b, self.a, worN=worN)
        print(w.max(), w.min())
        
        fig = plt.figure(figsize=(12,12))
        ax1 = fig.add_subplot(111)
        flist = w / ((2.0 * np.pi) / self.fs)
        plt.title('frequency response')
        
        plt.semilogx(flist, 20 * np.log10(abs(h)), 'b')  
        # plt.plot(flist, 20 * np.log10(abs(h)), 'b')
        
        plt.xlabel('Frequency [Hz]')
        plt.ylabel('Amplitude [dB]', color='b')
        plt.grid()
        
        ax2 = ax1.twinx()
        angles = np.unwrap(np.angle(h))
        angles = angles / ((2.0 * np.pi) / 360.0)
        plt.semilogx(flist, angles, 'g')  
        # plt.plot(flist, angles, 'g')
        plt.ylabel('Angle(deg)', color='g')
        
        plt.axis('tight')
        plt.show()

    def set_shelving(self,):
        
        K = np.tan(np.pi * self.fc / self.fs)
        V0 = np.power(10.0, self.G/20.0)
        root2 = 1/self.Q # sqrt(2)

        # Invert gain if a cut
        if (V0 < 1):
            V0 = 1 / V0

        ####################
        #    BASE BOOST
        ####################
        if self.shelf_type == 'Base_Shelf' and self.G>0:
            b0 = (1 + np.sqrt(V0)*root2*K + V0*K**2) / (1 + root2*K + K**2)
            b1 =                (2 * (V0*K**2 - 1) ) / (1 + root2*K + K**2)
            b2 = (1 - np.sqrt(V0)*root2*K + V0*K**2) / (1 + root2*K + K**2)
            a1 =                   (2 * (K**2 - 1) ) / (1 + root2*K + K**2)
            a2 =                (1 - root2*K + K**2) / (1 + root2*K + K**2)

        ####################
        #    BASE CUT
        ####################
        elif self.shelf_type == 'Base_Shelf' and self.G<0:
            b0 =                (1 + root2*K + K**2) / (1 + root2*np.sqrt(V0)*K + V0*K**2)
            b1 =                   (2 * (K**2 - 1) ) / (1 + root2*np.sqrt(V0)*K + V0*K**2)
            b2 =                (1 - root2*K + K**2) / (1 + root2*np.sqrt(V0)*K + V0*K**2)
            a1 =                (2 * (V0*K**2 - 1) ) / (1 + root2*np.sqrt(V0)*K + V0*K**2)
            a2 = (1 - root2*np.sqrt(V0)*K + V0*K**2) / (1 + root2*np.sqrt(V0)*K + V0*K**2)

        ####################
        #    TREBLE BOOST
        ####################
        elif self.shelf_type == 'Treble_Shelf' and self.G>0:
            b0 = (V0 + root2*np.sqrt(V0)*K + K**2) / (1 + root2*K + K**2)
            b1 =                (2 * (K**2 - V0) ) / (1 + root2*K + K**2)
            b2 = (V0 - root2*np.sqrt(V0)*K + K**2) / (1 + root2*K + K**2)
            a1 =                 (2 * (K**2 - 1) ) / (1 + root2*K + K**2)
            a2 =              (1 - root2*K + K**2) / (1 + root2*K + K**2)

        ####################
        #    TREBLE CUT
        ####################
        elif self.shelf_type == 'Treble_Shelf' and self.G<0:
            b0 =                  (1 + root2*K + K**2) / (V0 + root2*np.sqrt(V0)*K + K**2)
            b1 =                     (2 * (K**2 - 1) ) / (V0 + root2*np.sqrt(V0)*K + K**2)
            b2 =                  (1 - root2*K + K**2) / (V0 + root2*np.sqrt(V0)*K + K**2)
            a1 =                (2 * ((K**2)/V0 - 1) ) / (1 + root2/np.sqrt(V0)*K + (K**2)/V0)
            a2 = (1 - root2/np.sqrt(V0)*K + (K**2)/V0) / (1 + root2/np.sqrt(V0)*K + (K**2)/V0)
        
        ####################
        #    All-Pass
        ####################
        else:
            b0 = V0
            b1 = 0
            b2 = 0
            a1 = 0
            a2 = 0
        
        a = np.array([1, a1, a2])
        b = np.array([b0, b1, b2])

        return b, a

if __name__ == '__main__':
    
    # high shelf filter sample 
    iir_LS1=Class_IIR_Shelving( fc=50, G=-20, Q=0.75, fs=44100, shelf_type='Base_Shelf')
    # draw frequency response
    iir_LS1.f_show(8192)