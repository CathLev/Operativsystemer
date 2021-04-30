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
#define WRITE_MODE "w"
#define READ_MODE "r"

#define ELF_32_EHDR_SIZE 52
#define SECTOR_SIZE 512       /* floppy sector size in bytes */
#define BOOTLOADER_SIG_OFFSET 0x1fe /* offset for boot loader signature */
#define KERNEL_SECTOR_OFFSET 0x02 /* offset for recording of kernel sectors */
// more defines...

/* Reads in an executable file in ELF format and returns the program header*/
Elf32_Phdr * read_exec_file(FILE **execfile, char *filename, Elf32_Ehdr **ehdr){
  Elf32_Phdr *phdr = malloc(sizeof(Elf32_Phdr)*sizeof(char)); //Allocates memory for program header
  fseek(*execfile, (*ehdr)->e_phoff, SEEK_SET); //Sets cursor at program header offset
  int test =fread(phdr, (sizeof(char)), sizeof(Elf32_Phdr), *execfile); // Writes phdr
  assert(test == sizeof(Elf32_Phdr)); // checks that correct number of bytes have been written

  return phdr;
}

/*Writes n_bytes of padding into **imagefile at current cursor position*/
void write_padding(FILE **imagefile, int n_bytes){
  int test =0; 

  while(n_bytes-- >0){ 
    test = fprintf(*imagefile, "%c", 0); // Writes 0-character as padding
    assert(test==1); // checks that writing has succeeded
  }

}

/* Writes the bootblock to the image file */
void write_bootblock(FILE **imagefile,FILE *bootfile,Elf32_Ehdr *boot_header, Elf32_Phdr *boot_phdr){
  char * buffer[boot_phdr->p_filesz];
  int test = 0;

  fseek(bootfile,boot_phdr->p_offset,SEEK_SET); // sets cursor in source file


  /* Read from bootfile to buffer */
  // check if correct use of filesz
  test = fread(buffer, sizeof(char), boot_phdr->p_filesz, bootfile);
  assert(test == boot_phdr->p_filesz);

  /* Write to imagefile from buffer*/
  test = fwrite(buffer, sizeof(char), boot_phdr->p_filesz, *imagefile);
  assert(test == boot_phdr->p_filesz);

  //write padding to ensure filesize multiple of 512 -2
  int remainder = (SECTOR_SIZE - boot_phdr->p_filesz % SECTOR_SIZE)-2;
  write_padding(imagefile, remainder);

  // Writes special characters to end bootsector
  test = fprintf(*imagefile, "%c", 0x55); 
  assert(test==1); 
  test = fprintf(*imagefile, "%c", 0xaa); 
  assert(test==1); 
}


/* Writes the kernel to the image file */
void write_kernel(FILE **imagefile,FILE *kernelfile,Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
  char * buffer[kernel_phdr->p_filesz];
  int test;
  int padding;

  fseek(kernelfile,kernel_phdr->p_offset,SEEK_SET); // sets cursor in source file

  /* Read from bootfile to buffer */
  // check if correct use of filesz
  test = fread(buffer, sizeof(char), kernel_phdr->p_filesz, kernelfile);
  assert(test == kernel_phdr->p_filesz);

  /* Write to imagefile from buffer*/
  test = fwrite(buffer, sizeof(char), kernel_phdr->p_filesz, *imagefile);
  assert(test == kernel_phdr->p_filesz);

  //write padding to ensure filesize multiple of 512
  padding = (SECTOR_SIZE - kernel_phdr->p_filesz % SECTOR_SIZE);
  write_padding(imagefile, padding);


 
}

/* Counts the number of sectors in the kernel */
int count_kernel_sectors(Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
    int kernel_size; // Total size of kernel
    int num_sectors;
    int padding;

    /*Get size of kernel*/
    for(int i =0; i < kernel_header->e_phnum; i++){
        kernel_size += (kernel_phdr[i]).p_memsz;
    }

    padding = (SECTOR_SIZE - kernel_size % SECTOR_SIZE); // Find out how much padding is used for the kernel sectors in image
    num_sectors = (kernel_size + padding)/SECTOR_SIZE;

    return num_sectors;
}

/* Records the number of sectors in the kernel */
void record_kernel_sectors(FILE **imagefile,Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr, int num_sec){
    int recorded = num_sec << 0; //ensures endianness
    fseek(*imagefile, KERNEL_SECTOR_OFFSET, SEEK_SET);
    fwrite(&recorded,sizeof(int),1, *imagefile);
}


/* Prints segment information for --extended option */
void extended_opt(Elf32_Phdr *bph, int k_phnum, Elf32_Phdr *kph, int num_sec){

  /* print number of disk sectors used by the image */
  
  
  /*bootblock segment info */
 

  /* print kernel segment info */
  

  /* print kernel size in sectors */
  printf("OS size: %d sectors ", num_sec);
}

/* MAIN */
int main(int argc, char **argv){
  FILE *kernelfile, *bootfile,*imagefile;  //file pointers for bootblock,kernel and image
  Elf32_Ehdr *boot_header = malloc(sizeof(Elf32_Ehdr));//bootblock ELF header
  Elf32_Ehdr *kernel_header = malloc(sizeof(Elf32_Ehdr));//kernel ELF header

  Elf32_Phdr *boot_program_header; //bootblock ELF program header
  Elf32_Phdr *kernel_program_header; //kernel ELF program header

  int num_sec;
  char *kernel_filename;
  char *boot_filename;
  int extended = 0;

  /*Process commandline input*/
  /*Check correct number of arguments*/
  assert(argc >=3 || argc <= 5);

  /*Extract file names*/
  kernel_filename = argv[argc-1];
  boot_filename = argv[argc-2];

  /*Extract options --extended and --vm*/
  for(int i = 1; i < argc-2; i++){
    if(!strncmp(argv[1],"--extended",11)){
      extended = 1;
    }
    if (!strncmp(argv[1],"--vm",5)){
      printf("Option \"--vm\" is not implemented.");
    }
  }
  
  /*Open input files*/
  kernelfile = fopen(kernel_filename, READ_MODE);
  assert(kernelfile != NULL);
  bootfile = fopen(boot_filename, READ_MODE);
  assert(bootfile != NULL);

  /*Create image file */
  imagefile = fopen(IMAGE_FILE, WRITE_MODE);
  assert(imagefile != NULL);

  /*Read executable bootblock file */  
  fread(boot_header, sizeof(char), sizeof(Elf32_Ehdr), bootfile);
  boot_program_header = read_exec_file(&bootfile, boot_filename, &boot_header);
  assert(boot_program_header != NULL);


  /* write bootblock */  
  write_bootblock(&imagefile,bootfile,boot_header, boot_program_header);

  /* read executable kernel file */
  fread(kernel_header, sizeof(char), sizeof(Elf32_Ehdr), kernelfile);
  kernel_program_header = read_exec_file(&kernelfile, kernel_filename, &kernel_header);
  assert(kernel_program_header != NULL);

  /* write kernel segments to image */
  write_kernel(&imagefile,kernelfile,kernel_header, kernel_program_header);

  /* tell the bootloader how many sectors to read to load the kernel */
  num_sec = count_kernel_sectors(kernel_header, kernel_program_header);
  assert(num_sec != 0);
  record_kernel_sectors(&imagefile,kernel_header, kernel_program_header, num_sec);
  /* check for  --extended option */
  if(extended){
	  /* print info */
    //extended_opt(boot_program_header, kernel_header->e_phnum, kernel_program_header, num_sec);
  }
  
  /* Close files upon completing the image*/
  fclose(kernelfile);
  fclose(bootfile);
  fclose(imagefile);

  /*Frees allocated memory */
  //free(boot_header);
  //free(kernel_header);
  //free(boot_program_header);
 // free(kernel_program_header);


  return 0;
} // ends main()



