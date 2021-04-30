/* Author(s): John McSpedon <mcspedon>, Rob Sami <rsami> 
 * COS 318, Fall 2013: Project 1 Bootloader
 * Creates operating system image suitable for placement on a boot disk
*/
/* Largely unimplemented */
#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMAGE_FILE "./image"
#define ARGS "[--extended] [--vm] <bootblock> <executable-file> ..."

#define SECTOR_SIZE 512       /* floppy sector size in bytes */
#define BOOTLOADER_SIG_OFFSET 0x1fe /* offset for boot loader signature */
#define KERNEL_SECTOR_OFFSET 2 /* offset for writing number of kernel sectors */
// more defines...

// private fxn called to write both bootblock and kernel
void write_to_image(FILE **imagefile, FILE *readfile, Elf32_Ehdr *elf_hdr, Elf32_Phdr *prog_hdr);

/* Reads in an executable file in ELF format*/
Elf32_Phdr * read_exec_file(FILE **execfile, char *filename, Elf32_Ehdr **ehdr){
  size_t numread, num_phdr;
  Elf32_Phdr *phdr; // return value
  
  //open file
  if(!(*execfile = fopen(filename, "r"))){
    fprintf(stderr, "Error reading %s: %s\n", filename, strerror(errno));
    exit(EXIT_FAILURE);
  }
  // read in ELF header
  numread = fread(*ehdr, 1, sizeof(Elf32_Ehdr), *execfile);
  assert(numread == sizeof(Elf32_Ehdr));
  num_phdr = (**ehdr).e_phnum;
  
  /* read in program headers */
    // check we're assuming correct struct
  assert((**ehdr).e_phentsize == sizeof(Elf32_Phdr)); 
    //set read position to beginning of program header table
  fseek(*execfile, (**ehdr).e_phoff, SEEK_SET);
    // allocate memory to store program header(s)
  phdr = calloc(sizeof(Elf32_Phdr), num_phdr); //ELF Program header
    // perform read
  numread = fread(phdr, sizeof(Elf32_Phdr), num_phdr, *execfile);
  assert(numread == num_phdr);
  
  return phdr;
}

/* Writes the bootblock to the image file */
void write_bootblock(FILE **imagefile, FILE *bootfile, Elf32_Ehdr *boot_header, Elf32_Phdr *boot_phdr){
  /* set file offsets */
  fseek(*imagefile, 0, SEEK_SET);
  write_to_image(imagefile, bootfile, boot_header, boot_phdr);

  /* write signature to end of first sector */
  fseek(*imagefile, BOOTLOADER_SIG_OFFSET, SEEK_SET);
  fputc(0x55, *imagefile);
  fputc(0xAA, *imagefile);
}

/* Writes the kernel to the image file */
void write_kernel(FILE **imagefile, FILE *kernelfile, Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
  /* set file offset to beginning of second segment */
  fseek(*imagefile, SECTOR_SIZE, SEEK_SET);
  write_to_image(imagefile, kernelfile, kernel_header, kernel_phdr); 
}

 /* NOTE: assumes imagefile is open at desired offset.
    For each program header, copies code segment then pads (memsz - progsize) bytes.
    At end, pads to nearest sector */
void write_to_image(FILE **imagefile, FILE *readfile, Elf32_Ehdr *elf_hdr, Elf32_Phdr *prog_hdr) {
  size_t required_padding; // padding (in bytes) required to bring write to the end of a sector in image
  size_t totalread = 0; // total bytes read thusfar
  size_t numremaining, num2read, numread; //for current pidx: number of bytes remaining to r/w, to r/w this turn, and to r/w during this loop
  int pidx; //which program header we're on
  char buffer[SECTOR_SIZE]; // buffer for copying a sector's worth of bytes

  // for each program header
  for(pidx = 0; pidx < (*elf_hdr).e_phnum; pidx++) {
    /* populate variables specific to this program header */
    numremaining = prog_hdr[pidx].p_filesz;
    required_padding = prog_hdr[pidx].p_memsz - numremaining;
    fseek(readfile, prog_hdr[pidx].p_offset, SEEK_SET); // this is where first byte of program resides

    /* copy code segment */
    while(numremaining > 0) {
      // read bytes   ( num2read = min(numremaining, SECTOR_SIZE) )
      num2read = (numremaining < SECTOR_SIZE) ? numremaining : SECTOR_SIZE;
      numread = fread(buffer, 1, num2read, readfile);
      assert(numread == num2read);
      // write bytes
      numread = fwrite(buffer, 1, num2read, *imagefile);
      assert(numread == num2read);
      // update counters
      totalread += numread;
      numremaining -= numread;
    }
    /* pad up to memsz */     
    while(required_padding > 0) {
      fputc(0, *imagefile);
      required_padding--;
      totalread++;
    }
  }
  /* pad to next sector */
  required_padding = (SECTOR_SIZE - totalread) % SECTOR_SIZE;
  while(required_padding > 0) {
    fputc(0, *imagefile);
    required_padding--;
  }
}

/* Counts the number of sectors in the kernel */
int count_kernel_sectors(Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr) {
  size_t kernel_size = 0;
  int pidx;
  for(pidx = 0; pidx < (*kernel_header).e_phnum; pidx++) {
    kernel_size += kernel_phdr[pidx].p_memsz;
  }
  return (int) kernel_size / SECTOR_SIZE + ((kernel_size % SECTOR_SIZE == 0)? 0 : 1);
}

/* Records the number of sectors in the kernel */
void record_kernel_sectors(FILE **imagefile, Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr, int num_sec) {
    /* write over bytes 3-6 inclusive of file */
    fseek(*imagefile, KERNEL_SECTOR_OFFSET, SEEK_SET);
    fwrite(&num_sec, sizeof(int), 1, *imagefile);
}


/* Prints segment information for --extended option */
void extended_opt(Elf32_Phdr *bph, int k_phnum, Elf32_Phdr *kph, int num_sec){
  // have assumed we can hardcode names by piazza post @90
  char *boot_fname = "./bootblock";
  char *kernel_fname = "./kernel";
  int pidx;
  size_t sim_address;

  /* print number of disk sectors used by the image */
  printf("image_size: %d sectors\n", num_sec);
  
  
  /*bootblock segment info */
    //print address? and fname
  pidx = 0;
  printf("0x%4x:\t%s\n", bph[pidx].p_vaddr, boot_fname);
  printf("\tsegment %d\n", pidx);
  printf("\t\t offset 0x%4x\t\tvaddr 0x%4x\n", bph[pidx].p_offset, bph[pidx].p_vaddr);
  printf("\t\t filesz 0x%4x\t\tmemsz 0x%4x\n", bph[pidx].p_filesz, bph[pidx].p_memsz);
  printf("\t\t writing 0x%4x bytes\n", bph[pidx].p_filesz);
  sim_address = ((bph[pidx].p_memsz - 1) / SECTOR_SIZE  + 1) * SECTOR_SIZE;
  printf("\t\t padding up to 0x%4x\n", sim_address);
 

  /* print kernel segment info */
  printf("0x%4x:\t%s\n", kph[pidx].p_vaddr, kernel_fname);
  for(pidx = 0; pidx < k_phnum; pidx++) {
    printf("\tsegment %d\n", pidx);
    printf("\t\t offset 0x%4x\t\tvaddr 0x%4x\n", kph[pidx].p_offset, kph[pidx].p_vaddr);
    printf("\t\t filesz 0x%4x\t\tmemsz 0x%4x\n", kph[pidx].p_filesz, kph[pidx].p_memsz);
    printf("\t\t writing 0x%4x bytes\n", kph[pidx].p_filesz);
    sim_address += kph[pidx].p_memsz;
    if (pidx == k_phnum -1) sim_address = ((sim_address - 1) / SECTOR_SIZE + 1) * SECTOR_SIZE;
    printf("\t\t padding up to 0x%4x\n", sim_address);
  }

  /* print kernel size in sectors */
  printf("os_size: %d sectors\n", num_sec-1);
}

// more helper functions...

/* MAIN */
// ignore the --vm argument when implementing (project 1)
int main(int argc, char **argv){
 /* argv = (argc == 3/4)
 * 	-/1: '--extended'
 * 	1/2: './bootblock'
 * 	2/3: './kernel'
 */

  FILE *kernelfile, *bootfile,*imagefile;  //file pointers for bootblock,kernel and image
  Elf32_Ehdr *boot_header = malloc(sizeof(Elf32_Ehdr));//bootblock ELF header
  Elf32_Ehdr *kernel_header = malloc(sizeof(Elf32_Ehdr));//kernel ELF header

  Elf32_Phdr *boot_phdr; //bootblock ELF program header
  Elf32_Phdr *kernel_phdr; //kernel ELF program header
  int num_sectors;

  /* check for legal num of args */
  if (argc != 3 && argc !=4) {
	fprintf(stderr, "Error: call as '%s (--extended) bootblock-location kernel-location'\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* open image file for writing*/
  if(!(imagefile = fopen(IMAGE_FILE, "w+"))) {
    fprintf(stderr, "Error creating %s: %s\n", IMAGE_FILE, strerror(errno));
    exit(EXIT_FAILURE);
  }


  /* read executable bootblock file   
   *     note that here filename = argv[argc-2]; */
  boot_phdr = read_exec_file(&bootfile, argv[argc-2], &boot_header);

  /* write bootblock */  
  write_bootblock(&imagefile, bootfile, boot_header, boot_phdr);

  /* read executable kernel file
   *     note that here filename = argv[argc-1]; */
  kernel_phdr = read_exec_file(&kernelfile, argv[argc-1], &kernel_header);

  /* write kernel segments to image */
  write_kernel(&imagefile, kernelfile, kernel_header, kernel_phdr);

  /* tell the bootloader how many sectors to read to load the kernel */
  num_sectors = count_kernel_sectors(kernel_header, kernel_phdr);
  record_kernel_sectors(&imagefile, kernel_header, kernel_phdr, num_sectors);


  /* check for  --extended option */
  if(!strncmp(argv[1],"--extended",11)){
	  extended_opt(boot_phdr, (int) (*kernel_header).e_phnum, kernel_phdr, num_sectors + 1);
  }
  
  // Free Memory
  free(boot_header);
  free(kernel_header);
  free(boot_phdr);
  free(kernel_phdr);

  // Close Files
  fclose(kernelfile);
  fclose(bootfile);
  fclose(imagefile);
  
  return 0;
} // ends main()