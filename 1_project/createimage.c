/* Author(s): <Your name here>
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
// more defines...

/* Reads in an executable file in ELF format

Identifies and creates the ELF header and the program header. The last parameter points to
the pointer that will contain the ELF header.

Signature was FILE **execfile, char *filename, Elf32_Ehdr **ehdr
*/
Elf32_Phdr * read_exec_file(FILE **execfile, Elf32_Ehdr *ehdr){
  
  // Insert from bytes into ELF header object
  //unsigned char *buffer[4];
  fseek(*execfile, ehdr->e_phoff, SEEK_SET);
  Elf32_Phdr *program_hdr = malloc(sizeof(Elf32_Phdr));
  fread(program_hdr, sizeof(char),ehdr->e_phentsize, *execfile);

  return program_hdr;
}

/* Writes the bootblock to the image file */
void write_bootblock(FILE **imagefile,FILE *bootfile,Elf32_Ehdr *boot_header, Elf32_Phdr *boot_phdr){
  
  // read from adress p_offset, p_filesz number of bytes


  // Start reading at offset given by program header
  fseek(bootfile, boot_phdr->p_offset, SEEK_SET);

  // read p_filesz bytes
  char bootblock_segment[boot_phdr->p_filesz];
  fread(&bootblock_segment, sizeof(char), boot_phdr->p_filesz, bootfile);

  // createimage.given starts with 0000 0900. Why? This must be kernel size
  bootblock_segment[2]=9;

  // print to image
  for (int i=0;i<boot_phdr->p_filesz;i++) {
    fprintf(*imagefile, "%c", (int)bootblock_segment[i]);
  }

  // pad the rest to 0x200, except last two bytes
  int file_size = boot_phdr->p_filesz;
  while (file_size % SECTOR_SIZE != SECTOR_SIZE-2) {
    fprintf(*imagefile, "%c", 0);
    file_size++;
  }
  // last two bytes must be 55aa to indicate boot disk
  fprintf(*imagefile, "%c", 0x55);
  fprintf(*imagefile, "%c", 0xaa);

}

/* Writes the kernel to the image file */
void write_kernel(FILE **imagefile,FILE *kernelfile,Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
  
  // read p_filesz bytes
  char kernel_segment[kernel_phdr->p_filesz];
  fread(&kernel_segment, sizeof(char), kernel_phdr->p_filesz, kernelfile);

  // createimage.given starts with 0000 0900. Why? This must be kernel size
  //bootblock_segment[2]=9

  // print to image
  for (int i=0;i<kernel_phdr->p_filesz;i++) {
    fprintf(*imagefile, "%c", (int)kernel_segment[i]);
  }

  // pad the rest to multiple of SECTOR_SIZE
  int file_size = kernel_phdr->p_filesz;
  while (file_size % SECTOR_SIZE != 0) {
    fprintf(*imagefile, "%c", 0);
    file_size++;
  }
 
}

/* Counts the number of sectors in the kernel */
int count_kernel_sectors(Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
  int size = kernel_phdr->p_filesz;
  return (size / SECTOR_SIZE + 1);
}

/* Records the number of sectors in the kernel */
void record_kernel_sectors(FILE **imagefile,Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr, int num_sec){
    
}


/* Prints segment information for --extended option */
void extended_opt(Elf32_Phdr *bph, Elf32_Phdr *kph, int num_sec){
  
  
  /* print number of disk sectors used by the image */
  printf("Number of used disk segments: %d \n", num_sec);
  
  /*bootblock segment info */
 

  /* print kernel segment info */
  

  /* print kernel size in sectors */
}

int main(int argc, char **argv){
  FILE *kernelfile, *bootfile,*imagefile;  //file pointers for bootblock,kernel and image
  Elf32_Ehdr *boot_header = malloc(sizeof(Elf32_Ehdr));//bootblock ELF header
  Elf32_Ehdr *kernel_header = malloc(sizeof(Elf32_Ehdr));//kernel ELF header

  Elf32_Phdr *boot_program_header; //bootblock ELF program header
  Elf32_Phdr *kernel_program_header; //kernel ELF program header

  /* create image file */
  imagefile = fopen("image", "wb");

  /* read executable bootblock file */ 
  bootfile = fopen("bootblock", "rb");

  // read the header immediately
  // This works because the bytes in the ELF32_Ehdr object is set corecctly
  fread(boot_header, sizeof(char), sizeof(Elf32_Ehdr), bootfile);
  // use the header to get the program header
  boot_program_header = read_exec_file(&bootfile, boot_header);
  /* write bootblock */  
  write_bootblock(&imagefile, bootfile, boot_header, boot_program_header);

  /* read executable kernel file */
  kernelfile = fopen("kernel", "rb");
  // read the header immediately like bootblock
  fread(kernel_header, sizeof(char), sizeof(Elf32_Ehdr), kernelfile);
  // use the header to get the program header
  kernel_program_header = read_exec_file(&kernelfile, kernel_header);

  /* write kernel segments to image */
  write_kernel(&imagefile, kernelfile, kernel_header, kernel_program_header);

  /* tell the bootloader how many sectors to read to load the kernel */

  int num_sectors = count_kernel_sectors(kernel_header, kernel_program_header);

  /* check for  --extended option */
  if(argc != 1) {
    if(!strncmp(argv[1],"--extended",11)){
        extended_opt(boot_program_header, kernel_program_header, num_sectors);
    }
  }


  fclose(imagefile);
  fclose(bootfile);
  fclose(kernelfile);
  
  return 0;
} // ends main()



