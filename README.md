
inkwave is a command-line utility for converting `.wbf` to `.wrf` files and displaying meta-data information from `.wbf` and `.wrf` files in a human readable format.

`.wbf` is the format stored on the flash chip present on the ribbon cable of some electronic paper displays made by the E Ink Corporation and `.wrf` is the input format used by the i.MX 508 EPDC (electronic paper display controller) and possibly the EPDCs of later i.MX chipsets.

In order to make full use of these displays it is necessary to read the `.wbf` data from the SPI flash chip, convert it to `.wrf` format and then pass it to the EPDC kernel module.

# Compiling

```
make
```

# Usage

```
inkwave file.wbf/file.wrf [-o output.wrf]

  Convert a .wbf file to a .wrf file
  or if no output file is specified display human
  readable info about the specified .wbf or .wrf file.

Options:

  -o: Specify output file.

  -f wrf/wbf: Force inkwave to interpret input file
              as either .wrf or .wbf format
              regardless of file extension.

  -h: Display this help message.
```

# Limitations

Currently doesn't work on big-endian architectures.

# License and copyright

License: GPLv2. For full license text see the LICENSE file.

* Copyright 2018 Marc Juul
* Copyright 2005-2017 Amazon Technologies, Inc.
* Copyright 2004-2013 Freescale Semiconductor, Inc.

# Disclaimer

E Ink is a registered trademark of the E Ink Corporation.

Neither the E Ink Corporation, nor Freescale Semiconductor Inc. nor Amazon Technologies Inc are in any way affiliated with fread nor this git project nor any of the authors of this project and neither fread nor this git project is in any way endorsed by any of these organizations.