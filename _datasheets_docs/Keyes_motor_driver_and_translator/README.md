
## Reference

La piccola PCB rossa si chiama **StepStick** prodotta da _reprap_.
E' una nuova versione migliorata ed ispirata a [questo prodotto](https://www.pololu.com/product/1182) della Pololu. Maggiori informazioni al riguardo si trovano a [questo link](http://reprap.org/wiki/Stepstick).

Come il prodotto della Pololu, anche questa PCB fa uso dell IC [A4988](https://www.allegromicro.com/en/Products/Motor-Driver-And-Interface-ICs/Bipolar-Stepper-Motor-Drivers/A4988.aspx) della Allegro.

L'A4988 di fatto è un **driver** con **translator** integrato!

Limite di corrente a 1A;



## Note originali tratte dal sito Keyes

* nome prodotto: A4988 stepper motor driver control panel + A4988 stepper motor driver
* [link](http://en.keyes-robot.com/productshow.aspx?id=217)

A4988 is a complete microstepping motor driver with built-in translator for easy operation.
The product is available in full, half , 1/4, 1/8 and 1/16 step mode to operate _bipolar stepper motors_ of an output drive capability of up to 35V and ±2A.

A4988 includes a fixed off-time current regulator, the regulator can operate in slow or mixed decay modes.
Converter is easy to implement key A4988. Just enter a pulse in the "step" input drives the motor to produce microstep .
Without the need for phase sequence tables, high frequency control lines, or complex interfaces to program.
A4988 interface is ideal for complex microprocessor which is unavailable or overloaded applications.

### More

In the microstep operation, the chopper control A4988 automatically selects the current decay mode (slow or mixed).

In mixed decay mode, the device is initially set to a fixed part of the rapid decay in downtime and slow decay for the rest of downtime. Mixed decay current control scheme results in reduced audible motor noise, increased step accuracy, and reduced power consumption. Internal synchronous rectification control circuitry provides to improve the pulse-width modulation (PWM) operation power consumption. Internal circuit protection includes:
* thermal shutdown with hysteresis,
* undervoltage lockout (UVLO):
* crossover current protection.

No need special power sequencing.

A4988 using surface mount QFN package (ES), size of 5 mm × 5 mm, nominal overall package height of 0.90 mm, with exposed pad for enhanced thermal dissipation. This package is Pb-free (suffix -T), with 100% matte tin leadframe plating.
