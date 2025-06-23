import numpy as np
import matplotlib.pyplot as plt
import matplotlib

matplotlib.use("qtagg")

A = np.loadtxt("build/salida.txt")

fs = 72000/(6*(12.5+41.5))

t = np.arange(0,A.size)/fs

plt.figure(1)
plt.plot(t,A);

plt.figure(2)
d = np.fft.fftshift(np.abs(np.fft.fft(A)))/(A.size)
f = np.fft.fftshift(np.fft.fftfreq(A.size,1/fs))
plt.plot(f,d)

plt.show()
