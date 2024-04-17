This repo documents the hardware design of a Daisy Seed-based Eurorack module called the Daisy Delay and contains the KiCad files for the project.

The Daisy Delay is planned as simple 6HP mono delay-line module, but can easily be used as a development platform for any kind of single-in/single-out audio processor.

This README gives an overview of the project, but there are also seperate notes for the [schematic](notes_schematic.md) and the [PCB](notes_pcb.md).

The module was first prototyped in VCVRack, before before being converted back to code that will run on the Seed. For more details on the VCVRack plugin, see [here](https://github.com/fablabnk/VaseProtoPlugin/tree/delay_line_params).

# Components / Bill of Materials (BOM)

The 

- 2 x 10K (for now) potentiometers
- 1 x SPST mini switch
- 1 x small LED (? diameter)
- 4 x Thonkiconn mono switching jack sockets (PJ301M-12)
- 1 x TL072 op amp
- 1 x TL074 op amp
- 1 x Shrouded 10 pin power connector
- various resistors
- various ceramic capacitors

# Workflow

Below is the basic workflow I am following to develop the module. 

1. Design schematic
- symbol and footprint for Seed Rev4 are already included in KiCad - search for "electrosmith"

2. Design front panel in Kicad PCB editor
- use VCV plugin panel .svg as template
- laser cut on paper / thin card first to test fit

3. Layout and Route PCB
- one board which drops onto components
- laser cut on paper / thin card first to test fit
- 2 layer / 4 layer routing?

4. Convert code to seed format
- test code on Pod

5. Send for fabrication (PCBWay)
- Order 5 x front and back panels

# Useful Links

https://github.com/clacktronics/Eurorack_KiCAD_templates

# Concerns / To Do

- Simpler hardware module first...probably!
- Symbol is only for Seed Rev 4 and we are now on Rev 7, but each Daisy Seed is pin to pin compatible with one another 
- Revisions were only to replace depricated audio codecs. See here: https://daisy.nyc3.cdn.digitaloceanspaces.com/products/seed/Daisy_Seed_datasheet.pdf