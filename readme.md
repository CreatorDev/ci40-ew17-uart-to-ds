![logo](https://static.creatordev.io/logo-md-s.svg)

# Ci40-ew17-uart-to-ds

This project is part of the Creator demo built for the Embedded World 2017 show. The demo consists of a pair of conveyor belts moving coloured blocks in a loop, a board detects the colour of blocks and the data is pushed to a cloud application to store the colour and update a multicolour LED bluetooth blub with the last scanned colour. 

The colour of the block is detected using a colour sensor fitted to a Clicker board which is made available over UART, this project allows you to connect the clicker into ci40's UART. The Ci40 uses AWALwM2M to make an IPSO object of the detected block colour. To have this working you also need awa-client bootstraped to DS. In the "script" folder you have simple scripts that defines proper IPSO object on awa-client. Please call it before each run of program from this repository.

To connect things together please use wires: on ci40 use 3.3V, GND, TX, RX from
mikrobus 2. On contiki side please connect to UART lines next to battery pack
on the edge of clicker board.



Other components of the EW17 Creator demo are

* [Clicker application](https://github.com/CreatorDev/clicker-ew-demo) that detects the current block colour and echo the value over UART
* [A javascript application](https://github.com/CreatorDev/webapp-ew17) that receives notifications when new colours are detected, stores them to a database, and pushes bulb-colour value changes
* [Ci40 application](https://github.com/CreatorDev/Ci40-ew17-bulb) that receives bulb-colour value changes and issues the corresponding colour change instruction to the bulb over bluetooth
* A python application on a Mediatek 7688 Duo that drives the motors for the conveyor belt when a user presses the stop/start button, or when an inactivity timeout triggers

---

## Help

If you have any problems installing or utilising this project, please look into 
our [Creator Forum](https://forum.creatordev.io). 

Otherwise, If you have come across a nasty bug or would like to suggest new 
features to be added then please go ahead and open an issue or a pull request 
against this repo.

## License

Please see the [license](LICENSE) file for a detailed explanation.

## Contributing

Any Bug fixes and/or feature enhancements are welcome. Please see the 
[contributing](CONTRIBUTING.md) file for a detailed explanation.

--- 