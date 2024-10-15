# Minimal STM32U5 Example

- Generate a project using STM32CubeMX for the STM32U5, configured like the Bristlemouth mote
- Create new source file `Core/Src/bm_example.c` with 2 functions: `bm_example_init()` and `bm_example_update()`
- Create new header file `Core/Inc/bm_example.h` with the same 2 function declarations
- Include `bm_example.h` in `main.c`
- Add a call to `bm_example_init()` before the main loop
- Add a call to `bm_example_update()` inside the main loop
- Add the headers from the `bm_netif` repo in a folder within `Core/Inc`
- Add the source from the `bm_netif` repo in a folder within `Core/Src`
- Create `Core/Src/bm_netif/hal.c` with platform-specific implementations following the example
- Be sure to set up the adin external interrupt handler to call the callback registered in hal.c
