# inkwave

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

* Currently doesn't work on big-endian architectures.
* Waveforms using 5 bits per pixel not yet supported
* For .wrf format currently only header is parsed

# Unsolved mysteries

## .wrf format

All mode pointers in the .wrf file need to be offset by 63 bytes. Likely has something to do with how they are passed by the epdc kernel module to the epdc. This is defined as `MYSTERIOUS OFFSET` in the code.

## .wbf format

Each waveform segment ends with two bytes that do not appear to be part of the waveform itself. The first is always `0xff` and the second is unpredictable. Unfortunately `0xff` can occur inside of waveforms as well so it is not useful as an endpoint marker. The last byte might be a sort of checksum but does not appear to be a simple 1-byte sum like other 1-byte checksums used in .wbf files.

## filesize zero

From the kernel it looks like sometimes header->filesize can be zero. If this is the case there is a different method for calculating the checksum. 

Look at `eink_get_computed_waveform_checksum` in `eink_waveform.c`.

# ToDo

Check `eink_waveform.c` in newer Kindle kernel releases. Might have more info.

# License and copyright

License: GPLv2. For full license text see the LICENSE file.

* Copyright 2018, 2021 Marc Juul
* Copyright 2005-2017 Amazon Technologies, Inc.
* Copyright 2004-2013 Freescale Semiconductor, Inc.

# Disclaimer

E Ink is a registered trademark of the E Ink Corporation.

Neither the E Ink Corporation, nor Freescale Semiconductor Inc. nor Amazon Technologies Inc are in any way affiliated with fread nor this git project nor any of the authors of this project and neither fread nor this git project is in any way endorsed by any of these organizations.

# Related projects
* [An impl in Python](https://github.com/KOLANICH-tools/inkwave.py)
