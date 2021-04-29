# Bleep
Firmware for an Internet of Shit device that does *Beeps* when endpoints are requested.

## Todo:
- [x] Have a way to restrict `tone` range and return `4xx` error if not in range.
- [x] Have a way to restrict `duration` range and return `4xx` error if not in range.
- [ ] Implement a start/stop mechanism to be able to create melodies.
- [x] Make the [**GET** `/bleep/{tone}:{duration}`] *examples* section a piano keyboard.
- [ ] Use the SPIFFS to store the [**GET** `/`] markdown.
- [ ] Enhance the **README.md** file.
