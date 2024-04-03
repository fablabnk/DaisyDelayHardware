To put together the schematic for the Daisy Delay, I studied the schematics of the [Daisy Patch](https://daisy.nyc3.cdn.digitaloceanspaces.com/products/patch/ES_Daisy_Patch_Rev4.pdf) which contains most of what we need to know.

Things I learned along the way:

- how to protect the module from overvoltage and inserting the 10-pin cable the wrong way around (reverse polarity protection). Q: how is overvoltage handled actually?

- how to power the daisy seed using Eurorack +-12V by converting it to +-5V

- how small value capacitors are used between +12V and -12V and GND at each stage of our circuit to reduce noise e.g.
    - directly as the +-12V comes in from our Eurorack power supply
    - again when the +12V is used at the ADC Input stage
    - again when the +12V and +3.3V_A is used at our Audio In/Out stage
    - ???Why is a capacitor placed in parallel with resistor over input/output of op amp?

- how to convert from the signal levels Eurorack modules expect to ones that the Daisy Seed expects

- how to use an analog circuit sum the potentiometer value with the CV input value for each parameter

- which values potentiometers to use? i.e. does it matter if we use 10K, 50K, 100K?

# Elements of the Schematic

## Daisy Seed

The pins on the Daisy Seed we will need are as follows:

| Pin Name     | Purpose                                                                                       |
| ------------ | --------------------------------------------------------------------------------------------- |
| AUDIO_IN_L   | Line-level audio input
| AUDIO_OUT_L  | Line-level audio output
| ADC_0        | Analog to Digital convertor pin for our 'Delay Time' potentiometer + CV Input
| ADC_1        | Analog to Digital convertor pin for our 'Feedback' potentiometer + CV Input
| ADC_2        | Analog to Digital convertor pin for our 'Delay Length Short/Long' switch
| DAC?         | LED
| V_IN         | Voltage input which we will feed with 5V (apparently 12V also okay but generate more heat)
| +3V3_A       | 3.3V Analog power, feeding our potentiometers
| +3V3_D       | Digital
| AGND         | Analog ground
| DGND         | Digital ground

Note: “D” for Digital GPIO or “A” for Analog I/O, depending on use case
Note: DGND and AGND must be connected to each other outside of the Seed.

Conveniently, a schematic symbol is already included in KiCad for the Daisy Seed Rev 4. We are now on Rev 7, but newer Daisy Seed revisions are pin to pin compatible with the old ones and were only made in order to replace unavailable audio codecs. See [here](https://daisy.nyc3.cdn.digitaloceanspaces.com/products/seed/Daisy_Seed_datasheet.pdf) for more details.

## ADC Inputs

The Daisy Patch has a small circuit for preparing each input potentiometer and CV input jack pair for analogue-to-digital conversion. The circuit serves two distinct purposes:

1. It sums the potentiometer output with the CV jack input

When making our VCVRack plugin I was doing this summing in code and could still do it this way.

2. It attenuates the summed signal from 10Vpp (+-5V) to +-3.3V

- We use an op amp to perform the attenuation
- We also need a stable 10V reference, which we get by using a component called a LM4040-10V Shunt Voltage Reference and which takes its input from our +12V rail

In our overall schematic we need two of the above circuits, one for our 'delay time' parameter and one for our 'feedback' parameter. These will come into the ADC_0 and ADC_1 pins, respectively.

In the Daisy.patch schematic an MCP6004 quad op amp is used offering input and output over the 1.8 - 6V range (why not 10V?). For our purposes a TL072 dual op amp should suffice, as we only have two parameters.

Note: The daisy patch uses an MPC6004, presumably to handle up to 8 inputs/outputs but maybe there's another reason i.e. can handle high voltages?

## Audio In/Out

We need to translate here from standard eurorack voltage levels (10V peak-to-peak) to line level (1-1.75Vpp) that the Daisy Seed audio input expects. We also need to do the opposite when the audio leaves the daisy seed. In practice we will use op amps to attenuate the signal coming in and boost it going out. Here we will use the same TL074 quad op amp chip as in the Daisy Seed, as each circuit requires two op amp stages (why?).

1. Audio In
- attentuates from Eurorack level to line level
- 100K resistor -> TL074 op amp 100K -> 100K resistor -> TL074 op amp 10K -> 1K resistor

2. Audio Out
- boosts from Eurorack level to line level
- 10K resistor -> TL074 op amp 100K -> 100K resistor -> TL074 op amp 100K -> 1K resistor

Note: I considered to share a 6 input op amp chip with our ADC Input section, but this cannot easily be done in practice as the former needs +-3.3V whilst this one needs +-12V

## Switch

- ADC is overkill here, GPIO makes more sense as our switch only has 0 (LOW) and 1 (HIGH) positions

- refer to the GPIO the example [here](https://electro-smith.github.io/libDaisy/md_doc_2md_2__a1___getting-_started-_g_p_i_o.html):

- use GPIO pin D0 (Daisy Seed pin 1)
- configured as an input
- configured use a pull up resistor
- my_button.Init(D0, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);
- [Switch class](https://electro-smith.github.io/libDaisy/classdaisy_1_1_switch.html#ad14a8ae20033d940d6c9eeb03c138)void daisy::Switch::Init(dsy_gpio_pin pin,float update_rate, Type t, Polarity pol, Pull pu) 
- hardware: connect one side of the switch to the GPIO input, and other side to GND


## LED

- use GPIO pin D1 (Daisy Seed pin 2)
- configured as an output
- pull up/down is not relevant here
- We connect one side of our LED to the GPIO input and the other side to GND and use a pull up resistor?
- my_led.Init(D1, GPIO::Mode::OUTPUT);
- [LED class](https://electro-smith.github.io/libDaisy/classdaisy_1_1_led.html)void daisy::Led::Init(dsy_gpio_pin pin, bool invert, float samplerate = 1000.0f)	
- hardware: D0 -> resistor -> + LED - -> GND

Determine the forward voltage drop and current rating of the LED you intend to use. This information is crucial for selecting an appropriate resistor to limit the current through the LED. using  Ohm's Law.

## Power

The power circuit of the Daisy Seed details some 

1. We take +-12V from our Eurorack power supply using a 10-pin connector

2. For both the +12V and -12V rails, we insert separately:
    - a ferrite bead (to filter out very high frequency interference) [see here](https://modwiggler.com/forum/viewtopic.php?t=91515&start=10) apparently important for CPU-based module
    - an S1B diode (for reverse polarity protection). This not the only approach e.g. 1N4001 diodes

3. We bridge both rails to GND using 10uF and 100nF capacitors

- this is for noise reduction

4. We send out to our internal +-12V rails

5. Providing 5V to power the Seed

- we perform isolator input filtering
- we somehow drop the signal to 5V before sending to +5V_ISOL to power the seed (VIN)
- Q: What does the Isolate block do here?

Apparently another approach would be to step down using a LM7805 

# Which symbols and footprint to use for Thonkiconn jacks

symbol: Connector:AudioJack2_SwitchT
footprint: Connector_Audio:Jack_3.5mm_QingPu_WQP-PJ398SM_Vertical_CircularHoles