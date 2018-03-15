# README

Il repository contiene software e datasheets per le componenti hardware utilizzate nella realizzazione di un prototipo che fa uso di:
* potenziomentro a slitta lineare motorizzato e touch-sensitive;
* Arduino UNO;
* Sparkfun "_Monster Moto shield_";

## Contenuto

* Nella cartella ```Arduino_code``` sono presenti gli sketch sviluppati durante il processo di prototipazione;
* La cartella ```Arduino_libraries``` continene le librerie utilizzate;
* la cartella ```Processing_code``` contiene il codice _Processing_ usato per testare la comunicazione seriale;
* la cartella ```datasheets``` contiene invece tutti i documenti _.pdf_ relativi all'hardware inpiegato;

## Touch sensing (Arduino Cap library)

Quando utilizziamo una libreria per il touch dobbiamo fare i conti con il fatto che alcuni dei metodi usati per il sensing occupano tempo macchina. Eseguono cioè operazioni che impediscono al ciclo _loop_ di lavorare ad altro.
Questo dipende molto da quanti cicli di _carica e scarica_ si fanno fare al pin analogico per campionarne il valore di tensione.

Da valutare la possibilità di utilizzare anche in questo caso un **MPR121** che, seppur sovradimensionato, potrebbe risolvere questo problema. L'MPR infatti si occuperebbe di eseguire tutti i cicli di _carica scarica_ senza inpiegare risorse del microcontrollore di Arduino, comunicandogli di tanto in tanto se sia avvenuto un tocco oppure no, magari via interrupt.

Inoltre l'uso dell'MPR121 peremetterebbe allo sistema di irrobustirsi contro il rumore ambientale e le false letture.

Di seguito alcune considerazioni sull librerie testater

* [Arduino cap library](https://playground.arduino.cc/Main/CapacitiveSensor?from=Main.CapSense) sembra essere molto dispendiosa in termini di tempo sottratto al ciclo loop. Il metodo _set_CS_Timeout_Millis_ sembra essere la causa del problema. La libreria richiede due pin digitali e alcuni componenti elettrici di contorno;
* [ADCTouch](https://github.com/martin2250/ADCTouch): sembra essere un po' meno precisa, ma più rapida (al massimo occupa 20ms). Non sembra essere troppo bloccante il ciclo loop. Maggiore è il quantitativo si samples che si richiede vengano campionati, maggiore il tempo impiegato dalla libreria per restituire il controllo al main. Usa un solo pin analogico anzichè due digitali.

## References and links

### Esempi e informazioni sull'utilizzo della Sparkfun Moto Shield

* [tutorial da instructables](http://www.instructables.com/id/Monster-Motor-Shield-VNH2SP30/)
* [tutorial](http://www.maffucci.it/2016/03/25/utilizzo-dellarduino-motor-shields-r3/) di Michele Maffucci;
* [fader motorizzato](https://blog.codyhazelwood.me/motorized-faders-and-the-arduino/)

### Current sensing theory
* [link](https://electronics.stackexchange.com/questions/70524/arduino-motor-shield-r3-current-sensing)
