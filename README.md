# README

## Esempi e informazioni sull'utilizzo della Sparkfun Moto Shield

* [tutorial da instructables](http://www.instructables.com/id/Monster-Motor-Shield-VNH2SP30/)
* [tutorial](http://www.maffucci.it/2016/03/25/utilizzo-dellarduino-motor-shields-r3/) di Michele Maffucci;
* [fader motorizzato](https://blog.codyhazelwood.me/motorized-faders-and-the-arduino/)

## Current sensing theory
* [link](https://electronics.stackexchange.com/questions/70524/arduino-motor-shield-r3-current-sensing)

## Touch sensing (Arduino Cap library)

* [Arduino cap library](https://playground.arduino.cc/Main/CapacitiveSensor?from=Main.CapSense) seems to take control over the Arduino loop. The _set_CS_Timeout_Millis_ method seems to be the problem.
* [ADCTouch](https://github.com/martin2250/ADCTouch): sembra essere un po' meno precisa, ma più rapida (al massimo occupa 20ms). Non sembra essere troppo bloccante il ciclo loop. Maggiore è il quantitativo si samples che si richiede vengano campionati, maggiore il tempo impiegato dalla libreria per restituire il controllo al main. Usa un pin analogico, l'unico rimasto disponibile nella nostra configurazione.

Sotto questo punto di vista potrebbe valere la pensa di usare (anche se notevolmente sovradimensionato) un MPR121 che è di sicuro più robusto, e stabile. Il vantaggio è che usa un solo pin analogico.
