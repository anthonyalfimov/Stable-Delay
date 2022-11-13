# Stable Delay [WIP]

<img width="800" alt="SD_01" src="https://user-images.githubusercontent.com/43878921/201531579-16da9006-94e0-44b5-ae5d-84a222fe6f15.png">

This plugin is a **work in progress**.

The current goal for the project is to design a delay that:

1. Produces runaway feedback that doesn't infinitely increase in volume, but has constant loudness close to that of the input signal (*hence, Stable Delay*).
2. Offers drive that is completely independent from the input signal level.

A prototype version of the plugin that achieves these goals can be found on the **`dsp-prototype`** branch. It is still under development, so **test at your own risk**.

<img width="800" alt="SD_02" src="https://user-images.githubusercontent.com/43878921/201532009-ef344af6-6777-4b3f-95d3-ec787896ed4f.png">

---

## Building

JUCE version: 7.0.2

C++17

---

This project has started as coursework for the "[Advanced Audio Plugin Development](https://www.kadenze.com/courses/advanced-audio-plugin-development)" course by [Output](https://output.com).
