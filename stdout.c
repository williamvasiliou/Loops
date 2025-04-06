#include <elf.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define E_PHOFF sizeof(Elf64_Ehdr)
#define E_PHNUM 4
#define P_FILESZ E_PHNUM * sizeof(Elf64_Phdr)
#define E_SHNUM 5
#define SH_SIZE E_SHNUM * sizeof(Elf64_Shdr)

#define P_FILESZ_TEXT 96
#define P_FILESZ_DATA 8
#define SH_SIZE_SHSTRTAB 31
#define P_FILESZ_RODATA E_PHOFF + P_FILESZ + P_FILESZ_TEXT + SH_SIZE_SHSTRTAB + SH_SIZE

#define P_OFFSET_TEXT E_PHOFF + P_FILESZ
#define P_OFFSET_RODATA P_OFFSET_TEXT + P_FILESZ_TEXT
#define P_OFFSET_DATA P_OFFSET_RODATA + P_FILESZ_RODATA
#define SH_OFFSET_SHSTRTAB P_OFFSET_DATA + P_FILESZ_DATA
#define E_SHOFF SH_OFFSET_SHSTRTAB + SH_SIZE_SHSTRTAB

#define P_VADDR_TEXT 0x1000
#define P_VADDR_RODATA 0x2000
#define P_VADDR_DATA 0x3000

#define SH_NAME_SHSTRTAB 1
#define SH_NAME_TEXT 11
#define SH_NAME_RODATA 17
#define SH_NAME_DATA 25

#define SH_ADDR(T) P_OFFSET_##T + P_VADDR_##T

static const Elf64_Ehdr ehdr = {
	.e_ident = {
		ELFMAG0,
		ELFMAG1,
		ELFMAG2,
		ELFMAG3,
		ELFCLASS64,
		ELFDATA2LSB,
		EV_CURRENT,
		ELFOSABI_SYSV,
	},
	.e_type = ET_DYN,
	.e_machine = EM_X86_64,
	.e_version = EV_CURRENT,
	.e_entry = SH_ADDR(TEXT),
	.e_phoff = E_PHOFF,
	.e_shoff = E_SHOFF,
	.e_flags = 0,
	.e_ehsize = sizeof(Elf64_Ehdr),
	.e_phentsize = sizeof(Elf64_Phdr),
	.e_phnum = E_PHNUM,
	.e_shentsize = sizeof(Elf64_Shdr),
	.e_shnum = E_SHNUM,
	.e_shstrndx = E_SHNUM - 1,
};

static const Elf64_Phdr phdr[E_PHNUM] = {
	{
		.p_type = PT_PHDR,
		.p_flags = PF_R,
		.p_offset = E_PHOFF,
		.p_vaddr = E_PHOFF,
		.p_paddr = E_PHOFF,
		.p_filesz = P_FILESZ,
		.p_memsz = P_FILESZ,
		.p_align = 8,
	},
	{
		.p_type = PT_LOAD,
		.p_flags = PF_R | PF_X,
		.p_offset = P_OFFSET_TEXT,
		.p_vaddr = SH_ADDR(TEXT),
		.p_paddr = SH_ADDR(TEXT),
		.p_filesz = P_FILESZ_TEXT,
		.p_memsz = P_FILESZ_TEXT,
		.p_align = 1,
	},
	{
		.p_type = PT_LOAD,
		.p_flags = PF_R,
		.p_offset = P_OFFSET_RODATA,
		.p_vaddr = SH_ADDR(RODATA),
		.p_paddr = SH_ADDR(RODATA),
		.p_filesz = P_FILESZ_RODATA,
		.p_memsz = P_FILESZ_RODATA,
		.p_align = 1,
	},
	{
		.p_type = PT_LOAD,
		.p_flags = PF_R | PF_W,
		.p_offset = P_OFFSET_DATA,
		.p_vaddr = SH_ADDR(DATA),
		.p_paddr = SH_ADDR(DATA),
		.p_filesz = P_FILESZ_DATA,
		.p_memsz = P_FILESZ_DATA,
		.p_align = 1,
	},
};

static const Elf64_Shdr shdr[E_SHNUM] = {
	{
		.sh_name = 0,
		.sh_type = 0,
		.sh_flags = 0,
		.sh_addr = 0,
		.sh_offset = 0,
		.sh_size = 0,
		.sh_link = 0,
		.sh_info = 0,
		.sh_addralign = 0,
		.sh_entsize = 0,
	},
	{
		.sh_name = SH_NAME_TEXT,
		.sh_type = SHT_PROGBITS,
		.sh_flags = SHF_ALLOC | SHF_EXECINSTR,
		.sh_addr = SH_ADDR(TEXT),
		.sh_offset = P_OFFSET_TEXT,
		.sh_size = P_FILESZ_TEXT,
		.sh_link = 0,
		.sh_info = 0,
		.sh_addralign = 1,
		.sh_entsize = 0,
	},
	{
		.sh_name = SH_NAME_RODATA,
		.sh_type = SHT_PROGBITS,
		.sh_flags = SHF_ALLOC,
		.sh_addr = SH_ADDR(RODATA),
		.sh_offset = P_OFFSET_RODATA,
		.sh_size = P_FILESZ_RODATA,
		.sh_link = 0,
		.sh_info = 0,
		.sh_addralign = 1,
		.sh_entsize = 0,
	},
	{
		.sh_name = SH_NAME_DATA,
		.sh_type = SHT_PROGBITS,
		.sh_flags = SHF_ALLOC | SHF_WRITE,
		.sh_addr = SH_ADDR(DATA),
		.sh_offset = P_OFFSET_DATA,
		.sh_size = P_FILESZ_DATA,
		.sh_link = 0,
		.sh_info = 0,
		.sh_addralign = 1,
		.sh_entsize = 0,
	},
	{
		.sh_name = SH_NAME_SHSTRTAB,
		.sh_type = SHT_STRTAB,
		.sh_flags = 0,
		.sh_addr = 0,
		.sh_offset = SH_OFFSET_SHSTRTAB,
		.sh_size = SH_SIZE_SHSTRTAB,
		.sh_link = 0,
		.sh_info = 0,
		.sh_addralign = 1,
		.sh_entsize = 0,
	},
};

static const uint8_t text[P_FILESZ_TEXT] = {
	72, 199, 194, 128, 1, 0, 0, 72, 141, 53, 82, 16, 0, 0, 72,
	49, 255, 72, 255, 199, 72, 137, 248, 80, 15, 5, 72, 199, 194,
	223, 2, 0, 0, 88, 80, 15, 5, 72, 199, 194, 8, 0, 0, 0, 72,
	141, 53, 12, 35, 0, 0, 72, 255, 6, 88, 80, 15, 5, 72, 199,
	194, 95, 1, 0, 0, 72, 141, 53, 152, 17, 0, 0, 88, 15, 5, 72,
	199, 192, 74, 0, 0, 0, 15, 5, 72, 49, 255, 72, 199, 192, 231,
	0, 0, 0, 15, 5,
};

static const uint8_t data[P_FILESZ_DATA] = {0, 0, 0, 0, 0, 0, 0, 0};
static const uint8_t shstrtab[SH_SIZE_SHSTRTAB] = "\0.shstrtab\0.text\0.rodata\0.data\0";

int main(void) {
	const int filedes = openat(AT_FDCWD, "stdout.o", O_CREAT | O_NOFOLLOW | O_TRUNC | O_WRONLY);

	if (filedes >= 0) {
		FILE *stream = fdopen(filedes, "w");

		if (stream) {
			fwrite(&ehdr, sizeof(Elf64_Ehdr), 1, stream);
			fwrite(phdr, sizeof(Elf64_Phdr), E_PHNUM, stream);
			fwrite(text, P_FILESZ_TEXT, 1, stream);
			fwrite(&ehdr, sizeof(Elf64_Ehdr), 1, stream);
			fwrite(phdr, sizeof(Elf64_Phdr), E_PHNUM, stream);
			fwrite(text, P_FILESZ_TEXT, 1, stream);
			fwrite(shstrtab, SH_SIZE_SHSTRTAB, 1, stream);
			fwrite(shdr, sizeof(Elf64_Shdr), E_SHNUM, stream);
			fwrite(data, P_FILESZ_DATA, 1, stream);
			fwrite(shstrtab, SH_SIZE_SHSTRTAB, 1, stream);
			fwrite(shdr, sizeof(Elf64_Shdr), E_SHNUM, stream);
			fsync(filedes);
			fchmod(filedes, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
			fclose(stream);
			exit(EXIT_SUCCESS);
		} else {
			close(filedes);
			exit(EXIT_FAILURE);
		}
	} else {
		exit(EXIT_FAILURE);
	}
}
