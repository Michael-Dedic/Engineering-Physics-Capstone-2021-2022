# -*- coding: utf-8 -*-
def FFT_Algorithm(ANALOG):
    #analog_values = []  #analog values from microphone
    t=[]                #time list used for FFT, should remain constant
    f=[]                #frequency list, should remain constant
    amp_peaks = []      # peaks ampltidues list, this will change each time
    freq_peaks=[]       #frequency peaks, this will change each time
    import scipy.fft  
    import matplotlib.pyplot as plt
    import numpy as np
    #import pandas as pd
    from scipy.signal import find_peaks

    #setup---------------------------------------------------------

    Fs = 2048;           #Sampling frequency                    
    T = 1/Fs             #Sampling period                
    L = 512            #Length of signal

    for i in range(L-1):
        t.append(i*T)

    for i in range(int(L/2)):
        f.append(float(Fs*i/L))
        
    y= ANALOG   
    #Simple Test case    
    #x = np.linspace(0.0, L*T, L)
    #y = np.sin(440.0 * 2.0*np.pi*x) + 0.5*np.sin(100 * 2.0*np.pi*x)   
    #-----------------------------------------------------------------
    #FFT--------------------------------------------------------------   
    #y_f = np.fft.fft(y)
    y_f = scipy.fft.fft(y)

    P2 = abs(y_f/L)
    P1 = P2[0:int(L/2)]
    P1[2:]=2*P1[2:]

    plt.plot(f[2:],P1[2:])
    plt.xlabel("Frequency(Hz)", fontsize = 10)
    plt.ylabel("Amplitude", fontsize = 10)
    plt.show()
    #-------------------------------------------------------------------
    #-------------------------------------------------------------------
    #DSP for finding frequency values with the highest amplitude
    height_threshold=1.3

    amplitudes_1D= P1.flatten()
    peaks_index, properties = find_peaks(amplitudes_1D, height=1.3)

                
                   
    for i in range(len(peaks_index)):
        if f[peaks_index[i]]>=200 and f[peaks_index[i]]<=500:
            amp_peaks.append(amplitudes_1D[peaks_index[i]])
            freq_peaks.append(f[peaks_index[i]])
                            

    max_amp = max(amp_peaks)
    print(max_amp)
    max_amp_index= amp_peaks.index(max_amp)
    print(max_amp_index)

    peak_freq = freq_peaks[max_amp_index]

    print(peak_freq)
    return peak_freq