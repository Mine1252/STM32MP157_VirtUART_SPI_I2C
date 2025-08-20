## Using I2C on the STM32MP157

### Preface

This document assumes you know basic terminology associated with the I2C bus.

### Setup

First you need to setup the board in STM32CubeIDE

1. Start a new project in STM32CubeIDE
2. If using a discovery board select your specific one otherwise select the STM32MP157D chip
3. In the Pinout & Configuration tab expand the connectivity dropdown menu and enable the I2C bus you want to use for the M4 core. This can be done by clicking the checkbox in the M4 core collumn in the row of the I2C bus you want to use. For this example we will be using I2C1.
![Selecting I2C1](/images/selectingI2C1.png)
4. Click on I2C1 to open the I2C1 Mode and Configuration menu. It should look something like this.
![I2C1 Mode and Configuration Screen](/images/I2C1ModeAndConfigurationMenu.png)
5. Change the mode from *disabled* to whatever setting best fits your needs. For this example we will be using *I2C*.
6. Save and Regenerate Code

### Code

To transmit on the I2C bus you can use the following function.

```
HAL_I2C_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout);
```