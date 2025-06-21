## gcc-competible startup arm file
https://github.com/STMicroelectronics/cmsis-device-f4.git: Source/Templates/gcc/startup_stm32f407xx.s

## elf/hex/bin

## Build openocd

```
./configure CFLAGS="-Wno-error=calloc-transposed-args" # Becaues it will encounter the build error. calloc related.
make -j6
make install
```

