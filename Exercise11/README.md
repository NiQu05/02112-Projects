# Exercise 11 code
This project demonstrates basic GPIO and timer functionality using the ESP-IDF framework on an ESP32. It includes features for controlling an LED through GPIO pins, implementing delays, and measuring time elapsed during tasks.

## 1. One-Shot Timer:

An LED connected to GPIO pin 8 is turned ON for a duration defined by `DELAY` (5 seconds).
After the delay, the LED turns OFF.


## 2. High-Resolution Timer (HRT):

Measures the time taken to blink the LED 5 times, each with a 500 ms interval.
The elapsed time is printed in microseconds.
The features can be toggled using the `BLINK` and `HRT` macros.