# Bleep

Bleep is a Internet of Shit firmware for [ESP8266](https://en.wikipedia.org/wiki/ESP8266) boards, connected to a speaker. It does *Beeps* when somebody tickles it's endpoints.

## Features

* Connects to your home Wi-Fi
* Has an HTTP server
* Connects to a speaker
* Has a API to play the speaker
* Can be exposed to the public internet if you're courgeous enough.
* Your friends can now play music for you sometimes, over the internet.
  * CLI Client included !

## Installation

Connect a speaker of your choosing to the *D2* pin of your board.

You will need to compile the program and load it onto the board. It is assumed you know how to do this. You can find help in the [ESP8266 Arduino repository](https://github.com/esp8266/Arduino).

After you have cloned the repo with `git clone https://github.com/Nurrl/bleep`, you should pull the submodules with `git submodule update --init --recursive` and `git submodule foreach git pull origin master`. This will pull a [makefile for ESP8266 and ESP32 Arduino projects.](https://github.com/plerup/makeEspArduino).

You will need to create a file named `.env` in the bleep folder, and define the `IWSSID` and `IWPASSWD` variables. They are respectively the name of your Wi-Fi network and it's password.

Example :

```
# .env file
IWSSID="Wifi Network name"
IWPASSWD="Super secure password (please don't use this as a password.)"
```

The source code folder includes a Makefile. You can type `make` and the program should compile. If not, please open an issue.

### Partition

Partition is a super basic shell script that will take a file containing a pattern of notes, and send it to a server running this code. It's Bleep's CLI client. You can find it in the `scripts` folder.


## Usage

Once loaded onto the ESP8266, it will load a Serial terminal with a baud rate of `115200`, and start itself. It will first connect to the Wi-Fi network you defined, and try connecting to it. Once successful, it will start an HTTP server on port 80, with the following endpoints ;

**GET** `/`
*Get this **wonderful** explanatory page.*

**GET** `/playing/{status}`
*Start or stop the dequeuing of notes depending on the provided `status`.*

Examples:
- on — `/playing/1`
- off — `/playing/0`

**GET** `/beep/{tone}:{duration}`
*Emit a beep or enqueue a new one (up to 128 by default) if one is currently playing.*
*You might want to do a pause in your notes, simply provide a `tone` of `0Hz`.*

Parameters:
- `tone`: `int{0|30,12000}` — the desired tone, exprimed in **Hz**.
- `duration`: `int{50,4000}` — the desired duration, exprimed in **ms**.

For the same price, you get a piano keyboard at the end of the home page.

You can use the `curl` software to interact with it.

## Licence

I'm an **IoS**(for Internet of Shit) device that does *bleeps* and *bloops* on a buzzer
bleep  Copyright (C) 2021  Nurrl@github.com and contributors

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.

bleep  Copyright (C) 2021  Nurrl@github.com and contributors
This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
This is free software, and you are welcome to redistribute it
under certain conditions; type `show c' for details.
