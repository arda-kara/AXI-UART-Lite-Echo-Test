# AXI UART Lite Echo Test

## Overview

This repository contains a simple echo test program for the AXI UART Lite core using Vitis. The program initializes the UART Lite, performs a self-test, and then enters a loop where it echoes back any data received from the terminal. The communication parameters are set to 9600 Baud, 8 data bits, no parity. Block design source files are available under `src`.

## Features

- Minimal Tx and Rx testing of the UART Lite core v2.0
- Accumulates data until the Enter key is pressed
- Sends the entire string and echoes it back to the sender
- Handles buffer overflow scenarios

## Requirements

- Vitis
- AXI UART Lite core v2.0
- UART terminal software (e.g., Tera Term)
- Nexys A7-100t board

## Setup

1. Clone the repository to your local machine.
2. Open Vitis and create a new platform project.
3. Import the provided `.xsa` file to create a platform component for the Nexys A7-100t board.
4. Create a new application project and add the `axi_uartlite_echotest.c` source file to the application component.
5. Build the project.

## Usage

1. Program the FPGA with the generated bitstream.
2. Run the application from Vitis.
3. Open a UART terminal (e.g., Tera Term) with the following settings:
   - Baud rate: 9600
   - Data bits: 8
   - Parity: None
   - Stop bits: 1
   - Flow control: None
4. Configure the COM port using the following commands if needed:
   ```
   MODE COMX: 9600,N,8,1
   ```
   Replace `X` with the appropriate COM port number.
5. Type characters in the terminal. The characters written will be sent back when you press Enter.

## Function Descriptions

### `main`

Main function to test the UART Lite by echoing received data. It initializes the platform, performs the UART Lite initialization and self-test, and then starts the echo test.

### `UARTLite_Init_SelfTest`

Initializes the UART Lite and performs a self-test.

**Parameters:**
- `DeviceID`: Device ID of the UART Lite from xparameters.h.

**Returns:**
- `XST_SUCCESS` if successful, otherwise `XST_FAILURE`.

### `EchoData`

Echoes received data back to the sender after the Enter key is pressed. It handles receiving data, checking for buffer overflow, and sending the echoed data back.


## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
