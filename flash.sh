openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program build/bootloader.elf verify reset exit"
