## Using SPI on the STM32MP157

### Preface

This document assumes you know basic terminology associated with the SPI bus.

This document only covers the basics of transmitting from the STM32MP157 while it is in *Full-Duplex Master* mode.

### Setup

First you need to setup the board in STM32CubeIDE

1. Start a new project in STM32CubeIDE
2. If using a discovery board select your specific one otherwise select the STM32MP157D chip
3. In the Pinout & Configuration tab expand the connectivity dropdown menu and enable the SPI bus you want to use for the M4 core. This can be done by clicking the checkbox in the M4 core collumn in the row of the SPI bus you want to use. For this example we will be using SPI5.
![Selecting SPI5](/images/selectingSPI5.png)
4. Click on SPI5 to open the SPI5 Mode and Configuration menu. It should look something like this.
![Mode and Configuration Menu](/images/SPI5ModeAndConfigurationMenu.png)
5. Change the mode from *disabled* to whatever setting best fits your needs. For this example we will be using *Full-Duplex Master*.
6. If you want to use a specific pin as your chip select you can skip this step otherwise for this example change the Hardware NSS Signal from *disabled* to *Hardware NSS Output Signal*. Your SPI5 Mode and Configuration Screen should look somethign like this
![Mode and Configuration Screen final Setup](/images/SPI5ModeAndConfigurationFinalSetup.png)
7. Below is the configuration screen for SPI5. This screen shows each of the GPIO pins and their current settings. For this example we will need to change the SPI5_SCK pin to be PF7.
![Initial Settings](/images/initialGPIOPinSettings.png)
8. If pin PF7 is already assigned to SPI5_SCK then you can skip this step. In the Pinout view click pin PF7 and select the option *SPI5_SCK*. This may reset any changes done to SPI5 if so those will need to be redone.
9. In the configuration screen select SPI5_SCK and in the menu below set the *Maximum Output Speed* to *Very High*.
![Proper PF7 Configuration](/images/properPF7Configuration.png)
10. Next we will be changing the parameter settings. Change the following:
    - Set Data Size to 8 Bits
    - Set Prescaler (for Baud Rate) to 128

When you atre finished it should look like the following
![Setting SPI5 Parameters](/images/settingSPI5Parameters.png)
11.  Save and regenerate code

### Code

Due to the code generation of STM32CubeIDE all initialization of the SPI5 Bus is completed. 

To transmit as if the STM32MP157 is the Master device use this function

```
HAL_SPI_Transmit(hspi, pData, Size, Timeout)
```