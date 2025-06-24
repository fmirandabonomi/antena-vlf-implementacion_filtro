import numpy as np
import matplotlib.pyplot as plt
import matplotlib

matplotlib.use("qtagg")

filtro = np.loadtxt("build/salida_filtro.txt");
nco = np.loadtxt("build/salida_nco.txt",dtype=complex);

fs = 72000/(6*(12.5+41.5))

t = np.arange(0,filtro.size)/fs

plt.figure(1)
plt.plot(t,filtro);
plt.title("Respuesta en el tiempo del filtro demo")

def periodogram(signal):
    f=np.fft.fftfreq(signal.size,d=1/fs)
    d=np.abs(np.fft.fft(signal))**2/(signal.size*fs)
    f,d=np.fft.fftshift(f),np.fft.fftshift(d)
    d = 10*np.log10(d)
    return f,d

plt.figure(2)
plt.plot(*periodogram(filtro))
plt.title("Respuesta en frecuencia del filtro demo")
plt.xlabel("Pxx dB/Hz")


plt.figure(3)
plt.plot(t,np.real(nco),t,np.imag(nco))
plt.legend(["I","Q"])
plt.title("Señal de NCO demo en dominio del tiempo")

plt.figure(4)
plt.plot(*periodogram(nco))
plt.title("Señal de NCO demo en dominio de la frecuencia")
plt.xlabel("Pxx dB/Hz")

plt.show()
