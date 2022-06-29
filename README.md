*Under MIT license.*

[RoxXxtar.com/bmc](https://www.roxxxtar.com/bmc)

# RoxMux

RoxMux is a Collection of Multiplexer Controllers for some commonly used multiplexer chips. Compatible with Teensy and most Arduino boards.

## Support RoxMux Development
To support RoxMux Development you can can buy products from the BMC Shop @ [BadassMIDI.com](http://www.badassmidi.com)

RoxMux includes support for:

- [74HC165](docs/74HC165.md)
- [74HC595](docs/74HC595.md)
- [74HC4067](docs/74HC4067.md)
- [74HC4051](docs/74HC4067.md)
- [MCP23017](docs/MCP23017.md) *(supported as of version 1.1.4)*
- [MCP23018](docs/MCP23017.md) *(supported as of version 1.1.4)*

In Addition as of Version 1.1.3 RoxMux has a class to read buttons called RoxButton. This class
allows you to read a button connected directly to a pin or a multiplexer.
RoxMux also allows you to read/debounce and detect a Press, Release and if the button was held! As of Version 1.1.8 buttons can also detect a double press.


Version 1.1.4 adds support for encoders and pots reading.

- [RoxButton](docs/RoxButton.md) *(available as of version 1.1.3)*
- [RoxEncoder](docs/RoxEncoder.md) *(available as of version 1.1.4)*
- [RoxPot](docs/RoxPot.md) *(available as of version 1.1.4)*


Version 1.1.5 adds RoxLedMeter, a tool to create a VU Meter with Leds.

- [RoxLedMeter](docs/RoxLedMeter.md) *(available as of version 1.1.6)*

Version 1.1.9 adds `.latched()` and `.unlatched()` to RoxButton for latching switches, see the examples.

Version 1.2.1 adds support for the `TCA9548A` i2c Multiplexer!

- [RoxTCA9548A](docs/RoxTCA9548A.md) *(available as of version 1.2.1)*

Version 1.3.0 adds `RoxLed`, utility to blink/pulse leds.

- [RoxLed](docs/RoxLed.md) *(available as of version 1.3.0)*

Version 1.4.0 adds `RoxLatchingRelay` and `RoxNonLatchingRelay`, utility to blink/pulse leds.

- [RoxLatchingRelay](docs/RoxLatchingRelay.md) *(available as of version 1.4.0)*
- [RoxNonLatchingRelay](docs/RoxNonLatchingRelay.md) *(available as of version 1.4.0)*

Version 1.4.1 removes use of analogWrite when compiling for ESP32.

Version 1.4.2 adds `RoxLogLifo` a *Last In, First Out* circular buffer for logging messages.

- [RoxLogLifo](docs/RoxLogLifo.md) *(available as of version 1.4.1)*

Version 1.4.5 adds `.pulse()` method to `RoxLed`, it can be used instead of .on() if you only want to momentarily then turn off without continously pulsing it.

Version 1.4.6 adds `RoxMidiClock` and `RoxMidiActiveSensing` for MIDI use, these both can be used with any MIDI Port like Serial/USB/USB HOST, they are simple tools that doesn't use interrupts so I recommend using this with fast boards like Teensy 3/4.

- [RoxMidiClock](docs/RoxMidiClock.md) *(available as of version 1.4.6)*
- [RoxMidiActiveSense](docs/RoxMidiClock.md) *(available as of version 1.4.6)*

See examples for code information.

RoxMux containes multiple libraries in one, just include `<RoxMux.h>` to get started. All the libraries are templated to reduce RAM usage.

No matter which mux or utility you will be using, you always have to include `#include <RoxMux.h>` this file will include all the RoxMux controllers.
