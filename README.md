# Yet another yet another stm32 bootloader.

Work in progress.

STM32F1x series microcontrollers are the only supported now.

1. A bit more than absolute minimum of something that could be called bootloader.
2. Zero safety features, just provides the ability to update firmware without using stlink/jtag.
3. Self-written very simple protocol, simple checksum, each packet is exactly 64 bytes, for ease of use with USB in future.
4. Only USART is supported atm.

Kinda usable already, although bugs might happen.

