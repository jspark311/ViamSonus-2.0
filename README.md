     __     ___                 ____                        
     \ \   / (_) __ _ _ __ ___ / ___|  ___  _ __  _   _ ___
      \ \ / /| |/ _` | '_ ` _ \\___ \ / _ \| '_ \| | | / __|
       \ V / | | (_| | | | | | |___) | (_) | | | | |_| \__ \
        \_/  |_|\__,_|_| |_| |_|____/ \___/|_| |_|\__,_|___/


An audio router board with integrated volume control and buffered output.

#### [Hackaday.io Page](https://hackaday.io/project/168501-viam-sonus-20)

#### [Order from Tindie](https://www.tindie.com/products/18586/)

------------------------

### What is in this repository:

**./doc**:  Location for documentation

**./KiCAD**:  Hardware design files

**./Arduino**:  Arduino driver

------------------------

### Usage under Arduino

In order to use this driver, you also need the Arduino drivers for the elements on the board. Clone the following repos into your user libraries (wherever you keep them):

    * [DS1881 Driver](https://github.com/jspark311/DS1881-Breakout/tree/master/Arduino)
    * [ADG2128 Driver](https://github.com/jspark311/ADG2128-Breakout/tree/master/Arduino)

After you've done that, you ought to be able to include the ViamSonus driver.

------------------------

### Usage under Linux

Fortunately, the Viam Sonus router board doesn't have any interrupt pins, which makes running it in userland fairly easy with existing kernel drivers for i2c and GPIO. The reset pin isn't _strictly_ required. It can be tied high.

------------------------

Front | Back
:-------:|:------:
![Front](osh-render-front.png)  | ![Back](osh-render-back.png)

[<img src="https://oshpark.com/assets/badge-5b7ec47045b78aef6eb9d83b3bac6b1920de805e9a0c227658eac6e19a045b9c.png" alt="Order from OSH Park">](https://oshpark.com/shared_projects/njoN7jnq)
