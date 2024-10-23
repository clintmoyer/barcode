![banner](https://github.com/user-attachments/assets/d84a125f-8151-4772-b237-6e8af1c39838)

# UPC Barcode Generator

This C program generates a UPC-A barcode and displays it using the X11 library on macOS or Linux. The program uses no third-party libraries and follows the UPC-A encoding format as per GS1 standards.

## Features

- Generates UPC-A barcode with 12-digit input.
- Displays the barcode using X11 (XQuartz for macOS).
- No external libraries are required beyond X11.

## Prerequisites

- XQuartz (for macOS) or X11 (for Linux).
- X11 development libraries.

## Compilation

You can compile the `barcode.c` file using the following command:

```bash
gcc -o barcode barcode.c `pkg-config --cflags --libs x11`
```

## Usage

Once compiled, run the program by passing a 12-digit UPC-A code as an argument:

```bash
./barcode 036000291452
```

This will display the barcode for the given UPC-A code in an X11 window.

![screenshot](https://github.com/user-attachments/assets/f9b3de95-84e5-4e07-8686-96efc0055db0)

## Structure of UPC-A Barcode

- A UPC-A barcode is 12 digits long.
- It includes left and right guard patterns, center guard, and the encoded digits.

### Guard Patterns

- **Left Guard**: `101`
- **Center Guard**: `01010`
- **Right Guard**: `101`

### Encoding

Each digit is represented by a 7-bit binary pattern. The left and right sides use different encodings:
- **Left side encoding**: Odd parity.
- **Right side encoding**: Even parity.

## Notes

- The program does not validate the check digit, but UPC-A barcodes typically use a checksum to ensure data integrity.
- Ensure that XQuartz or X11 is running before executing the program.

