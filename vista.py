import numpy as np
import scipy.signal as sg
import matplotlib.pyplot as plt

A = np.loadtxt("build/salida.txt")

fs = 72000/(6*(12.5+41.5))

t = np.arange(0,A.size)/fs

plt.figure(1)
plt.plot(t,A);

plt.figure(2)
f,d=sg.periodogram(A,fs=fs,return_onesided=False,detrend=False)
f,d=np.fft.fftshift(f),np.fft.fftshift(d)
plt.plot(f,d)

plt.show()
