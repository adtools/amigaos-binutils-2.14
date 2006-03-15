/* Intel IA-32 specific support for 32-bit big endian ELF on Amithlon.
   Copyright 2002 Free Software Foundation, Inc.
   Written by Martin Blom.

This file is part of BFD, the Binary File Descriptor library.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#define TARGET_LITTLE_SYM		bfd_elf32_i386_amithlon_vec
#define TARGET_LITTLE_NAME		"elf32-i386-amithlon"
#define TARGET_BIG_SYM			bfd_elf32_i386be_amithlon_vec
#define TARGET_BIG_NAME			"elf32-i386be-amithlon"
#define ELF_ARCH			bfd_arch_i386
#define ELF_MACHINE_CODE		EM_386
#define ELF_MAXPAGESIZE			32  //0x1000

#include "bfd.h"
#include "sysdep.h"
#include "elf-bfd.h"

/* Forward declarations */
extern const bfd_target bfd_elf32_i386_amithlon_vec;
extern const bfd_target bfd_elf32_i386be_amithlon_vec;

static boolean elf_i386_relocate_section
  PARAMS ((bfd *, struct bfd_link_info *, bfd *, asection *, bfd_byte *,
	   Elf_Internal_Rela *, Elf_Internal_Sym *, asection **));

static boolean elf_i386_finish_dynamic_sections
  PARAMS ((bfd *, struct bfd_link_info *));

static boolean elf_i386_finish_dynamic_symbol
  PARAMS ((bfd *, struct bfd_link_info *, struct elf_link_hash_entry *,
	   Elf_Internal_Sym *));

boolean elf_link_output_relocs
  PARAMS ((bfd *, asection *, Elf_Internal_Shdr *, Elf_Internal_Rela *));

#define swap_xvec(xvec,c)                                   \
do {                                                        \
  xvec->bfd_getx64         = bfd_get ## c ## 64;            \
  xvec->bfd_getx_signed_64 = bfd_get ## c ## _signed_64;    \
  xvec->bfd_putx64         = bfd_put ## c ## 64;            \
  xvec->bfd_getx32         = bfd_get ## c ## 32;            \
  xvec->bfd_getx_signed_32 = bfd_get ## c ## _signed_32;    \
  xvec->bfd_putx32         = bfd_put ## c ## 32;            \
  xvec->bfd_getx16         = bfd_get ## c ## 16;            \
  xvec->bfd_getx_signed_16 = bfd_get ## c ## _signed_16;    \
  xvec->bfd_putx16         = bfd_put ## c ## 16;            \
} while(0)

/* Relocate a big endian i386 ELF section.  */

static boolean
amithlon_relocate_section (output_bfd, info, input_bfd, input_section,
			   contents, relocs, local_syms, local_sections)
     bfd *output_bfd;
     struct bfd_link_info *info;
     bfd *input_bfd;
     asection *input_section;
     bfd_byte *contents;
     Elf_Internal_Rela *relocs;
     Elf_Internal_Sym *local_syms;
     asection **local_sections;
{
  boolean switched_input  = false;
  boolean switched_output = false;
  boolean rc;

  /* Since code sections are actually little endian, no matter what
     endian mode we're operating in, this rather dirty hack is used to
     make sure the correct data access routines are used. */

  if ((bfd_get_section_flags (input_bfd, input_section) & SEC_CODE))
  {
    if (input_bfd->xvec == &bfd_elf32_i386be_amithlon_vec)
    {
      switched_input = true;

      swap_xvec(input_bfd->xvec,l);
    }
    
    if (output_bfd->xvec == &bfd_elf32_i386be_amithlon_vec)
    {
      switched_output = true;

      swap_xvec(output_bfd->xvec,l);
    }
  }

  rc = elf_i386_relocate_section (output_bfd, info, input_bfd, input_section,
				  contents, relocs,
				  local_syms, local_sections);
  
  if (switched_input)
  {
    swap_xvec(input_bfd->xvec,b);
  }

  if (switched_output)
  {
    swap_xvec(output_bfd->xvec,b);
  }
  
  return rc;
}  

static boolean
amithlon_finish_dynamic_sections (output_bfd, info)
     bfd *output_bfd;
     struct bfd_link_info *info;
{
  boolean switched_output = false;
  boolean rc;

  /* Since code sections are actually little endian, no matter what
     endian mode we're operating in, this rather dirty hack is used to
     make sure the correct data access routines are used. */

  if (output_bfd->xvec == &bfd_elf32_i386be_amithlon_vec)
  {
    switched_output = true;

    swap_xvec(output_bfd->xvec,l);
  }

  rc = elf_i386_finish_dynamic_sections (output_bfd, info);

  if (switched_output)
  {
    swap_xvec(output_bfd->xvec,b);
  }
  
  return rc;
}

static boolean
amithlon_finish_dynamic_symbol (output_bfd, info, h, sym)
     bfd *output_bfd;
     struct bfd_link_info *info;
     struct elf_link_hash_entry *h;
     Elf_Internal_Sym *sym;
{
  boolean switched_output = false;
  boolean rc;

  /* Since code sections are actually little endian, no matter what
     endian mode we're operating in, this rather dirty hack is used to
     make sure the correct data access routines are used. */

  if (output_bfd->xvec == &bfd_elf32_i386be_amithlon_vec)
  {
    switched_output = true;

    swap_xvec(output_bfd->xvec,l);
  }

  rc = elf_i386_finish_dynamic_symbol (output_bfd, info, h, sym);

  if (switched_output)
  {
    swap_xvec(output_bfd->xvec,b);
  }
  
  return rc;
}

static bfd_size_type
amithlon_additional_program_headers (abfd)
     bfd *abfd;
{
//  printf("big: %x little: %x\n",
//	 &bfd_elf32_i386be_amithlon_vec,
//	 &bfd_elf32_i386_amithlon_vec);
  // headers, text, rodata, data+bss
  return -2+4;
}

#define elf_backend_relocate_section	        amithlon_relocate_section
#define elf_backend_finish_dynamic_sections     amithlon_finish_dynamic_sections
#define elf_backend_finish_dynamic_symbol       amithlon_finish_dynamic_symbol
#define elf_backend_additional_program_headers  amithlon_additional_program_headers

#define ELF32_I386_RELOCATABLE_EXECUTABLES 1

#include "elf32-i386.c"
