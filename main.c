
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

/*

# format info

The following is format info gleaned from open source header files that hasn't yet been incorporated into this program.

## update modes

* 0x00: INIT (panel initialization)
* 0x01: MU/DU (direct update, 1bpp)
* 0x02: GU/GC4/GC16 (greyscale update / grayscale clear 2bpp / or 4bpp)
* 0x03: GC16_FAST / A2 (grayscale clear 4bpp / ?)
* 0x04: AU (animation update, 1bpp)
* 0x05: GL (white transition, grayscale clear, 4bpp)
* 0x06: GL_FAST/GLF (text to text, grayscale clear, 4bpp)
* 0x07: DU4 (text to text)
* 0x08: REAGL (non-flashing update)
* 0x09: REAGLD (non-flashing update with dithering)

*/

typedef struct {
  uint32_t key;
  const char val[64];
} Pair;

Pair mfg_codes[] = {
  {0x33, "ED060SCF (V220 6\" Tequila)"},
  {0x34, "ED060SCFH1 (V220 Tequila Hydis – Line 2)"},
  {0x35, "ED060SCFH1 (V220 Tequila Hydis – Line 3)"},
  {0x36, "ED060SCFC1 (V220 Tequila CMO)"},
  {0x37, "ED060SCFT1 (V220 Tequila CPT)"},
  {0x38, "ED060SCG (V220 Whitney)"},
  {0x39, "ED060SCGH1 (V220 Whitney Hydis – Line 2)"},
  {0x3A, "ED060SCGH1 (V220 Whitney Hydis – Line 3)"},
  {0x3B, "ED060SCGC1 (V220 Whitney CMO)"},
  {0x3C, "ED060SCGT1 (V220 Whitney CPT)"},
  {0xA3, "LB060S03-RD02 (LGD Tequila Line 1)"},
  {0xA4, "2nd LGD Tequila Line"},
  {0xA5, "LB060S05-RD02 (LGD Whitney Line 1)"},
  {0xA6, "2nd LGD Whitney Line"}
};
unsigned int mfg_codes_size = sizeof(mfg_codes) / sizeof(Pair);

Pair run_types[] = {
  {0x00, "[B]aseline"},
  {0x01, "[T]est/trial"},
  {0x02, "[P]roduction"},
  {0x03, "[Q]ualification"},
  {0x04, "V110[A]"},
  {0x05, "V220[C]"},
  {0x06, "D"},
  {0x07, "V220[E]"},
  {0x08, "F"},
  {0x09, "G"},
  {0x0A, "H"},
  {0x0B, "I"},
  {0x0C, "J"},
  {0x0D, "K"},
  {0x0E, "L"},
  {0x0F, "M"},
  {0x10, "N"}
};
unsigned int run_types_size = sizeof(run_types) / sizeof(Pair);

Pair fpl_platforms[] = {
  {0x00, "2.0"},
  {0x01, "2.1"},
  {0x02, "2.3"},
  {0x03, "V110"},
  {0x04, "V110A"},
  {0x06, "V220"},
  {0x07, "V250"},
  {0x08, "V220E"}
};
unsigned int fpl_platforms_size = sizeof(fpl_platforms) / sizeof(Pair);

Pair fpl_sizes[] = {
  {0x32, "5\", unknown resolution"},
  {0x3C, "6\", 800x600"},
  {0x3D, "6.1\", 1024x768"},
  {0x3F, "6\", 800x600"},
  {0x50, "8\", unknown resolution"},
  {0x61, "9.7\", 1200x825"},
  {0x63, "9.7\", 1600x1200"}
};
unsigned int fpl_sizes_size = sizeof(fpl_sizes) / sizeof(Pair);

Pair fpl_rates[] = {
  {0x50, "50Hz"},
  {0x60, "60Hz"},
  {0x85, "85Hz"}
};
unsigned int fpl_rates_size = sizeof(fpl_rates) / sizeof(Pair);

Pair mode_versions[] = {
  {0x00, "MU/GU/GC/PU (V100 modes)"},
  {0x01, "DU/GC16/GC4 (V110/V110A modes)"},
  {0x02, "DU/GC16/GC4 (V110/V110A modes)"},
  {0x03, "DU/GC16/GC4/AU (V220, 50Hz/85Hz modes)"},
  {0x04, "DU/GC16/AU (V220, 85Hz modes)"},
  {0x06, "? (V220, 210 dpi, 85Hz modes)"},
  {0x07, "? (V220, 210 dpi, 85Hz modes)"}
};
unsigned int mode_versions_size = sizeof(mode_versions) / sizeof(Pair);


Pair waveform_types[] = {
  {0x0B, "TE"},
  {0x0E, "WE"},
  {0x15, "WJ"},
  {0x16, "WK"},
  {0x17, "WL"},
  {0x18, "VJ"},
  {0x2B, "WR"}
};
unsigned int waveform_types_size = sizeof(waveform_types) / sizeof(Pair);


Pair waveform_tuning_biases[] = {
  {0x00, "Standard"},
  {0x01, "Increased DS Blooming V110/V110E"},
  {0x02, "Increased DS Blooming V220/V220E"}
};
unsigned int waveform_tuning_biases_size = sizeof(waveform_tuning_biases) / sizeof(Pair);

// get human readable descriptor
const char* get_desc(Pair table[], unsigned int table_size,  unsigned int key, const char* def) {
  int i;

  for(i=0; i < table_size; i++) {
    if(table[i].key == key) {
      return table[i].val;
    }
  }
  return def;
}

const char* get_desc_mfg_code(unsigned int mfg_code) {
  const char* desc = get_desc(mfg_codes, mfg_codes_size, mfg_code, NULL);

  if(desc) return desc;

  if(mfg_code >= 0x33 && mfg_code < 0x3c) {
    return "PVI/EIH panel\0";
  } else if(mfg_code >= 0xA0 && mfg_code < 0xA8) {
    return "LGD panel\0";
  }

  return "Unknown code\0";
}

struct waveform_data_header {
	unsigned int checksum:32; // 0
	unsigned int filesize:32; // 4
	unsigned int serial:32; // 8 serial number
  unsigned int run_type:8; // 12
  unsigned int fpl_platform:8; // 13
  unsigned int fpl_lot:16; // 14
  unsigned int mode_version_or_adhesive_run_num:8; // 16
  unsigned int waveform_version:8; // 17
  unsigned int waveform_subversion:8; // 18
  unsigned int waveform_type:8; // 19
  unsigned int fpl_size:8; // 20
  unsigned int mfg_code:8; // 21
  unsigned int waveform_tuning_bias_or_rev:8; // 22
  unsigned int fpl_rate:8; // 23
  unsigned int unknown0:32; // 24
	unsigned int xwia:24; // extra waveform information
	unsigned int cs1:8; // checksum 1
	unsigned int wmta:24;
	unsigned int fvsn:8;
	unsigned int luts:8;
	unsigned int mc:8; // mode count (length of mode table - 1)
	unsigned int trc:8; // temperature range count (length of temperature table - 1)
	unsigned int reserved0_0:8;
	unsigned int eb:8;
	unsigned int sb:8;
	unsigned int reserved0_1:8;
	unsigned int reserved0_2:8;
	unsigned int reserved0_3:8;
	unsigned int reserved0_4:8;
	unsigned int reserved0_5:8;
	unsigned int cs2:8; // checksum 2
};

void print_header(struct waveform_data_header* header) {
  printf("File size (according to header): %d bytes\n", header->filesize);
  printf("Serial number: %d\n", header->serial);
  printf("Run type: 0x%x | %s\n", header->run_type, get_desc(run_types, run_types_size, header->run_type, "Unknown"));
  printf("Manufacturer code: 0x%x | %s\n", header->mfg_code, get_desc_mfg_code(header->mfg_code));

  printf("Frontplane Laminate (FPL) platform: 0x%x | %s\n", header->fpl_platform, get_desc(fpl_platforms, fpl_platforms_size, header->fpl_platform, "Unknown"));
  printf("Frontplane Laminate (FPL) lot: %d\n", header->fpl_lot);
  printf("Frontplane Laminate (FPL) size: 0x%x | %s\n", header->fpl_size, get_desc(fpl_sizes, fpl_sizes_size, header->fpl_size, "Unknown"));
  printf("Frontplane Laminate (FPL) rate: 0x%x | %s\n", header->fpl_rate, get_desc(fpl_rates, fpl_rates_size, header->fpl_rate, "Unknown"));

  printf("Waveform version: %d\n", header->waveform_version);
  printf("Waveform sub-version: %d\n", header->waveform_subversion);

  printf("Waveform type: 0x%x | %s\n", header->waveform_type, get_desc(waveform_types, waveform_types_size, header->waveform_type, "Unknown"));

  // if waveform_type is WJ or earlier 
  // then waveform_tuning_bias_or_rev is the tuning bias.
  // if it is WR type or later then it is the revision.
  // if it is in between then we don't know.
  if(header->waveform_type <= 0x15) { // WJ type or earlier
    printf("Waveform tuning bias: 0x%x | %s\n", header->waveform_tuning_bias_or_rev, get_desc(waveform_tuning_biases, waveform_tuning_biases_size, header->waveform_tuning_bias_or_rev, "Unknown"));
    printf("Waveform revision: Unknown\n");    
  } else if(header->waveform_type >= 0x2B) { // WR type or later
    printf("Waveform tuning bias: Unknown\n");
    printf("Waveform revision: %d\n", header->waveform_tuning_bias_or_rev);
  } else {
    printf("Waveform tuning bias: Unknown\n");
    printf("Waveform revision: Unknown\n");
  }

  // if fpl_platform is < 3 then 
  // mode_version_or_adhesive_run_num is the adhesive run number
  if(header->fpl_platform < 3) {
    printf("Adhesive run number: %d\n", header->mode_version_or_adhesive_run_num);
    printf("Mode version: Unknown\n");
  } else {
    printf("Adhesive run number: Unknown\n");
    printf("Mode version: 0x%x | %s\n", header->mode_version_or_adhesive_run_num, get_desc(mode_versions, mode_versions_size, header->mode_version_or_adhesive_run_num, "Unknown"));
  }

  printf("Number of modes in this waveform: %d\n", header->mc + 1);
  printf("Number of temperature ranges in this waveform: %d\n", header->trc + 1);

}

int check() {

  // TODO:
  // * check filesize from header against real file size
  // * check checksum
}

void usage(FILE* fd) {
  fprintf(fd, "Usage: inkwave filename.wbf -o [output.wrf]\n");
}


int main(int argc, char **argv) {

  char* infile_path;
  FILE* infile;
  size_t len;
  struct waveform_data_header header;

  if(argc != 2) {
    usage(stderr);
    return 1;
  }

  infile_path = argv[1];

  infile = fopen(infile_path, "r");
  if(!infile) {
    fprintf(stderr, "Opening file %s failed: %s\n", infile_path, strerror(errno));
    return 1;
  }

  len = fread(&header, 1, sizeof(struct waveform_data_header), infile);
  if(len <= 0) {
    fprintf(stderr, "Reading file %s failed: %s\n", infile_path, strerror(errno));
    return 1;
  }

  print_header(&header);

  return 0;
}
