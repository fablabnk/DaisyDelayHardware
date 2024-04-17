
Q's?
- should LM4040 pin 3 be connected anywhere?
- what package should LM4040 have - what can we buy?
- what to do about switch - cut holes for it?
- panel layout
- don't forget thicker trace for power

# The process

- In schematic editor `Tools -> Edit Symbol Fields` helps to quickly and systematically assign footprints!
- Do we have a footprint for 2x8 Eurorack power connector? If not, make one!
    - closest generic is: Connector_PinHeader_2.54mm:PinHeader_2x08_P2.54mm_Vertical - using it for now
    - also could use the one from HagiwoMixer
- Need to order the following physical components
    - MCP6002
    - LM4040-10 - which package - SOT-23 or 

- LED diameter - 3mm/5mm? Note: Resistor value is dependent on desired brightness and/or LED part
- switch dimensions
- switch is not PCB mountable - what to do here? where to place on board, which footprint to use?
- breadboard the switch and LED as an example
- this time do a design rules check

# Multiple boards
The design will require two stacked PCBs

1. Mounting board
    - for pots, jacks etc
2. Seed board
    - for

- Components will be mounted onto the front side of board 1
- The panel will sit on top and be attached by thonkiconn screws
- On the rear side of board 1 there will be pin sockets to the left and right
- On the front side of board 2 there will be pin headers
- How many pins will be needed?
- On the rear side of board 2 there will left and right pin headers for the daisy seed to slot into
- There will also be the 16 pin power connector

It is suggested to place the pin sockets and headers on the schematic, but this makes the overall logic very unclear. Is there another way to do it?

https://forum.kicad.info/t/how-to-connect-modular-stacked-pcbs/16662/6

KiCad does not have real built-in support for panels yet.

A panel in PCB design refers to a larger substrate containing multiple individual PCBs arranged in a grid pattern for streamlined fabrication and assembly processes.

Each KiCad board file refers to a single PCB design. There is no official support for creating arrays or panels of PCBs within KiCad, although some community-created add-ons provide this functionality.

This thread offers a workflow
https://modwiggler.com/forum/viewtopic.php?t=184448

Just leave the connections between boards in the ratsnest, route everything else, then keep a manual checklist of how these go through the pin headers


Right now there looks like around 10 connections

How to offset the numbers on the right hand set of sockets/headers to avoid confusion?

- I could include the pin headers on the schematic with simple to/from labels, which wouldn't break the logic

# Panel Design

Layout will be like our VCVRack module here

- `Flip Board View` before