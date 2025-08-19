## Using SPI on the STM32MP157

### Setup

First you need to setup the board in STM32CubeIDE

1. Start a new project in STM32CubeIDE
2. If using a discovery board select your specific one otherwise select the STM32MP157 chip
3. In the Pinout & Configuration tab expand the connectivity dropdown menu and enable the SPI bus you want to use for the M4 core. This can be done by clicking the checkbox in the M4 core collumn in the row of the SPI bus you want to use. For this example we will be using SPI5.
![Selecting SPI5](/images/selectingSPI5.png)
4. Click on SPI5 to open the SPI5 Mode and Configuration menu. It should look somethign like this.
![Mode and Configuration Menu](/images/SPI5ModeAndConfigurationMenu.png)
5. Change the mode from *disabled* to whatever setting best fits your needs. For this example we will be using *Full-Duplex Master*.
6. If you want to use a specific pin as your chip select you can skip this step otherwise for this example change the Hardware NSS Signal from *disabled* to *Hardware NSS Output Signal*. Your SPI5 Mode and Configuration Screen should look somethign like this
![Mode and Configuration Screen final Setup](/images/SPI5ModeAndConfigurationFinalSetup.png)


Add the stuff abotu the pin being different from the datasheet for the discovery board and add how to fix it

Then explain the code