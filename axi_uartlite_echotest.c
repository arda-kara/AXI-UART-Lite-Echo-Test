/*******************************************************************************
 * File: axi_uartlite_echotest.c
 *
 * Description: This program demonstrates minimal Tx and Rx testing of the UART
 *              Lite core v2.0. It accumulates data until the Enter key is
 *              pressed, then sends the entire string and echoes it back to
 *              the sender (e.g., Tera Term) at 9600 Baud, 8 data bits, no parity.
 *
 * 
 *
 * MODIFICATION HISTORY:
 * Ver   Who  Date        Changes
 * ----- ---- -------- -----------------------------------------------
 * 1.00a AA   29.07.24    First Release
 ******************************************************************************/

/***************************** Include Files *********************************/
#include "platform.h"
#include "xil_printf.h"
#include "xstatus.h"
#include "xparameters.h"
#include "xuartlite.h"

/************************** Constant Definitions *****************************/
#define UARTLITE_DEVICE_ID  XPAR_XUARTLITE_0_BASEADDR
#define BUFFER_SIZE         128

/************************** Function Prototypes ******************************/
static int UARTLite_Init_SelfTest(u16 DeviceID);
static void EchoData(void);

/************************** Variable Definitions *****************************/
static XUartLite UartLite0;

/*****************************************************************************/
/**
 * main
 *
 * Main function to test the UART Lite by echoing received data.
 *
 * @return XST_SUCCESS if successful, otherwise XST_FAILURE.
 *****************************************************************************/
int main()
{
    int UartLiteStatus;

    init_platform();

    UartLiteStatus = UARTLite_Init_SelfTest(UARTLITE_DEVICE_ID);
    if (UartLiteStatus != XST_SUCCESS) {
        xil_printf("UART Lite Initialization failed\n\r");
        cleanup_platform();
        return XST_FAILURE;
    }

    xil_printf("UART Lite Echo test started\n\r");

    EchoData();

    cleanup_platform();
    return XST_SUCCESS;
}

/*****************************************************************************/
/**
 * UARTLite_Init_SelfTest
 *
 * Initializes the UART Lite and performs a self-test.
 *
 * @param DeviceID: Device ID of the UART Lite from xparameters.h.
 *
 * @return XST_SUCCESS if successful, otherwise XST_FAILURE.
 *****************************************************************************/
static int UARTLite_Init_SelfTest(u16 DeviceID)
{
    int Status;

    // Perform initialization tests
    Status = XUartLite_Initialize(&UartLite0, DeviceID);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    // Perform self-test
    Status = XUartLite_SelfTest(&UartLite0);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
 * EchoData
 *
 * Echoes received data back to the sender after Enter key is pressed.
 *****************************************************************************/
static void EchoData(void)
{
    u8 RecvBuffer[BUFFER_SIZE];
    int NumBytesReceived = 0;
    int NumBytesSent = 0;

    while (1) {
        // Receive data until Enter key (carriage return) is pressed
        while (1) {
            int BytesRead = XUartLite_Recv(&UartLite0, &RecvBuffer[NumBytesReceived], 1);
            if (BytesRead > 0) {
                xil_printf("%c", RecvBuffer[NumBytesReceived]);  // Echo back as the user types

                // Check for Enter key (carriage return)
                if (RecvBuffer[NumBytesReceived] == '\r') {
                    RecvBuffer[NumBytesReceived] = '\n';  // Convert carriage return to newline
                    xil_printf("\n");  // Move to a new line in the terminal
                    break;
                }
                NumBytesReceived++;

                // Handle buffer overflow
                if (NumBytesReceived >= BUFFER_SIZE - 1) {
                    xil_printf("\n\rBuffer overflow\n\r");
                    NumBytesReceived = 0;
                    break;
                }
            }
        }

        // Null-terminate the received string
        RecvBuffer[NumBytesReceived] = '\0';

        // Echo received data back
        NumBytesSent = 0;
        while (NumBytesSent < NumBytesReceived) {
            int BytesWritten = XUartLite_Send(&UartLite0, &RecvBuffer[NumBytesSent], NumBytesReceived - NumBytesSent);
            if (BytesWritten > 0) {
                NumBytesSent += BytesWritten;
            }
        }

        // Reset buffer index for the next input
        NumBytesReceived = 0;
    }
}
