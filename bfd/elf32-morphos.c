/* PowerPC-specific support for 32-bit ELF
   Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002
   Free Software Foundation, Inc.
   Written by Ian Lance Taylor, Cygnus Support.

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

/* This file is based on a preliminary PowerPC ELF ABI.  The
   information may not match the final PowerPC ELF ABI.  It includes
   suggestions from the in-progress Embedded PowerPC ABI, and that
   information may also not match.  */

#define ARCH_SIZE		32

#include "bfd.h"
#include "sysdep.h"
#include "bfdlink.h"
#include "genlink.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/ppc.h"

#define USE_RELA		/* we want RELA relocations, not REL */

/* Renaming structures, typedefs, macros and functions to be size-specific.  */
#define Elf_External_Ehdr	NAME(Elf,External_Ehdr)
#define Elf_External_Sym	NAME(Elf,External_Sym)
#define Elf_External_Shdr	NAME(Elf,External_Shdr)
#define Elf_External_Phdr	NAME(Elf,External_Phdr)
#define Elf_External_Rel	NAME(Elf,External_Rel)
#define Elf_External_Rela	NAME(Elf,External_Rela)
#define Elf_External_Dyn	NAME(Elf,External_Dyn)

#define elf_core_file_failing_command	NAME(bfd_elf,core_file_failing_command)
#define elf_core_file_failing_signal	NAME(bfd_elf,core_file_failing_signal)
#define elf_core_file_matches_executable_p \
  NAME(bfd_elf,core_file_matches_executable_p)
#define elf_object_p			NAME(bfd_elf,object_p)
#define elf_core_file_p			NAME(bfd_elf,core_file_p)
#define elf_get_symtab_upper_bound	NAME(bfd_elf,get_symtab_upper_bound)
#define elf_get_dynamic_symtab_upper_bound \
  NAME(bfd_elf,get_dynamic_symtab_upper_bound)
#define elf_swap_reloc_in		NAME(bfd_elf,swap_reloc_in)
#define elf_swap_reloca_in		NAME(bfd_elf,swap_reloca_in)
#define elf_swap_reloc_out		NAME(bfd_elf,swap_reloc_out)
#define elf_swap_reloca_out		NAME(bfd_elf,swap_reloca_out)
#define elf_swap_symbol_in		NAME(bfd_elf,swap_symbol_in)
#define elf_swap_symbol_out		NAME(bfd_elf,swap_symbol_out)
#define elf_swap_phdr_in		NAME(bfd_elf,swap_phdr_in)
#define elf_swap_phdr_out		NAME(bfd_elf,swap_phdr_out)
#define elf_swap_dyn_in			NAME(bfd_elf,swap_dyn_in)
#define elf_swap_dyn_out		NAME(bfd_elf,swap_dyn_out)
#define elf_get_reloc_upper_bound	NAME(bfd_elf,get_reloc_upper_bound)
#define elf_canonicalize_reloc		NAME(bfd_elf,canonicalize_reloc)
#define elf_slurp_symbol_table		NAME(bfd_elf,slurp_symbol_table)
#define elf_get_symtab			NAME(bfd_elf,get_symtab)
#define elf_canonicalize_dynamic_symtab \
  NAME(bfd_elf,canonicalize_dynamic_symtab)
#define elf_make_empty_symbol		NAME(bfd_elf,make_empty_symbol)
#define elf_get_symbol_info		NAME(bfd_elf,get_symbol_info)
#define elf_get_lineno			NAME(bfd_elf,get_lineno)
#define elf_set_arch_mach		NAME(bfd_elf,set_arch_mach)
#define elf_find_nearest_line		NAME(bfd_elf,find_nearest_line)
#define elf_sizeof_headers		NAME(bfd_elf,sizeof_headers)
#define elf_set_section_contents	NAME(bfd_elf,set_section_contents)
#define elf_no_info_to_howto		NAME(bfd_elf,no_info_to_howto)
#define elf_no_info_to_howto_rel	NAME(bfd_elf,no_info_to_howto_rel)
#define elf_find_section		NAME(bfd_elf,find_section)
#define elf_bfd_link_add_symbols	NAME(bfd_elf,bfd_link_add_symbols)
#define elf_add_dynamic_entry		NAME(bfd_elf,add_dynamic_entry)
#define elf_write_shdrs_and_ehdr	NAME(bfd_elf,write_shdrs_and_ehdr)
#define elf_write_out_phdrs		NAME(bfd_elf,write_out_phdrs)
#define elf_write_relocs		NAME(bfd_elf,write_relocs)
#define elf_slurp_reloc_table		NAME(bfd_elf,slurp_reloc_table)
#define elf_link_create_dynamic_sections \
  NAME(bfd_elf,link_create_dynamic_sections)
#define elf_bfd_discard_info		NAME(bfd_elf,discard_info)
#define elf_reloc_symbol_deleted_p	NAME(_bfd_elf,reloc_symbol_deleted_p)
#define elf_link_record_dynamic_symbol  _bfd_elf_link_record_dynamic_symbol
#define elf_bfd_final_link		NAME(bfd_elf,bfd_final_link)
#define elf_create_pointer_linker_section NAME(bfd_elf,create_pointer_linker_section)
#define elf_finish_pointer_linker_section NAME(bfd_elf,finish_pointer_linker_section)
#define elf_gc_sections			NAME(_bfd_elf,gc_sections)
#define elf_gc_common_finalize_got_offsets \
  NAME(_bfd_elf,gc_common_finalize_got_offsets)
#define elf_gc_common_final_link	NAME(_bfd_elf,gc_common_final_link)
#define elf_gc_record_vtinherit		NAME(_bfd_elf,gc_record_vtinherit)
#define elf_gc_record_vtentry		NAME(_bfd_elf,gc_record_vtentry)
#define elf_link_record_local_dynamic_symbol \
  NAME(_bfd_elf,link_record_local_dynamic_symbol)

#define ELF_R_INFO(X,Y)	ELF32_R_INFO(X,Y)
#define ELF_R_SYM(X)	ELF32_R_SYM(X)
#define ELF_R_TYPE(X)	ELF32_R_TYPE(X)
#define ELFCLASS	ELFCLASS32
#define FILE_ALIGN	4
#define LOG_FILE_ALIGN	2

#define H_PUT_WORD		H_PUT_32
#define H_PUT_SIGNED_WORD	H_PUT_S32
#define H_GET_WORD		H_GET_32
#define H_GET_SIGNED_WORD	H_GET_S32

#define elf_stringtab_init _bfd_elf_stringtab_init

#define section_from_elf_index bfd_section_from_elf_index

static int ddr_count;
static unsigned *ddr_ptr;

static reloc_howto_type *ppc_elf_reloc_type_lookup
  PARAMS ((bfd *abfd, bfd_reloc_code_real_type code));
static void ppc_elf_info_to_howto
  PARAMS ((bfd *abfd, arelent *cache_ptr, Elf32_Internal_Rela *dst));
static void ppc_elf_howto_init PARAMS ((void));
static int ppc_elf_sort_rela PARAMS ((const PTR, const PTR));
static boolean ppc_elf_relax_section
  PARAMS ((bfd *, asection *, struct bfd_link_info *, boolean *));
static bfd_reloc_status_type ppc_elf_addr16_ha_reloc
  PARAMS ((bfd *, arelent *, asymbol *, PTR, asection *, bfd *, char **));
static boolean ppc_elf_object_p PARAMS ((bfd *));
static boolean ppc_elf_set_private_flags PARAMS ((bfd *, flagword));
static boolean ppc_elf_merge_private_bfd_data PARAMS ((bfd *, bfd *));

static int ppc_elf_additional_program_headers PARAMS ((bfd *));
static boolean ppc_elf_modify_segment_map PARAMS ((bfd *));

static asection *ppc_elf_create_got
  PARAMS ((bfd *, struct bfd_link_info *));
static boolean ppc_elf_create_dynamic_sections
  PARAMS ((bfd *, struct bfd_link_info *));

static boolean ppc_elf_section_from_shdr PARAMS ((bfd *,
						  Elf32_Internal_Shdr *,
						  const char *));
static boolean ppc_elf_fake_sections
  PARAMS ((bfd *, Elf32_Internal_Shdr *, asection *));

static elf_linker_section_t *ppc_elf_create_linker_section
  PARAMS ((bfd *abfd,
	   struct bfd_link_info *info,
	   enum elf_linker_section_enum));

static boolean ppc_elf_check_relocs PARAMS ((bfd *,
					     struct bfd_link_info *,
					     asection *,
					     const Elf_Internal_Rela *));

static asection * ppc_elf_gc_mark_hook PARAMS ((asection *sec,
						struct bfd_link_info *info,
						Elf_Internal_Rela *rel,
						struct elf_link_hash_entry *h,
						Elf_Internal_Sym *sym));

static boolean ppc_elf_gc_sweep_hook PARAMS ((bfd *abfd,
					      struct bfd_link_info *info,
					      asection *sec,
					      const Elf_Internal_Rela *relocs));

static boolean ppc_elf_adjust_dynamic_symbol PARAMS ((struct bfd_link_info *,
						      struct elf_link_hash_entry *));

static boolean ppc_elf_size_dynamic_sections PARAMS ((bfd *, struct bfd_link_info *));

static boolean ppc_elf_relocate_section PARAMS ((bfd *,
						 struct bfd_link_info *info,
						 bfd *,
						 asection *,
						 bfd_byte *,
						 Elf_Internal_Rela *relocs,
						 Elf_Internal_Sym *local_syms,
						 asection **));

static boolean ppc_elf_add_symbol_hook  PARAMS ((bfd *,
						 struct bfd_link_info *,
						 const Elf_Internal_Sym *,
						 const char **,
						 flagword *,
						 asection **,
						 bfd_vma *));

static boolean ppc_elf_finish_dynamic_symbol PARAMS ((bfd *,
						      struct bfd_link_info *,
						      struct elf_link_hash_entry *,
						      Elf_Internal_Sym *));

static boolean ppc_elf_finish_dynamic_sections PARAMS ((bfd *, struct bfd_link_info *));
static enum elf_reloc_type_class ppc_elf_reloc_type_class
  PARAMS ((const Elf_Internal_Rela *));
static boolean ppc_elf_grok_prstatus
  PARAMS ((bfd *abfd, Elf_Internal_Note *note));
static boolean ppc_elf_grok_psinfo
  PARAMS ((bfd *abfd, Elf_Internal_Note *note));

#define BRANCH_PREDICT_BIT 0x200000		/* branch prediction bit for branch taken relocs */
#define RA_REGISTER_MASK 0x001f0000		/* mask to set RA in memory instructions */
#define RA_REGISTER_SHIFT 16			/* value to shift register by to insert RA */

/* The name of the dynamic interpreter.  This is put in the .interp
   section.  */

#define ELF_DYNAMIC_INTERPRETER "/usr/lib/ld.so.1"

/* The size in bytes of an entry in the procedure linkage table.  */
#define PLT_ENTRY_SIZE 12
/* The initial size of the plt reserved for the dynamic linker.  */
#define PLT_INITIAL_ENTRY_SIZE 72
/* The size of the gap between entries in the PLT.  */
#define PLT_SLOT_SIZE 8
/* The number of single-slot PLT entries (the rest use two slots).  */
#define PLT_NUM_SINGLE_ENTRIES 8192

/* Will references to this symbol always reference the symbol
   in this object?  */
#define SYMBOL_REFERENCES_LOCAL(INFO, H)				\
  ((! INFO->shared							\
    || INFO->symbolic							\
    || H->dynindx == -1							\
    || ELF_ST_VISIBILITY (H->other) == STV_INTERNAL			\
    || ELF_ST_VISIBILITY (H->other) == STV_HIDDEN)			\
   && (H->elf_link_hash_flags & ELF_LINK_HASH_DEF_REGULAR) != 0)

/* Will _calls_ to this symbol always call the version in this object?  */
#define SYMBOL_CALLS_LOCAL(INFO, H)				\
  ((! INFO->shared							\
    || INFO->symbolic							\
    || H->dynindx == -1							\
    || ELF_ST_VISIBILITY (H->other) != STV_DEFAULT)			\
   && (H->elf_link_hash_flags & ELF_LINK_HASH_DEF_REGULAR) != 0)

static reloc_howto_type *ppc_elf_howto_table[(int) R_PPC_max];

static reloc_howto_type ppc_elf_howto_raw[] = {
  /* This reloc does nothing.  */
  HOWTO (R_PPC_NONE,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_NONE",		/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 false),		/* pcrel_offset */

  /* A standard 32 bit relocation.  */
  HOWTO (R_PPC_ADDR32,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_ADDR32",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffffffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* An absolute 26 bit branch; the lower two bits must be zero.
     FIXME: we don't check that, we just clear them.  */
  HOWTO (R_PPC_ADDR24,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 26,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_ADDR24",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0x3fffffc,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* A standard 16 bit relocation.  */
  HOWTO (R_PPC_ADDR16,		/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_ADDR16",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* A 16 bit relocation without overflow.  */
  HOWTO (R_PPC_ADDR16_LO,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont,/* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_ADDR16_LO",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* The high order 16 bits of an address.  */
  HOWTO (R_PPC_ADDR16_HI,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_ADDR16_HI",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* The high order 16 bits of an address, plus 1 if the contents of
     the low 16 bits, treated as a signed number, is negative.  */
  HOWTO (R_PPC_ADDR16_HA,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 ppc_elf_addr16_ha_reloc, /* special_function */
	 "R_PPC_ADDR16_HA",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* An absolute 16 bit branch; the lower two bits must be zero.
     FIXME: we don't check that, we just clear them.  */
  HOWTO (R_PPC_ADDR14,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_ADDR14",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xfffc,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* An absolute 16 bit branch, for which bit 10 should be set to
     indicate that the branch is expected to be taken.	The lower two
     bits must be zero.  */
  HOWTO (R_PPC_ADDR14_BRTAKEN,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_ADDR14_BRTAKEN",/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xfffc,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* An absolute 16 bit branch, for which bit 10 should be set to
     indicate that the branch is not expected to be taken.  The lower
     two bits must be zero.  */
  HOWTO (R_PPC_ADDR14_BRNTAKEN, /* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_ADDR14_BRNTAKEN",/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xfffc,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* A relative 26 bit branch; the lower two bits must be zero.  */
  HOWTO (R_PPC_REL24,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 26,			/* bitsize */
	 true,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_REL24",		/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0x3fffffc,		/* dst_mask */
	 true),			/* pcrel_offset */

  /* A relative 16 bit branch; the lower two bits must be zero.  */
  HOWTO (R_PPC_REL14,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 true,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_REL14",		/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xfffc,		/* dst_mask */
	 true),			/* pcrel_offset */

  /* A relative 16 bit branch.  Bit 10 should be set to indicate that
     the branch is expected to be taken.  The lower two bits must be
     zero.  */
  HOWTO (R_PPC_REL14_BRTAKEN,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 true,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_REL14_BRTAKEN",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xfffc,		/* dst_mask */
	 true),			/* pcrel_offset */

  /* A relative 16 bit branch.  Bit 10 should be set to indicate that
     the branch is not expected to be taken.  The lower two bits must
     be zero.  */
  HOWTO (R_PPC_REL14_BRNTAKEN,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 true,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_REL14_BRNTAKEN",/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xfffc,		/* dst_mask */
	 true),			/* pcrel_offset */

  /* Like R_PPC_ADDR16, but referring to the GOT table entry for the
     symbol.  */
  HOWTO (R_PPC_GOT16,		/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_GOT16",		/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* Like R_PPC_ADDR16_LO, but referring to the GOT table entry for
     the symbol.  */
  HOWTO (R_PPC_GOT16_LO,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_GOT16_LO",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* Like R_PPC_ADDR16_HI, but referring to the GOT table entry for
     the symbol.  */
  HOWTO (R_PPC_GOT16_HI,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_GOT16_HI",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		 /* pcrel_offset */

  /* Like R_PPC_ADDR16_HA, but referring to the GOT table entry for
     the symbol.  */
  HOWTO (R_PPC_GOT16_HA,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 ppc_elf_addr16_ha_reloc, /* special_function */
	 "R_PPC_GOT16_HA",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* Like R_PPC_REL24, but referring to the procedure linkage table
     entry for the symbol.  */
  HOWTO (R_PPC_PLTREL24,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 26,			/* bitsize */
	 true,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed,  /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_PLTREL24",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0x3fffffc,		/* dst_mask */
	 true),			/* pcrel_offset */

  /* This is used only by the dynamic linker.  The symbol should exist
     both in the object being run and in some shared library.  The
     dynamic linker copies the data addressed by the symbol from the
     shared library into the object, because the object being
     run has to have the data at some particular address.  */
  HOWTO (R_PPC_COPY,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	 /* special_function */
	 "R_PPC_COPY",		/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 false),		/* pcrel_offset */

  /* Like R_PPC_ADDR32, but used when setting global offset table
     entries.  */
  HOWTO (R_PPC_GLOB_DAT,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	 /* special_function */
	 "R_PPC_GLOB_DAT",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffffffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* Marks a procedure linkage table entry for a symbol.  */
  HOWTO (R_PPC_JMP_SLOT,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	 /* special_function */
	 "R_PPC_JMP_SLOT",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 false),		/* pcrel_offset */

  /* Used only by the dynamic linker.  When the object is run, this
     longword is set to the load address of the object, plus the
     addend.  */
  HOWTO (R_PPC_RELATIVE,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	 /* special_function */
	 "R_PPC_RELATIVE",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffffffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* Like R_PPC_REL24, but uses the value of the symbol within the
     object rather than the final value.  Normally used for
     _GLOBAL_OFFSET_TABLE_.  */
  HOWTO (R_PPC_LOCAL24PC,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 26,			/* bitsize */
	 true,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_LOCAL24PC",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0x3fffffc,		/* dst_mask */
	 true),			/* pcrel_offset */

  /* Like R_PPC_ADDR32, but may be unaligned.  */
  HOWTO (R_PPC_UADDR32,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_UADDR32",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffffffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* Like R_PPC_ADDR16, but may be unaligned.  */
  HOWTO (R_PPC_UADDR16,		/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_UADDR16",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* 32-bit PC relative */
  HOWTO (R_PPC_REL32,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 true,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_REL32",		/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffffffff,		/* dst_mask */
	 true),			/* pcrel_offset */

  /* 32-bit relocation to the symbol's procedure linkage table.
     FIXME: not supported.  */
  HOWTO (R_PPC_PLT32,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_PLT32",		/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 false),		/* pcrel_offset */

  /* 32-bit PC relative relocation to the symbol's procedure linkage table.
     FIXME: not supported.  */
  HOWTO (R_PPC_PLTREL32,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 true,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_PLTREL32",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 true),			/* pcrel_offset */

  /* Like R_PPC_ADDR16_LO, but referring to the PLT table entry for
     the symbol.  */
  HOWTO (R_PPC_PLT16_LO,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_PLT16_LO",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* Like R_PPC_ADDR16_HI, but referring to the PLT table entry for
     the symbol.  */
  HOWTO (R_PPC_PLT16_HI,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_PLT16_HI",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		 /* pcrel_offset */

  /* Like R_PPC_ADDR16_HA, but referring to the PLT table entry for
     the symbol.  */
  HOWTO (R_PPC_PLT16_HA,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 ppc_elf_addr16_ha_reloc, /* special_function */
	 "R_PPC_PLT16_HA",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* A sign-extended 16 bit value relative to _SDA_BASE_, for use with
     small data items.  */
  HOWTO (R_PPC_SDAREL16,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_SDAREL16",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* 16-bit section relative relocation.  */
  HOWTO (R_PPC_SECTOFF,		/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_SECTOFF",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* 16-bit lower half section relative relocation.  */
  HOWTO (R_PPC_SECTOFF_LO,	  /* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_SECTOFF_LO",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* 16-bit upper half section relative relocation.  */
  HOWTO (R_PPC_SECTOFF_HI,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_SECTOFF_HI",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		 /* pcrel_offset */

  /* 16-bit upper half adjusted section relative relocation.  */
  HOWTO (R_PPC_SECTOFF_HA,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 ppc_elf_addr16_ha_reloc, /* special_function */
	 "R_PPC_SECTOFF_HA",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* The remaining relocs are from the Embedded ELF ABI, and are not
     in the SVR4 ELF ABI.  */

  /* 32 bit value resulting from the addend minus the symbol */
  HOWTO (R_PPC_EMB_NADDR32,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_EMB_NADDR32",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffffffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* 16 bit value resulting from the addend minus the symbol */
  HOWTO (R_PPC_EMB_NADDR16,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_EMB_NADDR16",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* 16 bit value resulting from the addend minus the symbol */
  HOWTO (R_PPC_EMB_NADDR16_LO,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont,/* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_EMB_ADDR16_LO",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* The high order 16 bits of the addend minus the symbol */
  HOWTO (R_PPC_EMB_NADDR16_HI,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_EMB_NADDR16_HI", /* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* The high order 16 bits of the result of the addend minus the address,
     plus 1 if the contents of the low 16 bits, treated as a signed number,
     is negative.  */
  HOWTO (R_PPC_EMB_NADDR16_HA,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 ppc_elf_addr16_ha_reloc, /* special_function */
	 "R_PPC_EMB_NADDR16_HA", /* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* 16 bit value resulting from allocating a 4 byte word to hold an
     address in the .sdata section, and returning the offset from
     _SDA_BASE_ for that relocation */
  HOWTO (R_PPC_EMB_SDAI16,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_EMB_SDAI16",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* 16 bit value resulting from allocating a 4 byte word to hold an
     address in the .sdata2 section, and returning the offset from
     _SDA2_BASE_ for that relocation */
  HOWTO (R_PPC_EMB_SDA2I16,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_EMB_SDA2I16",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* A sign-extended 16 bit value relative to _SDA2_BASE_, for use with
     small data items.	 */
  HOWTO (R_PPC_EMB_SDA2REL,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_EMB_SDA2REL",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* Relocate against either _SDA_BASE_ or _SDA2_BASE_, filling in the 16 bit
     signed offset from the appropriate base, and filling in the register
     field with the appropriate register (0, 2, or 13).  */
  HOWTO (R_PPC_EMB_SDA21,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_EMB_SDA21",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* Relocation not handled: R_PPC_EMB_MRKREF */
  /* Relocation not handled: R_PPC_EMB_RELSEC16 */
  /* Relocation not handled: R_PPC_EMB_RELST_LO */
  /* Relocation not handled: R_PPC_EMB_RELST_HI */
  /* Relocation not handled: R_PPC_EMB_RELST_HA */
  /* Relocation not handled: R_PPC_EMB_BIT_FLD */

  /* PC relative relocation against either _SDA_BASE_ or _SDA2_BASE_, filling
     in the 16 bit signed offset from the appropriate base, and filling in the
     register field with the appropriate register (0, 2, or 13).  */
  HOWTO (R_PPC_EMB_RELSDA,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 true,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_EMB_RELSDA",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* GNU extension to record C++ vtable hierarchy */
  HOWTO (R_PPC_GNU_VTINHERIT,	/* type */
	 0,			/* rightshift */
	 0,			/* size (0 = byte, 1 = short, 2 = long) */
	 0,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 NULL,			/* special_function */
	 "R_PPC_GNU_VTINHERIT",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 false),		/* pcrel_offset */

  /* GNU extension to record C++ vtable member usage */
  HOWTO (R_PPC_GNU_VTENTRY,	/* type */
	 0,			/* rightshift */
	 0,			/* size (0 = byte, 1 = short, 2 = long) */
	 0,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 NULL,			/* special_function */
	 "R_PPC_GNU_VTENTRY",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 false),		/* pcrel_offset */

  /* Phony reloc to handle AIX style TOC entries */
  HOWTO (R_PPC_TOC16,		/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_TOC16",		/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* 32-bit relocation relative to _SDA_BASE_ */
  HOWTO (R_PPC_MORPHOS_DREL,	/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_MORPHOS_DREL",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 false),		/* pcrel_offset */

  /* Lower 16 bits of a relocation relative to _SDA_BASE */
  HOWTO (R_PPC_MORPHOS_DREL_LO,	/* type */
	 0,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont,/* complain_on_overflow */
	 /*complain_overflow_bitfield,*/ /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_MORPHOS_DREL_LO",/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */

  /* Upper 16 bits of a relocation relative to _SDA_BASE */
  HOWTO (R_PPC_MORPHOS_DREL_HI,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont,/* complain_on_overflow */
	 /*complain_overflow_bitfield,*/ /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_MORPHOS_DREL_HI",/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		 /* pcrel_offset */

  /* Upper 16 bits of a relocation relative to _SDA_BASE */
  HOWTO (R_PPC_MORPHOS_DREL_HA,	/* type */
	 16,			/* rightshift */
	 1,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont,/* complain_on_overflow */
	 /*complain_overflow_bitfield,*/ /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_PPC_MORPHOS_DREL_HA",/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0xffff,		/* dst_mask */
	 false),		/* pcrel_offset */
};

/* Initialize the ppc_elf_howto_table, so that linear accesses can be done.  */

static void
ppc_elf_howto_init ()
{
  unsigned int i, type;

  for (i = 0; i < sizeof (ppc_elf_howto_raw) / sizeof (ppc_elf_howto_raw[0]); i++)
    {
      type = ppc_elf_howto_raw[i].type;
      BFD_ASSERT (type < sizeof (ppc_elf_howto_table) / sizeof (ppc_elf_howto_table[0]));
      ppc_elf_howto_table[type] = &ppc_elf_howto_raw[i];
    }
}

/* This function handles relaxing for the PPC with option --mpc860c0[=<n>].

   The MPC860, revision C0 or earlier contains a bug in the die.
   If all of the following conditions are true, the next instruction
   to be executed *may* be treated as a no-op.
   1/ A forward branch is executed.
   2/ The branch is predicted as not taken.
   3/ The branch is taken.
   4/ The branch is located in the last 5 words of a page.
      (The EOP limit is 5 by default but may be specified as any value from 1-10.)

   Our software solution is to detect these problematic branches in a
   linker pass and modify them as follows:
   1/ Unconditional branches - Since these are always predicted taken,
      there is no problem and no action is required.
   2/ Conditional backward branches - No problem, no action required.
   3/ Conditional forward branches - Ensure that the "inverse prediction
      bit" is set (ensure it is predicted taken).
   4/ Conditional register branches - Ensure that the "y bit" is set
      (ensure it is predicted taken).
*/

/* Sort sections by address.  */

static int
ppc_elf_sort_rela (arg1, arg2)
     const PTR arg1;
     const PTR arg2;
{
  const Elf_Internal_Rela **rela1 = (const Elf_Internal_Rela**) arg1;
  const Elf_Internal_Rela **rela2 = (const Elf_Internal_Rela**) arg2;

  /* Sort by offset.  */
  return ((*rela1)->r_offset - (*rela2)->r_offset);
}

static boolean
ppc_elf_relax_section (abfd, isec, link_info, again)
     bfd *abfd;
     asection *isec;
     struct bfd_link_info *link_info;
     boolean *again;
{
#define PAGESIZE 0x1000

  bfd_byte *contents = NULL;
  bfd_byte *free_contents = NULL;
  Elf_Internal_Rela *internal_relocs = NULL;
  Elf_Internal_Rela *free_relocs = NULL;
  Elf_Internal_Rela **rela_comb = NULL;
  int comb_curr, comb_count;

  /* We never have to do this more than once per input section.  */
  *again = false;

  /* If needed, initialize this section's cooked size.  */
  if (isec->_cooked_size == 0)
      isec->_cooked_size = isec->_raw_size;

  /* We're only interested in text sections which overlap the
     troublesome area at the end of a page.  */
  if (link_info->mpc860c0 && (isec->flags & SEC_CODE) && isec->_cooked_size)
    {
      bfd_vma dot, end_page, end_section;
      boolean section_modified;

      /* Get the section contents.  */
      /* Get cached copy if it exists.  */
      if (elf_section_data (isec)->this_hdr.contents != NULL)
	  contents = elf_section_data (isec)->this_hdr.contents;
      else
	{
	  /* Go get them off disk.  */
	  contents = (bfd_byte *) bfd_malloc (isec->_raw_size);
	  if (contents == NULL)
	    goto error_return;
	  free_contents = contents;

	  if (! bfd_get_section_contents (abfd, isec, contents,
					  (file_ptr) 0, isec->_raw_size))
	    goto error_return;
	}

      comb_curr = 0;
      comb_count = 0;
      if (isec->reloc_count)
	{
          unsigned n;
	  bfd_size_type amt;

          /* Get a copy of the native relocations.  */
          internal_relocs = _bfd_elf32_link_read_relocs (
    	    abfd, isec, (PTR) NULL, (Elf_Internal_Rela *) NULL,
    	    link_info->keep_memory);
          if (internal_relocs == NULL)
    	      goto error_return;
          if (! link_info->keep_memory)
    	      free_relocs = internal_relocs;

          /* Setup a faster access method for the reloc info we need.  */
	  amt = isec->reloc_count;
	  amt *= sizeof (Elf_Internal_Rela*);
          rela_comb = (Elf_Internal_Rela**) bfd_malloc (amt);
          if (rela_comb == NULL)
              goto error_return;
          for (n = 0; n < isec->reloc_count; ++n)
            {
              long r_type;

              r_type = ELF32_R_TYPE (internal_relocs[n].r_info);
              if (r_type < 0 || r_type >= (int) R_PPC_max)
                  goto error_return;

              /* Prologue constants are sometimes present in the ".text"
              sections and they can be identified by their associated relocation.
              We don't want to process those words and some others which
              can also be identified by their relocations.  However, not all
              conditional branches will have a relocation so we will
              only ignore words that 1) have a reloc, and 2) the reloc
              is not applicable to a conditional branch.
              The array rela_comb is built here for use in the EOP scan loop.  */
              switch (r_type)
                {
                case R_PPC_ADDR14_BRNTAKEN:     /* absolute, predicted not taken */
                case R_PPC_REL14:               /* relative cond. br.  */
                case R_PPC_REL14_BRNTAKEN:      /* rel. cond. br., predicted not taken */
                  /* We should check the instruction.  */
                  break;
                default:
                  /* The word is not a conditional branch - ignore it.  */
                  rela_comb[comb_count++] = &internal_relocs[n];
                  break;
                }
            }
          if (comb_count > 1)
	    qsort (rela_comb, (size_t) comb_count, sizeof (int), ppc_elf_sort_rela);
	}

      /* Enumerate each EOP region that overlaps this section.  */
      end_section = isec->vma + isec->_cooked_size;
      dot = end_page = (isec->vma | (PAGESIZE - 1)) + 1;
      dot -= link_info->mpc860c0;
      section_modified = false;
      if (dot < isec->vma)      /* Increment the start position if this section */
          dot = isec->vma;      /* begins in the middle of its first EOP region.  */
      for (;
           dot < end_section;
           dot += PAGESIZE, end_page += PAGESIZE)
        {

          /* Check each word in this EOP region.  */
          for (; dot < end_page; dot += 4)
            {
              bfd_vma isec_offset;
              unsigned long insn;
              boolean skip, modified;

              /* Don't process this word if there is a relocation for it and
              the relocation indicates the word is not a conditional branch.  */
              skip = false;
              isec_offset = dot - isec->vma;
              for (; comb_curr<comb_count; ++comb_curr)
                {
                  bfd_vma r_offset;

                  r_offset = rela_comb[comb_curr]->r_offset;
                  if (r_offset >= isec_offset)
                    {
                      if (r_offset == isec_offset) skip = true;
                      break;
                    }
                }
              if (skip) continue;

              /* Check the current word for a problematic conditional branch.  */
#define BO0(insn) ((insn) & 0x02000000)
#define BO2(insn) ((insn) & 0x00800000)
#define BO4(insn) ((insn) & 0x00200000)
              insn = (unsigned long) bfd_get_32 (abfd, contents + isec_offset);
              modified = false;
              if ((insn & 0xFc000000) == 0x40000000)
                {
                  /* Instruction is BCx */
                  if ((!BO0(insn) || !BO2(insn)) && !BO4(insn))
                    {
                      bfd_vma target;
                      /* This branch is predicted as "normal".
                      If this is a forward branch, it is problematic.  */

                      target = insn & 0x0000Fffc;               /*extract*/
                      target = (target ^ 0x8000) - 0x8000;      /*sign extend*/
                      if ((insn & 0x00000002) == 0)
                          target += dot;                        /*convert to abs*/
                      if (target > dot)
                        {
                          insn |= 0x00200000;   /* set the prediction bit */
                          modified = true;
                        }
                    }
                }
              else if ((insn & 0xFc00Fffe) == 0x4c000420)
                {
                  /* Instruction is BCCTRx */
                  if ((!BO0(insn) || !BO2(insn)) && !BO4(insn))
		    {
		      /* This branch is predicted as not-taken.
		      If this is a forward branch, it is problematic.
                      Since we can't tell statically if it will branch forward,
                      always set the prediction bit.  */
                      insn |= 0x00200000;   /* set the prediction bit */
                      modified = true;
		    }
                }
              else if ((insn & 0xFc00Fffe) == 0x4c000020)
                {
                  /* Instruction is BCLRx */
                  if ((!BO0(insn) || !BO2(insn)) && !BO4(insn))
		    {
		      /* This branch is predicted as not-taken.
		      If this is a forward branch, it is problematic.
                      Since we can't tell statically if it will branch forward,
                      always set the prediction bit.  */
                      insn |= 0x00200000;   /* set the prediction bit */
                      modified = true;
		    }
                }
#undef BO0
#undef BO2
#undef BO4
              if (modified)
	        {
                  bfd_put_32 (abfd, (bfd_vma) insn, contents + isec_offset);
		  section_modified = true;
	        }
            }
        }
      if (section_modified)
	{
	  elf_section_data (isec)->this_hdr.contents = contents;
	  free_contents = NULL;
	}
    }

  if (rela_comb != NULL)
    {
      free (rela_comb);
      rela_comb = NULL;
    }

  if (free_relocs != NULL)
    {
      free (free_relocs);
      free_relocs = NULL;
    }

  if (free_contents != NULL)
    {
      if (! link_info->keep_memory)
	free (free_contents);
      else
	{
	  /* Cache the section contents for elf_link_input_bfd.  */
	  elf_section_data (isec)->this_hdr.contents = contents;
	}
      free_contents = NULL;
    }

  return true;

error_return:
  if (rela_comb != NULL)
    free (rela_comb);
  if (free_relocs != NULL)
    free (free_relocs);
  if (free_contents != NULL)
    free (free_contents);
  return false;
}

static reloc_howto_type *
ppc_elf_reloc_type_lookup (abfd, code)
     bfd *abfd ATTRIBUTE_UNUSED;
     bfd_reloc_code_real_type code;
{
  enum elf_ppc_reloc_type ppc_reloc = R_PPC_NONE;

  if (!ppc_elf_howto_table[R_PPC_ADDR32])
    /* Initialize howto table if needed.  */
    ppc_elf_howto_init ();

  switch ((int) code)
    {
    default:
      return (reloc_howto_type *) NULL;

    case BFD_RELOC_NONE:		ppc_reloc = R_PPC_NONE;			break;
    case BFD_RELOC_32:			ppc_reloc = R_PPC_ADDR32;		break;
    case BFD_RELOC_PPC_BA26:		ppc_reloc = R_PPC_ADDR24;		break;
    case BFD_RELOC_16:			ppc_reloc = R_PPC_ADDR16;		break;
    case BFD_RELOC_LO16:		ppc_reloc = R_PPC_ADDR16_LO;		break;
    case BFD_RELOC_HI16:		ppc_reloc = R_PPC_ADDR16_HI;		break;
    case BFD_RELOC_HI16_S:		ppc_reloc = R_PPC_ADDR16_HA;		break;
    case BFD_RELOC_PPC_BA16:		ppc_reloc = R_PPC_ADDR14;		break;
    case BFD_RELOC_PPC_BA16_BRTAKEN:	ppc_reloc = R_PPC_ADDR14_BRTAKEN;	break;
    case BFD_RELOC_PPC_BA16_BRNTAKEN:	ppc_reloc = R_PPC_ADDR14_BRNTAKEN;	break;
    case BFD_RELOC_PPC_B26:		ppc_reloc = R_PPC_REL24;		break;
    case BFD_RELOC_PPC_B16:		ppc_reloc = R_PPC_REL14;		break;
    case BFD_RELOC_PPC_B16_BRTAKEN:	ppc_reloc = R_PPC_REL14_BRTAKEN;	break;
    case BFD_RELOC_PPC_B16_BRNTAKEN:	ppc_reloc = R_PPC_REL14_BRNTAKEN;	break;
    case BFD_RELOC_16_GOTOFF:		ppc_reloc = R_PPC_GOT16;		break;
    case BFD_RELOC_LO16_GOTOFF:		ppc_reloc = R_PPC_GOT16_LO;		break;
    case BFD_RELOC_HI16_GOTOFF:		ppc_reloc = R_PPC_GOT16_HI;		break;
    case BFD_RELOC_HI16_S_GOTOFF:	ppc_reloc = R_PPC_GOT16_HA;		break;
    case BFD_RELOC_24_PLT_PCREL:	ppc_reloc = R_PPC_PLTREL24;		break;
    case BFD_RELOC_PPC_COPY:		ppc_reloc = R_PPC_COPY;			break;
    case BFD_RELOC_PPC_GLOB_DAT:	ppc_reloc = R_PPC_GLOB_DAT;		break;
    case BFD_RELOC_PPC_LOCAL24PC:	ppc_reloc = R_PPC_LOCAL24PC;		break;
    case BFD_RELOC_32_PCREL:		ppc_reloc = R_PPC_REL32;		break;
    case BFD_RELOC_32_PLTOFF:		ppc_reloc = R_PPC_PLT32;		break;
    case BFD_RELOC_32_PLT_PCREL:	ppc_reloc = R_PPC_PLTREL32;		break;
    case BFD_RELOC_LO16_PLTOFF:		ppc_reloc = R_PPC_PLT16_LO;		break;
    case BFD_RELOC_HI16_PLTOFF:		ppc_reloc = R_PPC_PLT16_HI;		break;
    case BFD_RELOC_HI16_S_PLTOFF:	ppc_reloc = R_PPC_PLT16_HA;		break;
    case BFD_RELOC_GPREL16:		ppc_reloc = R_PPC_SDAREL16;		break;
    case BFD_RELOC_16_BASEREL:		ppc_reloc = R_PPC_SECTOFF;		break;
    case BFD_RELOC_LO16_BASEREL:	ppc_reloc = R_PPC_SECTOFF_LO;		break;
    case BFD_RELOC_HI16_BASEREL:	ppc_reloc = R_PPC_SECTOFF_HI;		break;
    case BFD_RELOC_HI16_S_BASEREL:	ppc_reloc = R_PPC_SECTOFF_HA;		break;
    case BFD_RELOC_CTOR:		ppc_reloc = R_PPC_ADDR32;		break;
    case BFD_RELOC_PPC_TOC16:		ppc_reloc = R_PPC_TOC16;		break;
    case BFD_RELOC_PPC_EMB_NADDR32:	ppc_reloc = R_PPC_EMB_NADDR32;		break;
    case BFD_RELOC_PPC_EMB_NADDR16:	ppc_reloc = R_PPC_EMB_NADDR16;		break;
    case BFD_RELOC_PPC_EMB_NADDR16_LO:	ppc_reloc = R_PPC_EMB_NADDR16_LO;	break;
    case BFD_RELOC_PPC_EMB_NADDR16_HI:	ppc_reloc = R_PPC_EMB_NADDR16_HI;	break;
    case BFD_RELOC_PPC_EMB_NADDR16_HA:	ppc_reloc = R_PPC_EMB_NADDR16_HA;	break;
    case BFD_RELOC_PPC_EMB_SDAI16:	ppc_reloc = R_PPC_EMB_SDAI16;		break;
    case BFD_RELOC_PPC_EMB_SDA2I16:	ppc_reloc = R_PPC_EMB_SDA2I16;		break;
    case BFD_RELOC_PPC_EMB_SDA2REL:	ppc_reloc = R_PPC_EMB_SDA2REL;		break;
    case BFD_RELOC_PPC_EMB_SDA21:	ppc_reloc = R_PPC_EMB_SDA21;		break;
    case BFD_RELOC_PPC_EMB_MRKREF:	ppc_reloc = R_PPC_EMB_MRKREF;		break;
    case BFD_RELOC_PPC_EMB_RELSEC16:	ppc_reloc = R_PPC_EMB_RELSEC16;		break;
    case BFD_RELOC_PPC_EMB_RELST_LO:	ppc_reloc = R_PPC_EMB_RELST_LO;		break;
    case BFD_RELOC_PPC_EMB_RELST_HI:	ppc_reloc = R_PPC_EMB_RELST_HI;		break;
    case BFD_RELOC_PPC_EMB_RELST_HA:	ppc_reloc = R_PPC_EMB_RELST_HA;		break;
    case BFD_RELOC_PPC_EMB_BIT_FLD:	ppc_reloc = R_PPC_EMB_BIT_FLD;		break;
    case BFD_RELOC_PPC_EMB_RELSDA:	ppc_reloc = R_PPC_EMB_RELSDA;		break;
    case BFD_RELOC_PPC_MORPHOS_DREL:	ppc_reloc = R_PPC_MORPHOS_DREL;		break;
    case BFD_RELOC_PPC_MORPHOS_DREL_LO:	ppc_reloc = R_PPC_MORPHOS_DREL_LO;	break;
    case BFD_RELOC_PPC_MORPHOS_DREL_HI:	ppc_reloc = R_PPC_MORPHOS_DREL_HI;	break;
    case BFD_RELOC_PPC_MORPHOS_DREL_HA:	ppc_reloc = R_PPC_MORPHOS_DREL_HA;	break;
    case BFD_RELOC_VTABLE_INHERIT:	ppc_reloc = R_PPC_GNU_VTINHERIT;	break;
    case BFD_RELOC_VTABLE_ENTRY:	ppc_reloc = R_PPC_GNU_VTENTRY;		break;
    }

  return ppc_elf_howto_table[(int) ppc_reloc];
};

/* Set the howto pointer for a PowerPC ELF reloc.  */

static void
ppc_elf_info_to_howto (abfd, cache_ptr, dst)
     bfd *abfd ATTRIBUTE_UNUSED;
     arelent *cache_ptr;
     Elf32_Internal_Rela *dst;
{
  if (!ppc_elf_howto_table[R_PPC_ADDR32])
    /* Initialize howto table if needed.  */
    ppc_elf_howto_init ();

  BFD_ASSERT (ELF32_R_TYPE (dst->r_info) < (unsigned int) R_PPC_max);
  cache_ptr->howto = ppc_elf_howto_table[ELF32_R_TYPE (dst->r_info)];
}

/* Handle the R_PPC_ADDR16_HA reloc.  */

static bfd_reloc_status_type
ppc_elf_addr16_ha_reloc (abfd, reloc_entry, symbol, data, input_section,
			 output_bfd, error_message)
     bfd *abfd ATTRIBUTE_UNUSED;
     arelent *reloc_entry;
     asymbol *symbol;
     PTR data ATTRIBUTE_UNUSED;
     asection *input_section;
     bfd *output_bfd;
     char **error_message ATTRIBUTE_UNUSED;
{
  /*bfd_vma relocation;*/

  if (output_bfd != NULL)
    {
      reloc_entry->address += input_section->output_offset;
      return bfd_reloc_ok;
    }
  else
    {
      reloc_entry->address += input_section->output_offset;
      input_section->output_section->orelocation[input_section->output_section->reloc_count++]=reloc_entry;
      return bfd_reloc_ok;
    }

  /*if (reloc_entry->address > input_section->_cooked_size)
    return bfd_reloc_outofrange;

  if (bfd_is_com_section (symbol->section))
    relocation = 0;
  else
    relocation = symbol->value;

  relocation += symbol->section->output_section->vma;
  relocation += symbol->section->output_offset;
  relocation += reloc_entry->addend;

  reloc_entry->addend += (relocation & 0x8000) << 1;

  return bfd_reloc_continue;*/
}

/* Fix bad default arch selected for a 32 bit input bfd when the
   default is 64 bit.  */

static boolean
ppc_elf_object_p (abfd)
     bfd *abfd;
{
  if (abfd->arch_info->the_default && abfd->arch_info->bits_per_word == 64)
    {
      Elf_Internal_Ehdr *i_ehdr = elf_elfheader (abfd);

      if (i_ehdr->e_ident[EI_CLASS] == ELFCLASS32)
	{
	  /* Relies on arch after 64 bit default being 32 bit default.  */
	  abfd->arch_info = abfd->arch_info->next;
	  BFD_ASSERT (abfd->arch_info->bits_per_word == 32);
	}
    }
  return true;
}

/* Function to set whether a module needs the -mrelocatable bit set.  */

static boolean
ppc_elf_set_private_flags (abfd, flags)
     bfd *abfd;
     flagword flags;
{
  BFD_ASSERT (!elf_flags_init (abfd)
	      || elf_elfheader (abfd)->e_flags == flags);

  elf_elfheader (abfd)->e_flags = flags;
  elf_flags_init (abfd) = true;
  return true;
}

/* Merge backend specific data from an object file to the output
   object file when linking */
static boolean
ppc_elf_merge_private_bfd_data (ibfd, obfd)
     bfd *ibfd;
     bfd *obfd;
{
  flagword old_flags;
  flagword new_flags;
  boolean error;

  /* Check if we have the same endianess */
  if (! _bfd_generic_verify_endian_match (ibfd, obfd))
    return false;

  if (bfd_get_flavour (ibfd) != bfd_target_elf_flavour
      || bfd_get_flavour (obfd) != bfd_target_elf_flavour)
    return true;

  new_flags = elf_elfheader (ibfd)->e_flags;
  old_flags = elf_elfheader (obfd)->e_flags;
  if (!elf_flags_init (obfd))	/* First call, no flags set */
    {
      elf_flags_init (obfd) = true;
      elf_elfheader (obfd)->e_flags = new_flags;
    }

  else if (new_flags == old_flags)	/* Compatible flags are ok */
    ;

  else					/* Incompatible flags */
    {
      /* Warn about -mrelocatable mismatch.  Allow -mrelocatable-lib to be linked
         with either.  */
      error = false;
      if ((new_flags & EF_PPC_RELOCATABLE) != 0
	  && (old_flags & (EF_PPC_RELOCATABLE | EF_PPC_RELOCATABLE_LIB)) == 0)
	{
	  error = true;
	  (*_bfd_error_handler)
	    (_("%s: compiled with -mrelocatable and linked with modules compiled normally"),
	     bfd_archive_filename (ibfd));
	}
      else if ((new_flags & (EF_PPC_RELOCATABLE | EF_PPC_RELOCATABLE_LIB)) == 0
	       && (old_flags & EF_PPC_RELOCATABLE) != 0)
	{
	  error = true;
	  (*_bfd_error_handler)
	    (_("%s: compiled normally and linked with modules compiled with -mrelocatable"),
	     bfd_archive_filename (ibfd));
	}

      /* The output is -mrelocatable-lib iff both the input files are.  */
      if (! (new_flags & EF_PPC_RELOCATABLE_LIB))
	elf_elfheader (obfd)->e_flags &= ~EF_PPC_RELOCATABLE_LIB;

      /* The output is -mrelocatable iff it can't be -mrelocatable-lib,
         but each input file is either -mrelocatable or -mrelocatable-lib.  */
      if (! (elf_elfheader (obfd)->e_flags & EF_PPC_RELOCATABLE_LIB)
	  && (new_flags & (EF_PPC_RELOCATABLE_LIB | EF_PPC_RELOCATABLE))
	  && (old_flags & (EF_PPC_RELOCATABLE_LIB | EF_PPC_RELOCATABLE)))
	elf_elfheader (obfd)->e_flags |= EF_PPC_RELOCATABLE;

      /* Do not warn about eabi vs. V.4 mismatch, just or in the bit if any module uses it */
      elf_elfheader (obfd)->e_flags |= (new_flags & EF_PPC_EMB);

      new_flags &= ~ (EF_PPC_RELOCATABLE | EF_PPC_RELOCATABLE_LIB | EF_PPC_EMB);
      old_flags &= ~ (EF_PPC_RELOCATABLE | EF_PPC_RELOCATABLE_LIB | EF_PPC_EMB);

      /* Warn about any other mismatches */
      if (new_flags != old_flags)
	{
	  error = true;
	  (*_bfd_error_handler)
	    (_("%s: uses different e_flags (0x%lx) fields than previous modules (0x%lx)"),
	     bfd_archive_filename (ibfd), (long) new_flags, (long) old_flags);
	}

      if (error)
	{
	  bfd_set_error (bfd_error_bad_value);
	  return false;
	}
    }

  return true;
}

/* Handle a PowerPC specific section when reading an object file.  This
   is called when elfcode.h finds a section with an unknown type.  */

static boolean
ppc_elf_section_from_shdr (abfd, hdr, name)
     bfd *abfd;
     Elf32_Internal_Shdr *hdr;
     const char *name;
{
  asection *newsect;
  flagword flags;

  if (! _bfd_elf_make_section_from_shdr (abfd, hdr, name))
    return false;

  newsect = hdr->bfd_section;
  flags = bfd_get_section_flags (abfd, newsect);
  if (hdr->sh_flags & SHF_EXCLUDE)
    flags |= SEC_EXCLUDE;

  if (hdr->sh_type == SHT_ORDERED)
    flags |= SEC_SORT_ENTRIES;

  bfd_set_section_flags (abfd, newsect, flags);
  return true;
}

/* Set up any other section flags and such that may be necessary.  */

static boolean
ppc_elf_fake_sections (abfd, shdr, asect)
     bfd *abfd ATTRIBUTE_UNUSED;
     Elf32_Internal_Shdr *shdr;
     asection *asect;
{
  if ((asect->flags & SEC_EXCLUDE) != 0)
    shdr->sh_flags |= SHF_EXCLUDE;

  if ((asect->flags & SEC_SORT_ENTRIES) != 0)
    shdr->sh_type = SHT_ORDERED;

  return true;
}

/* Create a special linker section */
static elf_linker_section_t *
ppc_elf_create_linker_section (abfd, info, which)
     bfd *abfd;
     struct bfd_link_info *info;
     enum elf_linker_section_enum which;
{
  bfd *dynobj = elf_hash_table (info)->dynobj;
  elf_linker_section_t *lsect;

  /* Record the first bfd section that needs the special section */
  if (!dynobj)
    dynobj = elf_hash_table (info)->dynobj = abfd;

  /* If this is the first time, create the section */
  lsect = elf_linker_section (dynobj, which);
  if (!lsect)
    {
      elf_linker_section_t defaults;
      static elf_linker_section_t zero_section;

      defaults = zero_section;
      defaults.which = which;
      defaults.hole_written_p = false;
      defaults.alignment = 2;

      /* Both of these sections are (technically) created by the user
	 putting data in them, so they shouldn't be marked
	 SEC_LINKER_CREATED.

	 The linker creates them so it has somewhere to attach their
	 respective symbols. In fact, if they were empty it would
	 be OK to leave the symbol set to 0 (or any random number), because
	 the appropriate register should never be used.  */
      defaults.flags = (SEC_ALLOC | SEC_LOAD | SEC_HAS_CONTENTS
			| SEC_IN_MEMORY);

      switch (which)
	{
	default:
	  (*_bfd_error_handler) (_("%s: Unknown special linker type %d"),
				 bfd_get_filename (abfd),
				 (int) which);

	  bfd_set_error (bfd_error_bad_value);
	  return (elf_linker_section_t *) 0;

	case LINKER_SECTION_SDATA:	/* .sdata/.sbss section */
	  defaults.name		  = ".sdata";
	  defaults.rel_name	  = ".rela.sdata";
	  defaults.bss_name	  = ".sbss";
	  defaults.sym_name	  = "_SDA_BASE_";
	  defaults.sym_offset	  = 32768;
	  break;

	case LINKER_SECTION_SDATA2:	/* .sdata2/.sbss2 section */
	  defaults.name		  = ".sdata2";
	  defaults.rel_name	  = ".rela.sdata2";
	  defaults.bss_name	  = ".sbss2";
	  defaults.sym_name	  = "_SDA2_BASE_";
	  defaults.sym_offset	  = 32768;
	  defaults.flags	 |= SEC_READONLY;
	  break;
	}

      lsect = _bfd_elf_create_linker_section (abfd, info, which, &defaults);
    }

  return lsect;
}

/* If we have a non-zero sized .sbss2 or .PPC.EMB.sbss0 sections, we
   need to bump up the number of section headers.  */

static int
ppc_elf_additional_program_headers (abfd)
     bfd *abfd;
{
  asection *s;
  int ret;

  ret = 0;

  s = bfd_get_section_by_name (abfd, ".interp");
  if (s != NULL)
    ++ret;

  s = bfd_get_section_by_name (abfd, ".sbss2");
  if (s != NULL && (s->flags & SEC_LOAD) != 0 && s->_raw_size > 0)
    ++ret;

  s = bfd_get_section_by_name (abfd, ".PPC.EMB.sbss0");
  if (s != NULL && (s->flags & SEC_LOAD) != 0 && s->_raw_size > 0)
    ++ret;

  return ret;
}

/* Modify the segment map if needed.  */

static boolean
ppc_elf_modify_segment_map (abfd)
     bfd *abfd ATTRIBUTE_UNUSED;
{
  return true;
}

/* The powerpc .got has a blrl instruction in it.  Mark it executable.  */

static asection *
ppc_elf_create_got (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info;
{
  register asection *s;
  flagword flags;

  if (!_bfd_elf_create_got_section (abfd, info))
    return NULL;

  s = bfd_get_section_by_name (abfd, ".got");
  if (s == NULL)
    abort ();

  flags = (SEC_ALLOC | SEC_LOAD | SEC_CODE | SEC_HAS_CONTENTS | SEC_IN_MEMORY
	   | SEC_LINKER_CREATED);
  if (!bfd_set_section_flags (abfd, s, flags))
    return NULL;
  return s;
}

/* We have to create .dynsbss and .rela.sbss here so that they get mapped
   to output sections (just like _bfd_elf_create_dynamic_sections has
   to create .dynbss and .rela.bss).  */

static boolean
ppc_elf_create_dynamic_sections (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info;
{
  register asection *s;
  flagword flags;

  if (!ppc_elf_create_got (abfd, info))
    return false;

  if (!_bfd_elf_create_dynamic_sections (abfd, info))
    return false;

  flags = (SEC_ALLOC | SEC_LOAD | SEC_HAS_CONTENTS | SEC_IN_MEMORY
	   | SEC_LINKER_CREATED);

  s = bfd_make_section (abfd, ".dynsbss");
  if (s == NULL
      || ! bfd_set_section_flags (abfd, s, SEC_ALLOC))
    return false;

  if (! info->shared)
    {
      s = bfd_make_section (abfd, ".rela.sbss");
      if (s == NULL
	  || ! bfd_set_section_flags (abfd, s, flags | SEC_READONLY)
	  || ! bfd_set_section_alignment (abfd, s, 2))
	return false;
    }

  s = bfd_get_section_by_name (abfd, ".plt");
  if (s == NULL)
    abort ();

  flags = SEC_ALLOC | SEC_CODE | SEC_IN_MEMORY | SEC_LINKER_CREATED;
  return bfd_set_section_flags (abfd, s, flags);
}

/* Adjust a symbol defined by a dynamic object and referenced by a
   regular object.  The current definition is in some section of the
   dynamic object, but we're not including those sections.  We have to
   change the definition to something the rest of the link can
   understand.  */

static boolean
ppc_elf_adjust_dynamic_symbol (info, h)
     struct bfd_link_info *info;
     struct elf_link_hash_entry *h;
{
  bfd *dynobj = elf_hash_table (info)->dynobj;
  asection *s;
  unsigned int power_of_two;
  bfd_vma plt_offset;

#ifdef DEBUG
  fprintf (stderr, "ppc_elf_adjust_dynamic_symbol called for %s\n", h->root.root.string);
#endif

  /* Make sure we know what is going on here.  */
  BFD_ASSERT (dynobj != NULL
	      && ((h->elf_link_hash_flags & ELF_LINK_HASH_NEEDS_PLT)
		  || h->weakdef != NULL
		  || ((h->elf_link_hash_flags
		       & ELF_LINK_HASH_DEF_DYNAMIC) != 0
		      && (h->elf_link_hash_flags
			  & ELF_LINK_HASH_REF_REGULAR) != 0
		      && (h->elf_link_hash_flags
			  & ELF_LINK_HASH_DEF_REGULAR) == 0)));

  /* If this is a function, put it in the procedure linkage table.  We
     will fill in the contents of the procedure linkage table later,
     when we know the address of the .got section.  */
  if (h->type == STT_FUNC
      || (h->elf_link_hash_flags & ELF_LINK_HASH_NEEDS_PLT) != 0)
    {
      if (! elf_hash_table (info)->dynamic_sections_created
 	  || SYMBOL_CALLS_LOCAL (info, h)
	  || (info->shared && h->plt.refcount <= 0))
	{
	  /* A PLT entry is not required/allowed when:

	     1. We are not using ld.so; because then the PLT entry
	     can't be set up, so we can't use one.

	     2. We know for certain that a call to this symbol
	     will go to this object.

	     3. GC has rendered the entry unused.
	     Note, however, that in an executable all references to the
	     symbol go to the PLT, so we can't turn it off in that case.
	     ??? The correct thing to do here is to reference count
	     all uses of the symbol, not just those to the GOT or PLT.  */
	  h->plt.offset = (bfd_vma) -1;
	  h->elf_link_hash_flags &= ~ELF_LINK_HASH_NEEDS_PLT;
	  return true;
	}

      /* Make sure this symbol is output as a dynamic symbol.  */
      if (h->dynindx == -1)
	{
	  if (! bfd_elf32_link_record_dynamic_symbol (info, h))
	    return false;
	}
      BFD_ASSERT (h->dynindx != -1);

      s = bfd_get_section_by_name (dynobj, ".plt");
      BFD_ASSERT (s != NULL);

      /* If this is the first .plt entry, make room for the special
	 first entry.  */
      if (s->_raw_size == 0)
	s->_raw_size += PLT_INITIAL_ENTRY_SIZE;

      /* The PowerPC PLT is actually composed of two parts, the first part
	 is 2 words (for a load and a jump), and then there is a remaining
	 word available at the end.  */
      plt_offset = (PLT_INITIAL_ENTRY_SIZE
		    + (PLT_SLOT_SIZE
		       * ((s->_raw_size - PLT_INITIAL_ENTRY_SIZE)
			  / PLT_ENTRY_SIZE)));

      /* If this symbol is not defined in a regular file, and we are
	 not generating a shared library, then set the symbol to this
	 location in the .plt.  This is required to make function
	 pointers compare as equal between the normal executable and
	 the shared library.  */
      if (! info->shared
	  && (h->elf_link_hash_flags & ELF_LINK_HASH_DEF_REGULAR) == 0)
	{
	  h->root.u.def.section = s;
	  h->root.u.def.value = plt_offset;
	}

      h->plt.offset = plt_offset;

      /* Make room for this entry.  After the 8192nd entry, room
         for two entries is allocated.  */
      if ((s->_raw_size - PLT_INITIAL_ENTRY_SIZE) / PLT_ENTRY_SIZE
	  >= PLT_NUM_SINGLE_ENTRIES)
	s->_raw_size += 2 * PLT_ENTRY_SIZE;
      else
	s->_raw_size += PLT_ENTRY_SIZE;

      /* We also need to make an entry in the .rela.plt section.  */
      s = bfd_get_section_by_name (dynobj, ".rela.plt");
      BFD_ASSERT (s != NULL);
      s->_raw_size += sizeof (Elf32_External_Rela);

      return true;
    }
  else
    h->plt.offset = (bfd_vma) -1;

  /* If this is a weak symbol, and there is a real definition, the
     processor independent code will have arranged for us to see the
     real definition first, and we can just use the same value.  */
  if (h->weakdef != NULL)
    {
      BFD_ASSERT (h->weakdef->root.type == bfd_link_hash_defined
		  || h->weakdef->root.type == bfd_link_hash_defweak);
      h->root.u.def.section = h->weakdef->root.u.def.section;
      h->root.u.def.value = h->weakdef->root.u.def.value;
      return true;
    }

  /* This is a reference to a symbol defined by a dynamic object which
     is not a function.  */

  /* If we are creating a shared library, we must presume that the
     only references to the symbol are via the global offset table.
     For such cases we need not do anything here; the relocations will
     be handled correctly by relocate_section.  */
  if (info->shared)
    return true;

  /* We must allocate the symbol in our .dynbss section, which will
     become part of the .bss section of the executable.  There will be
     an entry for this symbol in the .dynsym section.  The dynamic
     object will contain position independent code, so all references
     from the dynamic object to this symbol will go through the global
     offset table.  The dynamic linker will use the .dynsym entry to
     determine the address it must put in the global offset table, so
     both the dynamic object and the regular object will refer to the
     same memory location for the variable.

     Of course, if the symbol is sufficiently small, we must instead
     allocate it in .sbss.  FIXME: It would be better to do this if and
     only if there were actually SDAREL relocs for that symbol.  */

  if (h->size <= elf_gp_size (dynobj))
    s = bfd_get_section_by_name (dynobj, ".dynsbss");
  else
    s = bfd_get_section_by_name (dynobj, ".dynbss");
  BFD_ASSERT (s != NULL);

  /* We must generate a R_PPC_COPY reloc to tell the dynamic linker to
     copy the initial value out of the dynamic object and into the
     runtime process image.  We need to remember the offset into the
     .rela.bss section we are going to use.  */
  if ((h->root.u.def.section->flags & SEC_ALLOC) != 0)
    {
      asection *srel;

      if (h->size <= elf_gp_size (dynobj))
	srel = bfd_get_section_by_name (dynobj, ".rela.sbss");
      else
	srel = bfd_get_section_by_name (dynobj, ".rela.bss");
      BFD_ASSERT (srel != NULL);
      srel->_raw_size += sizeof (Elf32_External_Rela);
      h->elf_link_hash_flags |= ELF_LINK_HASH_NEEDS_COPY;
    }

  /* We need to figure out the alignment required for this symbol.  I
     have no idea how ELF linkers handle this.  */
  power_of_two = bfd_log2 (h->size);
  if (power_of_two > 4)
    power_of_two = 4;

  /* Apply the required alignment.  */
  s->_raw_size = BFD_ALIGN (s->_raw_size,
			    (bfd_size_type) (1 << power_of_two));
  if (power_of_two > bfd_get_section_alignment (dynobj, s))
    {
      if (! bfd_set_section_alignment (dynobj, s, power_of_two))
	return false;
    }

  /* Define the symbol as being at this point in the section.  */
  h->root.u.def.section = s;
  h->root.u.def.value = s->_raw_size;

  /* Increment the section size to make room for the symbol.  */
  s->_raw_size += h->size;

  return true;
}

/* Set the sizes of the dynamic sections.  */

static boolean
ppc_elf_size_dynamic_sections (output_bfd, info)
     bfd *output_bfd ATTRIBUTE_UNUSED;
     struct bfd_link_info *info;
{
  bfd *dynobj;
  asection *s;
  boolean plt;
  boolean relocs;

#ifdef DEBUG
  fprintf (stderr, "ppc_elf_size_dynamic_sections called\n");
#endif

  dynobj = elf_hash_table (info)->dynobj;
  BFD_ASSERT (dynobj != NULL);

  if (elf_hash_table (info)->dynamic_sections_created)
    {
      /* Set the contents of the .interp section to the interpreter.  */
      if (! info->shared)
	{
	  s = bfd_get_section_by_name (dynobj, ".interp");
	  BFD_ASSERT (s != NULL);
	  s->_raw_size = sizeof ELF_DYNAMIC_INTERPRETER;
	  s->contents = (unsigned char *) ELF_DYNAMIC_INTERPRETER;
	}
    }
  else
    {
      /* We may have created entries in the .rela.got, .rela.sdata, and
	 .rela.sdata2 sections.  However, if we are not creating the
	 dynamic sections, we will not actually use these entries.  Reset
	 the size of .rela.got, et al, which will cause it to get
	 stripped from the output file below.  */
      static char *rela_sections[] = { ".rela.got", ".rela.sdata",
				       ".rela.sdata2", ".rela.sbss",
				       (char *) 0 };
      char **p;

      for (p = rela_sections; *p != (char *) 0; p++)
	{
	  s = bfd_get_section_by_name (dynobj, *p);
	  if (s != NULL)
	    s->_raw_size = 0;
	}
    }

  /* The check_relocs and adjust_dynamic_symbol entry points have
     determined the sizes of the various dynamic sections.  Allocate
     memory for them.  */
  plt = false;
  relocs = false;
  for (s = dynobj->sections; s != NULL; s = s->next)
    {
      const char *name;
      boolean strip;

      if ((s->flags & SEC_LINKER_CREATED) == 0)
	continue;

      /* It's OK to base decisions on the section name, because none
	 of the dynobj section names depend upon the input files.  */
      name = bfd_get_section_name (dynobj, s);

      strip = false;

      if (strcmp (name, ".plt") == 0)
	{
	  if (s->_raw_size == 0)
	    {
	      /* Strip this section if we don't need it; see the
                 comment below.  */
	      strip = true;
	    }
	  else
	    {
	      /* Remember whether there is a PLT.  */
	      plt = true;
	    }
	}
      else if (strncmp (name, ".rela", 5) == 0)
	{
	  if (s->_raw_size == 0)
	    {
	      /* If we don't need this section, strip it from the
		 output file.  This is mostly to handle .rela.bss and
		 .rela.plt.  We must create both sections in
		 create_dynamic_sections, because they must be created
		 before the linker maps input sections to output
		 sections.  The linker does that before
		 adjust_dynamic_symbol is called, and it is that
		 function which decides whether anything needs to go
		 into these sections.  */
	      strip = true;
	    }
	  else
	    {
	      /* Remember whether there are any relocation sections.  */
	      relocs = true;

	      /* We use the reloc_count field as a counter if we need
		 to copy relocs into the output file.  */
	      s->reloc_count = 0;
	    }
	}
      else if (strcmp (name, ".got") != 0
	       && strcmp (name, ".sdata") != 0
	       && strcmp (name, ".sdata2") != 0)
	{
	  /* It's not one of our sections, so don't allocate space.  */
	  continue;
	}

      if (strip)
	{
	  _bfd_strip_section_from_output (info, s);
	  continue;
	}

      /* Allocate memory for the section contents.  */
      s->contents = (bfd_byte *) bfd_zalloc (dynobj, s->_raw_size);
      if (s->contents == NULL && s->_raw_size != 0)
	return false;
    }

  if (elf_hash_table (info)->dynamic_sections_created)
    {
      /* Add some entries to the .dynamic section.  We fill in the
	 values later, in ppc_elf_finish_dynamic_sections, but we
	 must add the entries now so that we get the correct size for
	 the .dynamic section.  The DT_DEBUG entry is filled in by the
	 dynamic linker and used by the debugger.  */
#define add_dynamic_entry(TAG, VAL) \
  bfd_elf32_add_dynamic_entry (info, (bfd_vma) (TAG), (bfd_vma) (VAL))

      if (!info->shared)
	{
	  if (!add_dynamic_entry (DT_DEBUG, 0))
	    return false;
	}

      if (plt)
	{
	  if (!add_dynamic_entry (DT_PLTGOT, 0)
	      || !add_dynamic_entry (DT_PLTRELSZ, 0)
	      || !add_dynamic_entry (DT_PLTREL, DT_RELA)
	      || !add_dynamic_entry (DT_JMPREL, 0))
	    return false;
	}

      if (relocs)
	{
	  if (!add_dynamic_entry (DT_RELA, 0)
	      || !add_dynamic_entry (DT_RELASZ, 0)
	      || !add_dynamic_entry (DT_RELAENT, sizeof (Elf32_External_Rela)))
	    return false;
	}

      if ((info->flags & DF_TEXTREL) != 0)
	{
	  if (!add_dynamic_entry (DT_TEXTREL, 0))
	    return false;
	  info->flags |= DF_TEXTREL;
	}
    }
#undef add_dynamic_entry

  return true;
}

/* Look through the relocs for a section during the first phase, and
   allocate space in the global offset table or procedure linkage
   table.  */

static boolean
ppc_elf_check_relocs (abfd, info, sec, relocs)
     bfd *abfd;
     struct bfd_link_info *info;
     asection *sec;
     const Elf_Internal_Rela *relocs;
{
  bfd *dynobj;
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes, **sym_hashes_end;
  const Elf_Internal_Rela *rel;
  const Elf_Internal_Rela *rel_end;
  bfd_signed_vma *local_got_refcounts;
  elf_linker_section_t *sdata;
  elf_linker_section_t *sdata2;
  asection *sreloc;
  asection *sgot = NULL;
  asection *srelgot = NULL;

  if (info->relocateable)
    return true;

#ifdef DEBUG
  fprintf (stderr, "ppc_elf_check_relocs called for section %s in %s\n",
	   bfd_get_section_name (abfd, sec),
	   bfd_archive_filename (abfd));
#endif

  /* Create the linker generated sections all the time so that the
     special symbols are created.  */

  if ((sdata = elf_linker_section (abfd, LINKER_SECTION_SDATA)) == NULL)
    {
      sdata = ppc_elf_create_linker_section (abfd, info, LINKER_SECTION_SDATA);
      if (!sdata)
	return false;
    }

  if ((sdata2 = elf_linker_section (abfd, LINKER_SECTION_SDATA2)) == NULL)
    {
      sdata2 = ppc_elf_create_linker_section (abfd, info, LINKER_SECTION_SDATA2);
      if (!sdata2)
	return false;
    }

  dynobj = elf_hash_table (info)->dynobj;
  symtab_hdr = &elf_tdata (abfd)->symtab_hdr;
  local_got_refcounts = elf_local_got_refcounts (abfd);

  sym_hashes = elf_sym_hashes (abfd);
  sym_hashes_end = sym_hashes + symtab_hdr->sh_size/sizeof (Elf32_External_Sym);
  if (!elf_bad_symtab (abfd))
    sym_hashes_end -= symtab_hdr->sh_info;

  sreloc = NULL;

  rel_end = relocs + sec->reloc_count;
  for (rel = relocs; rel < rel_end; rel++)
    {
      unsigned long r_symndx;
      struct elf_link_hash_entry *h;

      r_symndx = ELF32_R_SYM (rel->r_info);
      if (r_symndx < symtab_hdr->sh_info)
	h = NULL;
      else
	h = sym_hashes[r_symndx - symtab_hdr->sh_info];

      /* If a relocation refers to _GLOBAL_OFFSET_TABLE_, create the .got.
	 This shows up in particular in an R_PPC_ADDR32 in the eabi
	 startup code.  */
      if (h && strcmp (h->root.root.string, "_GLOBAL_OFFSET_TABLE_") == 0)
	{
	  if (sgot == NULL)
	    {
	      if (dynobj == NULL)
		elf_hash_table (info)->dynobj = dynobj = abfd;
	      sgot = ppc_elf_create_got (dynobj, info);
	      if (sgot == NULL)
		return false;
	    }
	}

      switch (ELF32_R_TYPE (rel->r_info))
	{
	/* GOT16 relocations */
	case R_PPC_GOT16:
	case R_PPC_GOT16_LO:
	case R_PPC_GOT16_HI:
	case R_PPC_GOT16_HA:
	  /* This symbol requires a global offset table entry.  */

	  if (sgot == NULL)
	    {
	      if (dynobj == NULL)
		elf_hash_table (info)->dynobj = dynobj = abfd;
	      sgot = ppc_elf_create_got (dynobj, info);
	      if (sgot == NULL)
		return false;
	    }

	  if (srelgot == NULL
	      && (h != NULL || info->shared))
	    {
	      srelgot = bfd_get_section_by_name (dynobj, ".rela.got");
	      if (srelgot == NULL)
		{
		  srelgot = bfd_make_section (dynobj, ".rela.got");
		  if (srelgot == NULL
		      || ! bfd_set_section_flags (dynobj, srelgot,
						  (SEC_ALLOC
						   | SEC_LOAD
						   | SEC_HAS_CONTENTS
						   | SEC_IN_MEMORY
						   | SEC_LINKER_CREATED
						   | SEC_READONLY))
		      || ! bfd_set_section_alignment (dynobj, srelgot, 2))
		    return false;
		}
	    }

	  if (h != NULL)
	    {
	      if (h->got.refcount == 0)
		{
		  /* Make sure this symbol is output as a dynamic symbol.  */
		  if (h->dynindx == -1)
		    if (!bfd_elf32_link_record_dynamic_symbol (info, h))
		      return false;

		  /* Allocate space in the .got.  */
		  sgot->_raw_size += 4;
		  /* Allocate relocation space.  */
		  srelgot->_raw_size += sizeof (Elf32_External_Rela);
		}
	      h->got.refcount++;
	    }
	  else
	    {
	      /* This is a global offset table entry for a local symbol.  */
	      if (local_got_refcounts == NULL)
		{
		  bfd_size_type size;

		  size = symtab_hdr->sh_info;
		  size *= sizeof (bfd_signed_vma);
		  local_got_refcounts
		    = (bfd_signed_vma *) bfd_zalloc (abfd, size);
		  if (local_got_refcounts == NULL)
		    return false;
		  elf_local_got_refcounts (abfd) = local_got_refcounts;
		}
	      if (local_got_refcounts[r_symndx] == 0)
		{
		  sgot->_raw_size += 4;

		  /* If we are generating a shared object, we need to
                     output a R_PPC_RELATIVE reloc so that the
                     dynamic linker can adjust this GOT entry.  */
		  if (info->shared)
		    srelgot->_raw_size += sizeof (Elf32_External_Rela);
		}
	      local_got_refcounts[r_symndx]++;
	    }
	  break;

	/* Indirect .sdata relocation */
	case R_PPC_EMB_SDAI16:
	  if (info->shared)
	    {
	      ((*_bfd_error_handler)
	       (_("%s: relocation %s cannot be used when making a shared object"),
		bfd_archive_filename (abfd), "R_PPC_EMB_SDAI16"));
	      return false;
	    }

	  if (srelgot == NULL && (h != NULL || info->shared))
	    {
	      srelgot = bfd_get_section_by_name (dynobj, ".rela.got");
	      if (srelgot == NULL)
		{
		  srelgot = bfd_make_section (dynobj, ".rela.got");
		  if (srelgot == NULL
		      || ! bfd_set_section_flags (dynobj, srelgot,
						  (SEC_ALLOC
						   | SEC_LOAD
						   | SEC_HAS_CONTENTS
						   | SEC_IN_MEMORY
						   | SEC_LINKER_CREATED
						   | SEC_READONLY))
		      || ! bfd_set_section_alignment (dynobj, srelgot, 2))
		    return false;
		}
	    }

	  if (!bfd_elf32_create_pointer_linker_section (abfd, info, sdata, h, rel))
	    return false;

	  break;

	/* Indirect .sdata2 relocation */
	case R_PPC_EMB_SDA2I16:
	  if (info->shared)
	    {
	      ((*_bfd_error_handler)
	       (_("%s: relocation %s cannot be used when making a shared object"),
		bfd_archive_filename (abfd), "R_PPC_EMB_SDA2I16"));
	      return false;
	    }

	  if (srelgot == NULL && (h != NULL || info->shared))
	    {
	      srelgot = bfd_get_section_by_name (dynobj, ".rela.got");
	      if (srelgot == NULL)
		{
		  srelgot = bfd_make_section (dynobj, ".rela.got");
		  if (srelgot == NULL
		      || ! bfd_set_section_flags (dynobj, srelgot,
						  (SEC_ALLOC
						   | SEC_LOAD
						   | SEC_HAS_CONTENTS
						   | SEC_IN_MEMORY
						   | SEC_LINKER_CREATED
						   | SEC_READONLY))
		      || ! bfd_set_section_alignment (dynobj, srelgot, 2))
		    return false;
		}
	    }

	  if (!bfd_elf32_create_pointer_linker_section (abfd, info, sdata2, h, rel))
	    return false;

	  break;

	case R_PPC_SDAREL16:
	case R_PPC_EMB_SDA2REL:
	case R_PPC_EMB_SDA21:
	  if (info->shared)
	    {
	      ((*_bfd_error_handler)
	       (_("%s: relocation %s cannot be used when making a shared object"),
		bfd_archive_filename (abfd),
		ppc_elf_howto_table[(int) ELF32_R_TYPE (rel->r_info)]->name));
	      return false;
	    }
	  break;

	case R_PPC_PLT32:
	case R_PPC_PLTREL24:
	case R_PPC_PLT16_LO:
	case R_PPC_PLT16_HI:
	case R_PPC_PLT16_HA:
#ifdef DEBUG
	  fprintf (stderr, "Reloc requires a PLT entry\n");
#endif
	  /* This symbol requires a procedure linkage table entry.  We
             actually build the entry in adjust_dynamic_symbol,
             because this might be a case of linking PIC code without
             linking in any dynamic objects, in which case we don't
             need to generate a procedure linkage table after all.  */

	  if (h == NULL)
	    {
	      /* It does not make sense to have a procedure linkage
                 table entry for a local symbol.  */
	      bfd_set_error (bfd_error_bad_value);
	      return false;
	    }

	  /* Make sure this symbol is output as a dynamic symbol.  */
	  if (h->dynindx == -1)
	    {
	      if (! bfd_elf32_link_record_dynamic_symbol (info, h))
		return false;
	    }
	  h->elf_link_hash_flags |= ELF_LINK_HASH_NEEDS_PLT;
	  h->plt.refcount++;
	  break;

	  /* The following relocations don't need to propagate the
	     relocation if linking a shared object since they are
	     section relative.  */
	case R_PPC_SECTOFF:
	case R_PPC_SECTOFF_LO:
	case R_PPC_SECTOFF_HI:
	case R_PPC_SECTOFF_HA:
	  break;

	  /* This refers only to functions defined in the shared library */
	case R_PPC_LOCAL24PC:
	  break;

	  /* This relocation describes the C++ object vtable hierarchy.
	     Reconstruct it for later use during GC.  */
	case R_PPC_GNU_VTINHERIT:
	  if (!_bfd_elf32_gc_record_vtinherit (abfd, sec, h, rel->r_offset))
	    return false;
	  break;

	  /* This relocation describes which C++ vtable entries are actually
	     used.  Record for later use during GC.  */
	case R_PPC_GNU_VTENTRY:
	  if (!_bfd_elf32_gc_record_vtentry (abfd, sec, h, rel->r_addend))
	    return false;
	  break;

	  /* When creating a shared object, we must copy these
	     relocs into the output file.  We create a reloc
	     section in dynobj and make room for the reloc.  */
	case R_PPC_REL24:
	case R_PPC_REL14:
	case R_PPC_REL14_BRTAKEN:
	case R_PPC_REL14_BRNTAKEN:
	case R_PPC_REL32:
	  if (h == NULL
	      || strcmp (h->root.root.string, "_GLOBAL_OFFSET_TABLE_") == 0
	      || SYMBOL_REFERENCES_LOCAL (info, h))
	    break;
	  /* fall through */

	default:
	  if (info->shared)
	    {
#ifdef DEBUG
	      fprintf (stderr, "ppc_elf_check_relocs need to create relocation for %s\n",
		       (h && h->root.root.string) ? h->root.root.string : "<unknown>");
#endif
	      if (sreloc == NULL)
		{
		  const char *name;

		  name = (bfd_elf_string_from_elf_section
			  (abfd,
			   elf_elfheader (abfd)->e_shstrndx,
			   elf_section_data (sec)->rel_hdr.sh_name));
		  if (name == NULL)
		    return false;

		  BFD_ASSERT (strncmp (name, ".rela", 5) == 0
			      && strcmp (bfd_get_section_name (abfd, sec),
					 name + 5) == 0);

		  sreloc = bfd_get_section_by_name (dynobj, name);
		  if (sreloc == NULL)
		    {
		      flagword flags;

		      sreloc = bfd_make_section (dynobj, name);
		      flags = (SEC_HAS_CONTENTS | SEC_READONLY
			       | SEC_IN_MEMORY | SEC_LINKER_CREATED);
		      if ((sec->flags & SEC_ALLOC) != 0)
			flags |= SEC_ALLOC | SEC_LOAD;
		      if (sreloc == NULL
			  || ! bfd_set_section_flags (dynobj, sreloc, flags)
			  || ! bfd_set_section_alignment (dynobj, sreloc, 2))
			return false;
		    }
		  if (sec->flags & SEC_READONLY)
		    info->flags |= DF_TEXTREL;
		}

	      sreloc->_raw_size += sizeof (Elf32_External_Rela);

	      /* FIXME: We should here do what the m68k and i386
		 backends do: if the reloc is pc-relative, record it
		 in case it turns out that the reloc is unnecessary
		 because the symbol is forced local by versioning or
		 we are linking with -Bdynamic.  Fortunately this
		 case is not frequent.  */
	    }

	  break;
	}
    }

  return true;
}

/* Return the section that should be marked against GC for a given
   relocation.  */

static asection *
ppc_elf_gc_mark_hook (sec, info, rel, h, sym)
     asection *sec;
     struct bfd_link_info *info ATTRIBUTE_UNUSED;
     Elf_Internal_Rela *rel;
     struct elf_link_hash_entry *h;
     Elf_Internal_Sym *sym;
{
  if (h != NULL)
    {
      switch (ELF32_R_TYPE (rel->r_info))
	{
	case R_PPC_GNU_VTINHERIT:
	case R_PPC_GNU_VTENTRY:
	  break;

	default:
	  switch (h->root.type)
	    {
	    case bfd_link_hash_defined:
	    case bfd_link_hash_defweak:
	      return h->root.u.def.section;

	    case bfd_link_hash_common:
	      return h->root.u.c.p->section;

	    default:
	      break;
	    }
	}
    }
  else
    return bfd_section_from_elf_index (sec->owner, sym->st_shndx);

  return NULL;
}

/* Update the got entry reference counts for the section being removed.  */

static boolean
ppc_elf_gc_sweep_hook (abfd, info, sec, relocs)
     bfd *abfd;
     struct bfd_link_info *info ATTRIBUTE_UNUSED;
     asection *sec;
     const Elf_Internal_Rela *relocs;
{
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes;
  bfd_signed_vma *local_got_refcounts;
  const Elf_Internal_Rela *rel, *relend;
  unsigned long r_symndx;
  struct elf_link_hash_entry *h;

  symtab_hdr = &elf_tdata (abfd)->symtab_hdr;
  sym_hashes = elf_sym_hashes (abfd);
  local_got_refcounts = elf_local_got_refcounts (abfd);

  relend = relocs + sec->reloc_count;
  for (rel = relocs; rel < relend; rel++)
    switch (ELF32_R_TYPE (rel->r_info))
      {
      case R_PPC_GOT16:
      case R_PPC_GOT16_LO:
      case R_PPC_GOT16_HI:
      case R_PPC_GOT16_HA:
	r_symndx = ELF32_R_SYM (rel->r_info);
	if (r_symndx >= symtab_hdr->sh_info)
	  {
	    h = sym_hashes[r_symndx - symtab_hdr->sh_info];
	    if (h->got.refcount > 0)
	      h->got.refcount--;
	  }
	else if (local_got_refcounts != NULL)
	  {
	    if (local_got_refcounts[r_symndx] > 0)
	      local_got_refcounts[r_symndx]--;
	  }
        break;

      case R_PPC_PLT32:
      case R_PPC_PLTREL24:
      case R_PPC_PLT16_LO:
      case R_PPC_PLT16_HI:
      case R_PPC_PLT16_HA:
	r_symndx = ELF32_R_SYM (rel->r_info);
	if (r_symndx >= symtab_hdr->sh_info)
	  {
	    h = sym_hashes[r_symndx - symtab_hdr->sh_info];
	    if (h->plt.refcount > 0)
	      h->plt.refcount--;
	  }
	break;

      default:
	break;
      }

  return true;
}

/* Hook called by the linker routine which adds symbols from an object
   file.  We use it to put .comm items in .sbss, and not .bss.  */

static boolean
ppc_elf_add_symbol_hook (abfd, info, sym, namep, flagsp, secp, valp)
     bfd *abfd;
     struct bfd_link_info *info;
     const Elf_Internal_Sym *sym;
     const char **namep ATTRIBUTE_UNUSED;
     flagword *flagsp ATTRIBUTE_UNUSED;
     asection **secp;
     bfd_vma *valp;
{
  if (sym->st_shndx == SHN_COMMON
      && !info->relocateable
      && sym->st_size <= elf_gp_size (abfd)
      && info->hash->creator->flavour == bfd_target_elf_flavour)
    {
      /* Common symbols less than or equal to -G nn bytes are automatically
	 put into .sdata.  */
      elf_linker_section_t *sdata
	= ppc_elf_create_linker_section (abfd, info, LINKER_SECTION_SDATA);

      if (!sdata->bss_section)
	{
	  bfd_size_type amt;

	  /* We don't go through bfd_make_section, because we don't
             want to attach this common section to DYNOBJ.  The linker
             will move the symbols to the appropriate output section
             when it defines common symbols.  */
	  amt = sizeof (asection);
	  sdata->bss_section = (asection *) bfd_zalloc (abfd, amt);
	  if (sdata->bss_section == NULL)
	    return false;
	  sdata->bss_section->name = sdata->bss_name;
	  sdata->bss_section->flags = SEC_IS_COMMON;
	  sdata->bss_section->output_section = sdata->bss_section;
	  amt = sizeof (asymbol);
	  sdata->bss_section->symbol = (asymbol *) bfd_zalloc (abfd, amt);
	  amt = sizeof (asymbol *);
	  sdata->bss_section->symbol_ptr_ptr =
	    (asymbol **) bfd_zalloc (abfd, amt);
	  if (sdata->bss_section->symbol == NULL
	      || sdata->bss_section->symbol_ptr_ptr == NULL)
	    return false;
	  sdata->bss_section->symbol->name = sdata->bss_name;
	  sdata->bss_section->symbol->flags = BSF_SECTION_SYM;
	  sdata->bss_section->symbol->section = sdata->bss_section;
	  *sdata->bss_section->symbol_ptr_ptr = sdata->bss_section->symbol;
	}

      *secp = sdata->bss_section;
      *valp = sym->st_size;
    }

  return true;
}

/* Finish up dynamic symbol handling.  We set the contents of various
   dynamic sections here.  */

static boolean
ppc_elf_finish_dynamic_symbol (output_bfd, info, h, sym)
     bfd *output_bfd;
     struct bfd_link_info *info;
     struct elf_link_hash_entry *h;
     Elf_Internal_Sym *sym;
{
  bfd *dynobj;

#ifdef DEBUG
  fprintf (stderr, "ppc_elf_finish_dynamic_symbol called for %s",
	   h->root.root.string);
#endif

  dynobj = elf_hash_table (info)->dynobj;
  BFD_ASSERT (dynobj != NULL);

  if (h->plt.offset != (bfd_vma) -1)
    {
      asection *splt;
      asection *srela;
      Elf_Internal_Rela rela;
      bfd_vma reloc_index;

#ifdef DEBUG
      fprintf (stderr, ", plt_offset = %d", h->plt.offset);
#endif

      /* This symbol has an entry in the procedure linkage table.  Set
         it up.  */

      BFD_ASSERT (h->dynindx != -1);

      splt = bfd_get_section_by_name (dynobj, ".plt");
      srela = bfd_get_section_by_name (dynobj, ".rela.plt");
      BFD_ASSERT (splt != NULL && srela != NULL);

      /* We don't need to fill in the .plt.  The ppc dynamic linker
	 will fill it in.  */

      /* Fill in the entry in the .rela.plt section.  */
      rela.r_offset = (splt->output_section->vma
		       + splt->output_offset
		       + h->plt.offset);
      rela.r_info = ELF32_R_INFO (h->dynindx, R_PPC_JMP_SLOT);
      rela.r_addend = 0;

      reloc_index = (h->plt.offset - PLT_INITIAL_ENTRY_SIZE) / PLT_SLOT_SIZE;
      if (reloc_index > PLT_NUM_SINGLE_ENTRIES)
	reloc_index -= (reloc_index - PLT_NUM_SINGLE_ENTRIES) / 2;
      bfd_elf32_swap_reloca_out (output_bfd, &rela,
				 ((Elf32_External_Rela *) srela->contents
				  + reloc_index));

      if ((h->elf_link_hash_flags & ELF_LINK_HASH_DEF_REGULAR) == 0)
	{
	  /* Mark the symbol as undefined, rather than as defined in
	     the .plt section.  Leave the value alone.  */
	  sym->st_shndx = SHN_UNDEF;
	  /* If the symbol is weak, we do need to clear the value.
	     Otherwise, the PLT entry would provide a definition for
	     the symbol even if the symbol wasn't defined anywhere,
	     and so the symbol would never be NULL.  */
	  if ((h->elf_link_hash_flags & ELF_LINK_HASH_REF_REGULAR_NONWEAK)
	      == 0)
	    sym->st_value = 0;
	}
    }

  if (h->got.offset != (bfd_vma) -1)
    {
      asection *sgot;
      asection *srela;
      Elf_Internal_Rela rela;

      /* This symbol has an entry in the global offset table.  Set it
         up.  */

      sgot = bfd_get_section_by_name (dynobj, ".got");
      srela = bfd_get_section_by_name (dynobj, ".rela.got");
      BFD_ASSERT (sgot != NULL && srela != NULL);

      rela.r_offset = (sgot->output_section->vma
		       + sgot->output_offset
		       + (h->got.offset &~ (bfd_vma) 1));

      /* If this is a -Bsymbolic link, and the symbol is defined
	 locally, we just want to emit a RELATIVE reloc.  The entry in
	 the global offset table will already have been initialized in
	 the relocate_section function.  */
      if (info->shared
	  && SYMBOL_REFERENCES_LOCAL (info, h))
	{
	  rela.r_info = ELF32_R_INFO (0, R_PPC_RELATIVE);
	  rela.r_addend = (h->root.u.def.value
			   + h->root.u.def.section->output_section->vma
			   + h->root.u.def.section->output_offset);
	}
      else
	{
	  BFD_ASSERT ((h->got.offset & 1) == 0);
	  rela.r_info = ELF32_R_INFO (h->dynindx, R_PPC_GLOB_DAT);
	  rela.r_addend = 0;
	}

      bfd_put_32 (output_bfd, (bfd_vma) 0, sgot->contents + h->got.offset);
      bfd_elf32_swap_reloca_out (output_bfd, &rela,
				 ((Elf32_External_Rela *) srela->contents
				  + srela->reloc_count));
      ++srela->reloc_count;
    }

  if ((h->elf_link_hash_flags & ELF_LINK_HASH_NEEDS_COPY) != 0)
    {
      asection *s;
      Elf_Internal_Rela rela;

      /* This symbols needs a copy reloc.  Set it up.  */

#ifdef DEBUG
      fprintf (stderr, ", copy");
#endif

      BFD_ASSERT (h->dynindx != -1);

      if (h->size <= elf_gp_size (dynobj))
	s = bfd_get_section_by_name (h->root.u.def.section->owner,
				     ".rela.sbss");
      else
	s = bfd_get_section_by_name (h->root.u.def.section->owner,
				     ".rela.bss");
      BFD_ASSERT (s != NULL);

      rela.r_offset = (h->root.u.def.value
		       + h->root.u.def.section->output_section->vma
		       + h->root.u.def.section->output_offset);
      rela.r_info = ELF32_R_INFO (h->dynindx, R_PPC_COPY);
      rela.r_addend = 0;
      bfd_elf32_swap_reloca_out (output_bfd, &rela,
				 ((Elf32_External_Rela *) s->contents
				  + s->reloc_count));
      ++s->reloc_count;
    }

#ifdef DEBUG
  fprintf (stderr, "\n");
#endif

  /* Mark some specially defined symbols as absolute.  */
  if (strcmp (h->root.root.string, "_DYNAMIC") == 0
      || strcmp (h->root.root.string, "_GLOBAL_OFFSET_TABLE_") == 0
      || strcmp (h->root.root.string, "_PROCEDURE_LINKAGE_TABLE_") == 0)
    sym->st_shndx = SHN_ABS;

  return true;
}

/* Finish up the dynamic sections.  */

static boolean
ppc_elf_finish_dynamic_sections (output_bfd, info)
     bfd *output_bfd;
     struct bfd_link_info *info;
{
  asection *sdyn;
  bfd *dynobj = elf_hash_table (info)->dynobj;
  asection *sgot = bfd_get_section_by_name (dynobj, ".got");

#ifdef DEBUG
  fprintf (stderr, "ppc_elf_finish_dynamic_sections called\n");
#endif

  sdyn = bfd_get_section_by_name (dynobj, ".dynamic");

  if (elf_hash_table (info)->dynamic_sections_created)
    {
      asection *splt;
      Elf32_External_Dyn *dyncon, *dynconend;

      splt = bfd_get_section_by_name (dynobj, ".plt");
      BFD_ASSERT (splt != NULL && sdyn != NULL);

      dyncon = (Elf32_External_Dyn *) sdyn->contents;
      dynconend = (Elf32_External_Dyn *) (sdyn->contents + sdyn->_raw_size);
      for (; dyncon < dynconend; dyncon++)
	{
	  Elf_Internal_Dyn dyn;
	  const char *name;
	  boolean size;

	  bfd_elf32_swap_dyn_in (dynobj, dyncon, &dyn);

	  switch (dyn.d_tag)
	    {
	    case DT_PLTGOT:   name = ".plt";	  size = false; break;
	    case DT_PLTRELSZ: name = ".rela.plt"; size = true;  break;
	    case DT_JMPREL:   name = ".rela.plt"; size = false; break;
	    default:	      name = NULL;	  size = false; break;
	    }

	  if (name != NULL)
	    {
	      asection *s;

	      s = bfd_get_section_by_name (output_bfd, name);
	      if (s == NULL)
		dyn.d_un.d_val = 0;
	      else
		{
		  if (! size)
		    dyn.d_un.d_ptr = s->vma;
		  else
		    {
		      if (s->_cooked_size != 0)
			dyn.d_un.d_val = s->_cooked_size;
		      else
			dyn.d_un.d_val = s->_raw_size;
		    }
		}
	      bfd_elf32_swap_dyn_out (output_bfd, &dyn, dyncon);
	    }
	}
    }

  /* Add a blrl instruction at _GLOBAL_OFFSET_TABLE_-4 so that a function can
     easily find the address of the _GLOBAL_OFFSET_TABLE_.  */
  if (sgot)
    {
      unsigned char *contents = sgot->contents;
      bfd_put_32 (output_bfd, (bfd_vma) 0x4e800021 /* blrl */, contents);

      if (sdyn == NULL)
	bfd_put_32 (output_bfd, (bfd_vma) 0, contents+4);
      else
	bfd_put_32 (output_bfd,
		    sdyn->output_section->vma + sdyn->output_offset,
		    contents+4);

      elf_section_data (sgot->output_section)->this_hdr.sh_entsize = 4;
    }

  return true;
}

/* The RELOCATE_SECTION function is called by the ELF backend linker
   to handle the relocations for a section.

   The relocs are always passed as Rela structures; if the section
   actually uses Rel structures, the r_addend field will always be
   zero.

   This function is responsible for adjust the section contents as
   necessary, and (if using Rela relocs and generating a
   relocateable output file) adjusting the reloc addend as
   necessary.

   This function does not have to worry about setting the reloc
   address or the reloc symbol index.

   LOCAL_SYMS is a pointer to the swapped in local symbols.

   LOCAL_SECTIONS is an array giving the section in the input file
   corresponding to the st_shndx field of each local symbol.

   The global hash table entry for the global symbols can be found
   via elf_sym_hashes (input_bfd).

   When generating relocateable output, this function must handle
   STB_LOCAL/STT_SECTION symbols specially.  The output symbol is
   going to be the section symbol corresponding to the output
   section, which means that the addend must be adjusted
   accordingly.  */

static boolean
ppc_elf_relocate_section (output_bfd, info, input_bfd, input_section,
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
  Elf_Internal_Shdr *symtab_hdr		  = &elf_tdata (input_bfd)->symtab_hdr;
  struct elf_link_hash_entry **sym_hashes = elf_sym_hashes (input_bfd);
  bfd *dynobj				  = elf_hash_table (info)->dynobj;
  elf_linker_section_t *sdata		  = (dynobj) ? elf_linker_section (dynobj, LINKER_SECTION_SDATA)  : NULL;
  elf_linker_section_t *sdata2		  = (dynobj) ? elf_linker_section (dynobj, LINKER_SECTION_SDATA2) : NULL;
  Elf_Internal_Rela *rel		  = relocs;
  Elf_Internal_Rela *relend		  = relocs + input_section->reloc_count;
  asection *sreloc			  = NULL;
  asection *splt;
  asection *sgot;
  bfd_vma *local_got_offsets;
  boolean ret				  = true;
  long insn;
  asection *sdata_sec                     = NULL;
  asection *sbss_sec                     = NULL;

#ifdef DEBUG
  fprintf (stderr, "ppc_elf_relocate_section called for %s section %s, %ld relocations%s\n",
	   bfd_archive_filename (input_bfd),
	   bfd_section_name(input_bfd, input_section),
	   (long) input_section->reloc_count,
	   (info->relocateable) ? " (relocatable)" : "");
#endif

  if (info->relocateable)
    return true;

  if (!ppc_elf_howto_table[R_PPC_ADDR32])
    /* Initialize howto table if needed.  */
    ppc_elf_howto_init ();

  if (!strcmp(bfd_section_name(output_bfd, input_section), ".sdata") ||
      !strcmp(bfd_section_name(output_bfd, input_section), ".sbss"))
  {
    sdata_sec = bfd_get_section_by_name(output_bfd, ".sdata");
    if (sdata_sec)
      sdata_sec = sdata_sec->output_section;
    sbss_sec = bfd_get_section_by_name(output_bfd, ".sbss");
    if (sbss_sec)
      sbss_sec = sbss_sec->output_section;
  }
 
  local_got_offsets = elf_local_got_offsets (input_bfd);

  splt = sgot = NULL;
  if (dynobj != NULL)
    {
      splt = bfd_get_section_by_name (dynobj, ".plt");
      sgot = bfd_get_section_by_name (dynobj, ".got");
    }

  for (; rel < relend; rel++)
    {
      enum elf_ppc_reloc_type r_type	= (enum elf_ppc_reloc_type)ELF32_R_TYPE (rel->r_info);
      bfd_vma offset			= rel->r_offset;
      bfd_vma addend			= rel->r_addend;
      bfd_reloc_status_type r		= bfd_reloc_other;
      Elf_Internal_Sym *sym		= (Elf_Internal_Sym *) 0;
      asection *sec			= (asection *) 0;
      struct elf_link_hash_entry *h	= (struct elf_link_hash_entry *) 0;
      const char *sym_name		= (const char *) 0;
      boolean copy                      = false;
      reloc_howto_type *howto;
      unsigned long r_symndx;
      bfd_vma relocation;
      int will_become_local;

      /* Unknown relocation handling */
      if ((unsigned) r_type >= (unsigned) R_PPC_max
	  || !ppc_elf_howto_table[(int) r_type])
	{
	  (*_bfd_error_handler) (_("%s: unknown relocation type %d"),
				 bfd_archive_filename (input_bfd),
				 (int) r_type);

	  bfd_set_error (bfd_error_bad_value);
	  ret = false;
	  continue;
	}

      howto = ppc_elf_howto_table[(int) r_type];
      r_symndx = ELF32_R_SYM (rel->r_info);

      if (r_symndx < symtab_hdr->sh_info)
	{
	  sym = local_syms + r_symndx;
	  sec = local_sections[r_symndx];
	  sym_name = "<local symbol>";

	  relocation = _bfd_elf_rela_local_sym (output_bfd, sym, sec, rel);
	  addend = rel->r_addend;
	  /* Relocs to local symbols are always resolved.  */
	  will_become_local = 1;
	}
      else
	{
	  h = sym_hashes[r_symndx - symtab_hdr->sh_info];
	  while (h->root.type == bfd_link_hash_indirect
		 || h->root.type == bfd_link_hash_warning)
	    h = (struct elf_link_hash_entry *) h->root.u.i.link;
	  sym_name = h->root.root.string;

	  /* Can this relocation be resolved immediately?  */
	  will_become_local = SYMBOL_REFERENCES_LOCAL (info, h);

	  if (h->root.type == bfd_link_hash_defined
	      || h->root.type == bfd_link_hash_defweak)
	    {
	      sec = h->root.u.def.section;
	      if (((r_type == R_PPC_PLT32
		    || r_type == R_PPC_PLTREL24)
		   && splt != NULL
		   && h->plt.offset != (bfd_vma) -1)
		  || (r_type == R_PPC_LOCAL24PC
		      && sec->output_section == NULL)
		  || ((r_type == R_PPC_GOT16
		       || r_type == R_PPC_GOT16_LO
		       || r_type == R_PPC_GOT16_HI
		       || r_type == R_PPC_GOT16_HA)
		      && elf_hash_table (info)->dynamic_sections_created
		      && (! info->shared || ! will_become_local))
		  || (info->shared
 		      && ! will_become_local
		      && ((input_section->flags & SEC_ALLOC) != 0
			  /* Testing SEC_DEBUGGING here may be wrong.
                             It's here to avoid a crash when
                             generating a shared library with DWARF
                             debugging information.  */
			  || ((input_section->flags & SEC_DEBUGGING) != 0
			      && (h->elf_link_hash_flags
				  & ELF_LINK_HASH_DEF_DYNAMIC) != 0))
		      && (r_type == R_PPC_ADDR32
			  || r_type == R_PPC_ADDR24
			  || r_type == R_PPC_ADDR16
			  || r_type == R_PPC_ADDR16_LO
			  || r_type == R_PPC_ADDR16_HI
			  || r_type == R_PPC_ADDR16_HA
			  || r_type == R_PPC_ADDR14
			  || r_type == R_PPC_ADDR14_BRTAKEN
			  || r_type == R_PPC_ADDR14_BRNTAKEN
			  || r_type == R_PPC_COPY
			  || r_type == R_PPC_GLOB_DAT
			  || r_type == R_PPC_JMP_SLOT
			  || r_type == R_PPC_UADDR32
			  || r_type == R_PPC_UADDR16
			  || r_type == R_PPC_SDAREL16
			  || r_type == R_PPC_EMB_NADDR32
			  || r_type == R_PPC_EMB_NADDR16
			  || r_type == R_PPC_EMB_NADDR16_LO
			  || r_type == R_PPC_EMB_NADDR16_HI
			  || r_type == R_PPC_EMB_NADDR16_HA
			  || r_type == R_PPC_EMB_SDAI16
			  || r_type == R_PPC_EMB_SDA2I16
			  || r_type == R_PPC_EMB_SDA2REL
			  || r_type == R_PPC_EMB_SDA21
			  || r_type == R_PPC_EMB_MRKREF
			  || r_type == R_PPC_EMB_BIT_FLD
			  || r_type == R_PPC_EMB_RELSDA
			  || ((r_type == R_PPC_REL24
			       || r_type == R_PPC_REL32
			       || r_type == R_PPC_REL14
			       || r_type == R_PPC_REL14_BRTAKEN
			       || r_type == R_PPC_REL14_BRNTAKEN
			       || r_type == R_PPC_RELATIVE)
			      && strcmp (h->root.root.string,
					 "_GLOBAL_OFFSET_TABLE_") != 0))))
		{
		  /* In these cases, we don't need the relocation
                     value.  We check specially because in some
                     obscure cases sec->output_section will be NULL.  */
		  relocation = 0;
		}
	      else if (sec->output_section == NULL)
		{
                  (*_bfd_error_handler)
                    (_("%s: warning: unresolvable relocation against symbol `%s' from %s section"),
                     bfd_archive_filename (input_bfd), h->root.root.string,
                     bfd_get_section_name (input_bfd, input_section));
		  relocation = 0;
		}
	      else
		relocation = (h->root.u.def.value
			      + sec->output_section->vma
			      + sec->output_offset);
	    }
	  else if (h->root.type == bfd_link_hash_undefweak)
	    relocation = 0;
	  else if (info->shared
		   && (!info->symbolic || info->allow_shlib_undefined)
		   && !info->no_undefined
		   && ELF_ST_VISIBILITY (h->other) == STV_DEFAULT)
	    relocation = 0;
	  else
	    {
	      if (! (*info->callbacks->undefined_symbol) (info,
							 h->root.root.string,
							 input_bfd,
							 input_section,
							 rel->r_offset,
							 (!info->shared
							  || info->no_undefined
							  || ELF_ST_VISIBILITY (h->other))))
		return false;
	      relocation = 0;
	    }
	}

      switch ((int) r_type)
	{
	default:
	  (*_bfd_error_handler) (_("%s: unknown relocation type %d for symbol %s"),
				 bfd_archive_filename (input_bfd),
				 (int) r_type, sym_name);

	  bfd_set_error (bfd_error_bad_value);
	  ret = false;
	  continue;

	case (int) R_PPC_NONE:
	  continue;

	/* Relocations that need no special processing.  */
	case (int) R_PPC_LOCAL24PC:
	  /* It makes no sense to point a local relocation
	     at a symbol not in this object.  */
	  if (h != NULL
	      && (h->root.type == bfd_link_hash_defined
		  || h->root.type == bfd_link_hash_defweak)
	      && sec->output_section == NULL)
	    {
	      if (! (*info->callbacks->undefined_symbol) (info,
							  h->root.root.string,
							  input_bfd,
							  input_section,
							  rel->r_offset,
							  true))
		return false;
	      continue;
	    }
	  break;

	/* Relocations that may need to be propagated if this is a shared
           object.  */
	case (int) R_PPC_REL24:
	case (int) R_PPC_REL32:
	case (int) R_PPC_REL14:
	  /* If these relocations are not to a named symbol, they can be
	     handled right here, no need to bother the dynamic linker.  */
	  if (info->shared && (h == NULL
	      || strcmp (h->root.root.string, "_GLOBAL_OFFSET_TABLE_") == 0
              || SYMBOL_REFERENCES_LOCAL (info, h)))
	    break;
	/* fall through */

	/* Relocations that always need to be propagated if this is a shared
           object.  */
	case (int) R_PPC_ADDR32:
	case (int) R_PPC_ADDR24:
	case (int) R_PPC_ADDR16:
	case (int) R_PPC_ADDR16_LO:
	case (int) R_PPC_ADDR16_HI:
	case (int) R_PPC_ADDR16_HA:
	case (int) R_PPC_ADDR14:
	case (int) R_PPC_UADDR32:
	case (int) R_PPC_UADDR16:
	  if (info->shared && r_symndx != 0)
	    {
	      Elf_Internal_Rela outrel;
	      int skip;

#ifdef DEBUG
	      fprintf (stderr, "ppc_elf_relocate_section need to create relocation for %s\n",
		       (h && h->root.root.string) ? h->root.root.string : "<unknown>");
#endif

	      /* When generating a shared object, these relocations
                 are copied into the output file to be resolved at run
                 time.  */

	      if (sreloc == NULL)
		{
		  const char *name;

		  name = (bfd_elf_string_from_elf_section
			  (input_bfd,
			   elf_elfheader (input_bfd)->e_shstrndx,
			   elf_section_data (input_section)->rel_hdr.sh_name));
		  if (name == NULL)
		    return false;

		  BFD_ASSERT (strncmp (name, ".rela", 5) == 0
			      && strcmp (bfd_get_section_name (input_bfd,
							       input_section),
					 name + 5) == 0);

		  sreloc = bfd_get_section_by_name (dynobj, name);
		  BFD_ASSERT (sreloc != NULL);
		}

	      skip = 0;

	      outrel.r_offset =
		_bfd_elf_section_offset (output_bfd, info, input_section,
					 rel->r_offset);
	      if (outrel.r_offset == (bfd_vma) -1
		  || outrel.r_offset == (bfd_vma) -2)
		skip = (int) outrel.r_offset;
	      outrel.r_offset += (input_section->output_section->vma
				  + input_section->output_offset);

	      if (skip)
		memset (&outrel, 0, sizeof outrel);
	      /* h->dynindx may be -1 if this symbol was marked to
                 become local.  */
	      else if (! will_become_local)
		{
		  outrel.r_info = ELF32_R_INFO (h->dynindx, r_type);
		  outrel.r_addend = rel->r_addend;
		}
	      else
		{
		  if (r_type == R_PPC_ADDR32)
		    {
		      outrel.r_info = ELF32_R_INFO (0, R_PPC_RELATIVE);
		      outrel.r_addend = relocation + rel->r_addend;
		    }
		  else
		    {
		      long indx;

		      if (h == NULL)
			sec = local_sections[r_symndx];
		      else
			{
			  BFD_ASSERT (h->root.type == bfd_link_hash_defined
				      || (h->root.type
					  == bfd_link_hash_defweak));
			  sec = h->root.u.def.section;
			}
		      if (sec != NULL && bfd_is_abs_section (sec))
			indx = 0;
		      else if (sec == NULL || sec->owner == NULL)
			{
			  bfd_set_error (bfd_error_bad_value);
			  return false;
			}
		      else
			{
			  asection *osec;

			  osec = sec->output_section;
			  indx = elf_section_data (osec)->dynindx;
			  BFD_ASSERT (indx > 0);
#ifdef DEBUG
			  if (indx <= 0)
			    {
			      printf ("indx=%d section=%s flags=%08x name=%s\n",
				     indx, osec->name, osec->flags,
				     h->root.root.string);
			    }
#endif
			}

		      outrel.r_info = ELF32_R_INFO (indx, r_type);
		      outrel.r_addend = relocation + rel->r_addend;
		    }
		}

	      bfd_elf32_swap_reloca_out (output_bfd, &outrel,
					 (((Elf32_External_Rela *)
					   sreloc->contents)
					  + sreloc->reloc_count));
	      ++sreloc->reloc_count;

	      if (skip == -1)
		continue;

	      /* This reloc will be computed at runtime.  We clear the memory
		 so that it contains predictable value.  */
	      if (! skip
		  && ((input_section->flags & SEC_ALLOC) != 0
		      || ELF32_R_TYPE (outrel.r_info) != R_PPC_RELATIVE))
		{
		  relocation = howto->pc_relative ? outrel.r_offset : 0;
		  addend = 0;
		  break;
		}
	    }
	  else if (r_type == R_PPC_REL24 || r_type == R_PPC_REL14)
	    {
	      if (sec->output_section != input_section->output_section)
		{
		  (*_bfd_error_handler) ("%s: The target (%s) of a %s relocation is in the wrong section (%s)",
					 bfd_get_filename (input_bfd),
					 sym_name,
					 ppc_elf_howto_table[ (int)r_type ]->name,
					 bfd_get_section_name (abfd, sec));
		  bfd_set_error (bfd_error_bad_value);
		  ret = false;
		  continue;	 
		}
	      break;
	    }
	  else if (r_type == R_PPC_REL32)
	    {
	      if (sec->output_section != input_section->output_section)
		copy = true;
	      else
		break;
	    }
	  else if (ddr_ptr && sec && r_type == R_PPC_ADDR32 && 
		   (sec->output_section == sdata_sec ||
		    sec->output_section == sbss_sec ||
		    !strcmp(bfd_get_section_name(abfd, sec), "COMMON") ||
		    !strcmp(bfd_get_section_name(abfd, sec), ".data") ||
		    !strcmp(bfd_get_section_name(abfd, sec), ".bss")) &&
		   (input_section->output_section == sdata_sec ||
		    input_section->output_section == sbss_sec ||
		    !strcmp(bfd_get_section_name(abfd, input_section), "COMMON") ||
		    !strcmp(bfd_get_section_name(abfd, input_section), ".data") ||
		    !strcmp(bfd_get_section_name(abfd, input_section), ".bss")))
	    {
	      ++ddr_count;
	      *ddr_ptr++ = input_section->output_offset + offset;
	      copy = true;
	      break;
	    }
	  else if (sec && !bfd_is_abs_section(sec))
	    {
	      copy = true;
	      break;
	    }

	  if (copy && ddr_ptr && sec &&
	      (sec->output_section == sdata_sec ||
	       sec->output_section == sbss_sec ||
	       !strcmp(bfd_get_section_name(abfd, sec), "COMMON") ||
	       !strcmp(bfd_get_section_name(abfd, sec), ".data") ||
	       !strcmp(bfd_get_section_name(abfd, sec), ".bss")) /*&&
	      (r_type != R_PPC_ADDR32 ||
	       !(input_section->output_section == sdata_sec->output_section ||
		 input_section->output_section == sbss_sec->output_section ||
		 !strcmp(bfd_get_section_name(abfd, input_section), "COMMON") ||
		 !strcmp(bfd_get_section_name(abfd, input_section), ".data") ||
		 !strcmp(bfd_get_section_name(abfd, input_section), ".bss")))*/)
	    {
	      (*_bfd_error_handler) ("%s: The target (%s) of a %s relocation is in the wrong section (%s)",
				     bfd_get_filename (input_bfd),
				     sym_name,
				     ppc_elf_howto_table[ (int)r_type ]->name,
				     bfd_get_section_name (abfd, sec));
	      bfd_set_error (bfd_error_bad_value);
	      ret = false;
	      continue;	    
	    }

	  /* Arithmetic adjust relocations that aren't going into a
	     shared object.  */
	  if (r_type == R_PPC_ADDR16_HA
	      /* It's just possible that this symbol is a weak symbol
		 that's not actually defined anywhere. In that case,
		 'sec' would be NULL, and we should leave the symbol
		 alone (it will be set to zero elsewhere in the link).  */
	      && sec != NULL)
	    {
	      addend += ((relocation + addend) & 0x8000) << 1;
	    }
	  break;

	/* branch taken prediction relocations */
	case (int) R_PPC_ADDR14_BRTAKEN:
	case (int) R_PPC_REL14_BRTAKEN:
	  insn = bfd_get_32 (output_bfd, contents + offset);
	  if ((relocation - offset) & 0x8000)
	    insn &= ~BRANCH_PREDICT_BIT;
	  else
	    insn |= BRANCH_PREDICT_BIT;
	  bfd_put_32 (output_bfd, (bfd_vma) insn, contents + offset);
	  break;

	/* branch not taken predicition relocations */
	case (int) R_PPC_ADDR14_BRNTAKEN:
	case (int) R_PPC_REL14_BRNTAKEN:
	  insn = bfd_get_32 (output_bfd, contents + offset);
	  if ((relocation - offset) & 0x8000)
	    insn |= BRANCH_PREDICT_BIT;
	  else
	    insn &= ~BRANCH_PREDICT_BIT;
	  bfd_put_32 (output_bfd, (bfd_vma) insn, contents + offset);
	  break;

	/* GOT16 relocations */
	case (int) R_PPC_GOT16:
	case (int) R_PPC_GOT16_LO:
	case (int) R_PPC_GOT16_HI:
	case (int) R_PPC_GOT16_HA:
	  /* Relocation is to the entry for this symbol in the global
             offset table.  */
	  BFD_ASSERT (sgot != NULL);

	  if (h != NULL)
	    {
	      bfd_vma off;

	      off = h->got.offset;
	      BFD_ASSERT (off != (bfd_vma) -1);

	      if (! elf_hash_table (info)->dynamic_sections_created
		  || (info->shared
		      && SYMBOL_REFERENCES_LOCAL (info, h)))
		{
		  /* This is actually a static link, or it is a
                     -Bsymbolic link and the symbol is defined
                     locally.  We must initialize this entry in the
                     global offset table.  Since the offset must
                     always be a multiple of 4, we use the least
                     significant bit to record whether we have
                     initialized it already.

		     When doing a dynamic link, we create a .rela.got
		     relocation entry to initialize the value.  This
		     is done in the finish_dynamic_symbol routine.  */
		  if ((off & 1) != 0)
		    off &= ~1;
		  else
		    {
		      bfd_put_32 (output_bfd, relocation,
				  sgot->contents + off);
		      h->got.offset |= 1;
		    }
		}

	      relocation = sgot->output_offset + off - 4;
	    }
	  else
	    {
	      bfd_vma off;

	      BFD_ASSERT (local_got_offsets != NULL
			  && local_got_offsets[r_symndx] != (bfd_vma) -1);

	      off = local_got_offsets[r_symndx];

	      /* The offset must always be a multiple of 4.  We use
		 the least significant bit to record whether we have
		 already processed this entry.  */
	      if ((off & 1) != 0)
		off &= ~1;
	      else
		{

		  if (info->shared)
		    {
		      asection *srelgot;
		      Elf_Internal_Rela outrel;

		      /* We need to generate a R_PPC_RELATIVE reloc
			 for the dynamic linker.  */
		      srelgot = bfd_get_section_by_name (dynobj, ".rela.got");
		      BFD_ASSERT (srelgot != NULL);

		      outrel.r_offset = (sgot->output_section->vma
					 + sgot->output_offset
					 + off);
		      outrel.r_info = ELF32_R_INFO (0, R_PPC_RELATIVE);
		      outrel.r_addend = relocation;
		      bfd_elf32_swap_reloca_out (output_bfd, &outrel,
						 (((Elf32_External_Rela *)
						   srelgot->contents)
						  + srelgot->reloc_count));
		      ++srelgot->reloc_count;
		      relocation = 0;
		    }

		  bfd_put_32 (output_bfd, relocation, sgot->contents + off);
		  local_got_offsets[r_symndx] |= 1;
		}

	      relocation = sgot->output_offset + off - 4;
	    }
	  break;

	/* Indirect .sdata relocation */
	case (int) R_PPC_EMB_SDAI16:
	  BFD_ASSERT (sdata != NULL);
	  relocation = bfd_elf32_finish_pointer_linker_section (output_bfd, input_bfd, info,
								sdata, h, relocation, rel,
								R_PPC_RELATIVE);
	  break;

	/* Indirect .sdata2 relocation */
	case (int) R_PPC_EMB_SDA2I16:
	  BFD_ASSERT (sdata2 != NULL);
	  relocation = bfd_elf32_finish_pointer_linker_section (output_bfd, input_bfd, info,
								sdata2, h, relocation, rel,
								R_PPC_RELATIVE);
	  break;

	/* Handle the TOC16 reloc.  We want to use the offset within the .got
	   section, not the actual VMA.  This is appropriate when generating
	   an embedded ELF object, for which the .got section acts like the
	   AIX .toc section.  */
	case (int) R_PPC_TOC16:			/* phony GOT16 relocations */
	  BFD_ASSERT (sec != (asection *) 0);
	  BFD_ASSERT (bfd_is_und_section (sec)
		      || strcmp (bfd_get_section_name (abfd, sec), ".got") == 0
		      || strcmp (bfd_get_section_name (abfd, sec), ".cgot") == 0)

	  addend -= sec->output_section->vma + sec->output_offset + 0x8000;
	  break;

	case (int) R_PPC_PLTREL24:
	  /* Relocation is to the entry for this symbol in the
             procedure linkage table.  */
	  BFD_ASSERT (h != NULL);

	  if (h->plt.offset == (bfd_vma) -1
	      || splt == NULL)
	    {
	      /* We didn't make a PLT entry for this symbol.  This
                 happens when statically linking PIC code, or when
                 using -Bsymbolic.  */
	      break;
	    }

	  relocation = (splt->output_section->vma
			+ splt->output_offset
			+ h->plt.offset);
	  break;

	/* relocate against _SDA_BASE_ */
	case (int) R_PPC_SDAREL16:
	  {
	    const char *name;

	    BFD_ASSERT (sec != (asection *) 0);
	    name = bfd_get_section_name (abfd, sec->output_section);
	    if (strcmp (name, ".sdata") != 0
		&& strcmp (name, ".sbss") != 0)
	      {
		(*_bfd_error_handler) (_("%s: The target (%s) of a %s relocation is in the wrong output section (%s)"),
				       bfd_archive_filename (input_bfd),
				       sym_name,
				       ppc_elf_howto_table[(int) r_type]->name,
				       name);
	      }
	    addend -= (sdata->sym_hash->root.u.def.value
		       + sdata->sym_hash->root.u.def.section->output_section->vma
		       /*+ sdata->sym_hash->root.u.def.section->output_offset*/);
	  }
	  break;

	/* relocate against _SDA_BASE_, in large data mode */
	case (int)R_PPC_MORPHOS_DREL:
	case (int)R_PPC_MORPHOS_DREL_LO:
	case (int)R_PPC_MORPHOS_DREL_HI:
	case (int)R_PPC_MORPHOS_DREL_HA:
	  BFD_ASSERT (sec != (asection *)0);
	  if (strcmp (bfd_get_section_name (abfd, sec), ".sdata") != 0
	      && strcmp (bfd_get_section_name (abfd, sec), ".data") != 0
	      && strcmp (bfd_get_section_name (abfd, sec), ".bss") != 0
	      && strcmp (bfd_get_section_name (abfd, sec), ".sbss") != 0
	      && strcmp (bfd_get_section_name (abfd, sec), "COMMON") != 0)
	    {
	      (*_bfd_error_handler) ("%s: The target (%s) of a %s relocation is in the wrong section (%s)",
				     bfd_get_filename (input_bfd),
				     sym_name,
				     ppc_elf_howto_table[ (int)r_type ]->name,
				     bfd_get_section_name (abfd, sec));

	      bfd_set_error (bfd_error_bad_value);
	      ret = false;
	      continue;
	    }
	  /*printf("DREL: addend = %x, sdata->val = %x, vma = %x, output_offset = %x\n",
		 addend, sdata->sym_hash->root.u.def.value,
		 sdata->sym_hash->root.u.def.section->output_section->vma,
		 sdata->sym_hash->root.u.def.section->output_offset);*/
	  addend -= (sdata->sym_hash->root.u.def.value
		     + sdata->sym_hash->root.u.def.section->output_section->vma
		     /*+ sdata->sym_hash->root.u.def.section->output_offset*/);
	  if (r_type == R_PPC_MORPHOS_DREL_HA)
	    addend += ((relocation + addend) & 0x8000) << 1;
	  break;
	  
	/* relocate against _SDA2_BASE_ */
	case (int) R_PPC_EMB_SDA2REL:
	  {
	    const char *name;

	    BFD_ASSERT (sec != (asection *) 0);
	    name = bfd_get_section_name (abfd, sec->output_section);
	    if (strcmp (name, ".sdata2") != 0 && strcmp (name, ".sbss2") != 0)
	      {
		(*_bfd_error_handler) (_("%s: The target (%s) of a %s relocation is in the wrong output section (%s)"),
				       bfd_archive_filename (input_bfd),
				       sym_name,
				       ppc_elf_howto_table[(int) r_type]->name,
				       name);

		bfd_set_error (bfd_error_bad_value);
		ret = false;
		continue;
	      }
	    addend -= (sdata2->sym_hash->root.u.def.value
		       + sdata2->sym_hash->root.u.def.section->output_section->vma
		       + sdata2->sym_hash->root.u.def.section->output_offset);
	  }
	  break;

	/* relocate against either _SDA_BASE_, _SDA2_BASE_, or 0 */
	case (int) R_PPC_EMB_SDA21:
	case (int) R_PPC_EMB_RELSDA:
	  {
	    const char *name;
	    int reg;

	    BFD_ASSERT (sec != (asection *) 0);
	    name = bfd_get_section_name (abfd, sec->output_section);
	    if (strcmp (name, ".sdata") == 0 || strcmp (name, ".sbss") == 0)
	      {
		reg = 13;
		addend -= (sdata->sym_hash->root.u.def.value
			   + sdata->sym_hash->root.u.def.section->output_section->vma
			   + sdata->sym_hash->root.u.def.section->output_offset);
	      }

	    else if (strcmp (name, ".sdata2") == 0
		     || strcmp (name, ".sbss2") == 0)
	      {
		reg = 2;
		addend -= (sdata2->sym_hash->root.u.def.value
			   + sdata2->sym_hash->root.u.def.section->output_section->vma
			   + sdata2->sym_hash->root.u.def.section->output_offset);
	      }

	    else if (strcmp (name, ".PPC.EMB.sdata0") == 0
		     || strcmp (name, ".PPC.EMB.sbss0") == 0)
	      {
		reg = 0;
	      }

	    else
	      {
		(*_bfd_error_handler) (_("%s: The target (%s) of a %s relocation is in the wrong output section (%s)"),
				       bfd_archive_filename (input_bfd),
				       sym_name,
				       ppc_elf_howto_table[(int) r_type]->name,
				       name);

		bfd_set_error (bfd_error_bad_value);
		ret = false;
		continue;
	      }

	    if (r_type == R_PPC_EMB_SDA21)
	      {			/* fill in register field */
		insn = bfd_get_32 (output_bfd, contents + offset);
		insn = (insn & ~RA_REGISTER_MASK) | (reg << RA_REGISTER_SHIFT);
		bfd_put_32 (output_bfd, (bfd_vma) insn, contents + offset);
	      }
	  }
	  break;

	/* Relocate against the beginning of the section */
	case (int) R_PPC_SECTOFF:
	case (int) R_PPC_SECTOFF_LO:
	case (int) R_PPC_SECTOFF_HI:
	  BFD_ASSERT (sec != (asection *) 0);
	  addend -= sec->output_section->vma;
	  break;

	case (int) R_PPC_SECTOFF_HA:
	  BFD_ASSERT (sec != (asection *) 0);
	  addend -= sec->output_section->vma;
	  addend += ((relocation + addend) & 0x8000) << 1;
	  break;

	/* Negative relocations */
	case (int) R_PPC_EMB_NADDR32:
	case (int) R_PPC_EMB_NADDR16:
	case (int) R_PPC_EMB_NADDR16_LO:
	case (int) R_PPC_EMB_NADDR16_HI:
	  addend -= 2 * relocation;
	  break;

	case (int) R_PPC_EMB_NADDR16_HA:
	  addend -= 2 * relocation;
	  addend += ((relocation + addend) & 0x8000) << 1;
	  break;

	/* NOP relocation that prevents garbage collecting linkers from omitting a
	   reference.  */
	case (int) R_PPC_EMB_MRKREF:
	  continue;

	case (int) R_PPC_COPY:
	case (int) R_PPC_GLOB_DAT:
	case (int) R_PPC_JMP_SLOT:
	case (int) R_PPC_RELATIVE:
	case (int) R_PPC_PLT32:
	case (int) R_PPC_PLTREL32:
	case (int) R_PPC_PLT16_LO:
	case (int) R_PPC_PLT16_HI:
	case (int) R_PPC_PLT16_HA:
	case (int) R_PPC_EMB_RELSEC16:
	case (int) R_PPC_EMB_RELST_LO:
	case (int) R_PPC_EMB_RELST_HI:
	case (int) R_PPC_EMB_RELST_HA:
	case (int) R_PPC_EMB_BIT_FLD:
	  (*_bfd_error_handler) (_("%s: Relocation %s is not yet supported for symbol %s."),
				 bfd_archive_filename (input_bfd),
				 ppc_elf_howto_table[(int) r_type]->name,
				 sym_name);

	  bfd_set_error (bfd_error_invalid_operation);
	  ret = false;
	  continue;

	case (int) R_PPC_GNU_VTINHERIT:
	case (int) R_PPC_GNU_VTENTRY:
	  /* These are no-ops in the end.  */
	  continue;
	}

#ifdef DEBUG
      fprintf (stderr, "\ttype = %s (%d), name = %s, symbol index = %ld, offset = %ld, addend = %ld\n",
	       howto->name,
	       (int) r_type,
	       sym_name,
	       r_symndx,
	       (long) offset,
	       (long) addend);
#endif
      if (copy) 
	{
	  Elf_Internal_Rela outrel;

	  if (sec == NULL) /* Don't know if it is possible... */ 
	    abort(); 

	  /*printf("copying reloc %d, addend=%x, rel=%x, indx=%d, offset=%x, sec_vma=%x\n",
		 r_type,addend,relocation,sec->output_section->target_index,
		 sec->output_offset,sec->output_section->vma);*/

	  outrel.r_info = ELF32_R_INFO(sec->output_section->target_index, r_type);
	  outrel.r_addend = relocation + addend - sec->output_section->vma;
	  outrel.r_offset = input_section->output_offset + offset;

 	  bfd_elf32_swap_reloca_out (output_bfd, &outrel,
				     (((Elf32_External_Rela *)
				       elf_section_data(input_section->output_section)->
				       rel_hdr.contents)
				      + input_section->output_section->reloc_count));
	  ++input_section->output_section->reloc_count;
	}
      else
	{
	  /*printf("applying reloc %d, sym=%s addend=%x, rel=%x, indx=%d, offset=%x, sec_vma=%x\n",
		 r_type,sym_name,addend,relocation,sec->output_section->target_index,
		 sec->output_offset,sec->output_section->vma);*/

      r = _bfd_final_link_relocate (howto,
				    input_bfd,
				    input_section,
				    contents,
				    offset,
				    relocation,
				    addend);

      if (r == bfd_reloc_ok)
	;
      else if (r == bfd_reloc_overflow)
	{
	  const char *name;

	  if (h != NULL)
	    {
	      if (h->root.type == bfd_link_hash_undefweak
		  && howto->pc_relative)
		{
		  /* Assume this is a call protected by other code that
		     detect the symbol is undefined.  If this is the case,
		     we can safely ignore the overflow.  If not, the
		     program is hosed anyway, and a little warning isn't
		     going to help.  */

		  continue;
		}

	      name = h->root.root.string;
	    }
	  else
	    {
	      name = bfd_elf_string_from_elf_section (input_bfd,
						      symtab_hdr->sh_link,
						      sym->st_name);
	      if (name == NULL)
		continue;
	      if (*name == '\0')
		name = bfd_section_name (input_bfd, sec);
	    }

	  if (! (*info->callbacks->reloc_overflow) (info,
						   name,
						   howto->name,
						   (bfd_vma) 0,
						   input_bfd,
						   input_section,
						   offset))
	    return false;
	}
      else
	ret = false;
    }
    }

#ifdef DEBUG
  fprintf (stderr, "\n");
#endif

  return ret;
}

static enum elf_reloc_type_class
ppc_elf_reloc_type_class (rela)
     const Elf_Internal_Rela *rela;
{
  switch ((int) ELF32_R_TYPE (rela->r_info))
    {
    case R_PPC_RELATIVE:
      return reloc_class_relative;
    case R_PPC_REL24:
    case R_PPC_ADDR24:
    case R_PPC_JMP_SLOT:
      return reloc_class_plt;
    case R_PPC_COPY:
      return reloc_class_copy;
    default:
      return reloc_class_normal;
    }
}

/* Support for core dump NOTE sections */
static boolean
ppc_elf_grok_prstatus (abfd, note)
     bfd *abfd;
     Elf_Internal_Note *note;
{
  int offset;
  unsigned int raw_size;

  switch (note->descsz)
    {
      default:
	return false;

      case 268:		/* Linux/PPC */
	/* pr_cursig */
	elf_tdata (abfd)->core_signal = bfd_get_16 (abfd, note->descdata + 12);

	/* pr_pid */
	elf_tdata (abfd)->core_pid = bfd_get_32 (abfd, note->descdata + 24);

	/* pr_reg */
	offset = 72;
	raw_size = 192;

	break;
    }

  /* Make a ".reg/999" section.  */
  return _bfd_elfcore_make_pseudosection (abfd, ".reg",
					  raw_size, note->descpos + offset);
}

static boolean
ppc_elf_grok_psinfo (abfd, note)
     bfd *abfd;
     Elf_Internal_Note *note;
{
  switch (note->descsz)
    {
      default:
	return false;

      case 128:		/* Linux/PPC elf_prpsinfo */
	elf_tdata (abfd)->core_program
	 = _bfd_elfcore_strndup (abfd, note->descdata + 32, 16);
	elf_tdata (abfd)->core_command
	 = _bfd_elfcore_strndup (abfd, note->descdata + 48, 80);
    }

  /* Note that for some reason, a spurious space is tacked
     onto the end of the args in some (at least one anyway)
     implementations, so strip it off if it exists.  */

  {
    char *command = elf_tdata (abfd)->core_command;
    int n = strlen (command);

    if (0 < n && command[n - 1] == ' ')
      command[n - 1] = '\0';
  }

  return true;
}

/* Special MorphOS final link routine.  */
/* This is almost the same as the elf one, except for the hanling of relocations */

/* A structure we use to avoid passing large numbers of arguments.  */

struct elf_final_link_info
{
  /* General link information.  */
  struct bfd_link_info *info;
  /* Output BFD.  */
  bfd *output_bfd;
  /* Symbol string table.  */
  struct bfd_strtab_hash *symstrtab;
  /* .dynsym section.  */
  asection *dynsym_sec;
  /* .hash section.  */
  asection *hash_sec;
  /* symbol version section (.gnu.version).  */
  asection *symver_sec;
  /* first SHF_TLS section (if any).  */
  asection *first_tls_sec;
  /* Buffer large enough to hold contents of any section.  */
  bfd_byte *contents;
  /* Buffer large enough to hold external relocs of any section.  */
  PTR external_relocs;
  /* Buffer large enough to hold internal relocs of any section.  */
  Elf_Internal_Rela *internal_relocs;
  /* Buffer large enough to hold external local symbols of any input
     BFD.  */
  Elf_External_Sym *external_syms;
  /* And a buffer for symbol section indices.  */
  Elf_External_Sym_Shndx *locsym_shndx;
  /* Buffer large enough to hold internal local symbols of any input
     BFD.  */
  Elf_Internal_Sym *internal_syms;
  /* Array large enough to hold a symbol index for each local symbol
     of any input BFD.  */
  long *indices;
  /* Array large enough to hold a section pointer for each local
     symbol of any input BFD.  */
  asection **sections;
  /* Buffer to hold swapped out symbols.  */
  Elf_External_Sym *symbuf;
  /* And one for symbol section indices.  */
  Elf_External_Sym_Shndx *symshndxbuf;
  /* Number of swapped out symbols in buffer.  */
  size_t symbuf_count;
  /* Number of symbols which fit in symbuf.  */
  size_t symbuf_size;
};

static boolean elf_link_output_sym
  PARAMS ((struct elf_final_link_info *, const char *,
	   Elf_Internal_Sym *, asection *));
static boolean elf_link_flush_output_syms
  PARAMS ((struct elf_final_link_info *));
static boolean elf_link_output_extsym
  PARAMS ((struct elf_link_hash_entry *, PTR));
static boolean elf_link_sec_merge_syms
  PARAMS ((struct elf_link_hash_entry *, PTR));
static boolean elf_link_check_versioned_symbol
  PARAMS ((struct bfd_link_info *, struct elf_link_hash_entry *));
static boolean elf_link_input_bfd
  PARAMS ((struct elf_final_link_info *, bfd *));
static boolean elf_reloc_link_order
  PARAMS ((bfd *, struct bfd_link_info *, asection *,
	   struct bfd_link_order *));

static boolean elf_section_ignore_discarded_relocs
  PARAMS ((asection *));

/* This struct is used to pass information to elf_link_output_extsym.  */

struct elf_outext_info
{
  boolean failed;
  boolean localsyms;
  struct elf_final_link_info *finfo;
};

/* Compute the size of, and allocate space for, REL_HDR which is the
   section header for a section containing relocations for O.  */

static boolean
elf_link_size_reloc_section (abfd, rel_hdr, o)
     bfd *abfd;
     Elf_Internal_Shdr *rel_hdr;
     asection *o;
{
  bfd_size_type reloc_count;
  bfd_size_type num_rel_hashes;

  /* Figure out how many relocations there will be.  */
  if (rel_hdr == &elf_section_data (o)->rel_hdr)
    reloc_count = elf_section_data (o)->rel_count;
  else
    reloc_count = elf_section_data (o)->rel_count2;

  num_rel_hashes = o->reloc_count;
  if (num_rel_hashes < reloc_count)
    num_rel_hashes = reloc_count;

  /* That allows us to calculate the size of the section.  */
  rel_hdr->sh_size = rel_hdr->sh_entsize * reloc_count;

  /* The contents field must last into write_object_contents, so we
     allocate it with bfd_alloc rather than malloc.  Also since we
     cannot be sure that the contents will actually be filled in,
     we zero the allocated space.  */
  rel_hdr->contents = (PTR) bfd_zalloc (abfd, rel_hdr->sh_size);
  if (rel_hdr->contents == NULL && rel_hdr->sh_size != 0)
    return false;

  /* We only allocate one set of hash entries, so we only do it the
     first time we are called.  */
  if (elf_section_data (o)->rel_hashes == NULL
      && num_rel_hashes)
    {
      struct elf_link_hash_entry **p;

      p = ((struct elf_link_hash_entry **)
	   bfd_zmalloc (num_rel_hashes
			* sizeof (struct elf_link_hash_entry *)));
      if (p == NULL)
	return false;

      elf_section_data (o)->rel_hashes = p;
    }

  return true;
}

/* When performing a relocateable link, the input relocations are
   preserved.  But, if they reference global symbols, the indices
   referenced must be updated.  Update all the relocations in
   REL_HDR (there are COUNT of them), using the data in REL_HASH.  */

static void
elf_link_adjust_relocs (abfd, rel_hdr, count, rel_hash)
     bfd *abfd;
     Elf_Internal_Shdr *rel_hdr;
     unsigned int count;
     struct elf_link_hash_entry **rel_hash;
{
  unsigned int i;
  struct elf_backend_data *bed = get_elf_backend_data (abfd);
  Elf_Internal_Rel *irel;
  Elf_Internal_Rela *irela;
  bfd_size_type amt = sizeof (Elf_Internal_Rel) * bed->s->int_rels_per_ext_rel;

  irel = (Elf_Internal_Rel *) bfd_zmalloc (amt);
  if (irel == NULL)
    {
      (*_bfd_error_handler) (_("Error: out of memory"));
      abort ();
    }

  amt = sizeof (Elf_Internal_Rela) * bed->s->int_rels_per_ext_rel;
  irela = (Elf_Internal_Rela *) bfd_zmalloc (amt);
  if (irela == NULL)
    {
      (*_bfd_error_handler) (_("Error: out of memory"));
      abort ();
    }

  for (i = 0; i < count; i++, rel_hash++)
    {
      if (*rel_hash == NULL)
	continue;

      BFD_ASSERT ((*rel_hash)->indx >= 0);

      if (rel_hdr->sh_entsize == sizeof (Elf_External_Rel))
	{
	  Elf_External_Rel *erel;
	  unsigned int j;

	  erel = (Elf_External_Rel *) rel_hdr->contents + i;
	  if (bed->s->swap_reloc_in)
	    (*bed->s->swap_reloc_in) (abfd, (bfd_byte *) erel, irel);
	  else
	    elf_swap_reloc_in (abfd, erel, irel);

	  for (j = 0; j < bed->s->int_rels_per_ext_rel; j++)
	    irel[j].r_info = ELF_R_INFO ((*rel_hash)->indx,
					 ELF_R_TYPE (irel[j].r_info));

	  if (bed->s->swap_reloc_out)
	    (*bed->s->swap_reloc_out) (abfd, irel, (bfd_byte *) erel);
	  else
	    elf_swap_reloc_out (abfd, irel, erel);
	}
      else
	{
	  Elf_External_Rela *erela;
	  unsigned int j;

	  BFD_ASSERT (rel_hdr->sh_entsize
		      == sizeof (Elf_External_Rela));

	  erela = (Elf_External_Rela *) rel_hdr->contents + i;
	  if (bed->s->swap_reloca_in)
	    (*bed->s->swap_reloca_in) (abfd, (bfd_byte *) erela, irela);
	  else
	    elf_swap_reloca_in (abfd, erela, irela);

	  for (j = 0; j < bed->s->int_rels_per_ext_rel; j++)
	    irela[j].r_info = ELF_R_INFO ((*rel_hash)->indx,
					  ELF_R_TYPE (irela[j].r_info));

	  if (bed->s->swap_reloca_out)
	    (*bed->s->swap_reloca_out) (abfd, irela, (bfd_byte *) erela);
	  else
	    elf_swap_reloca_out (abfd, irela, erela);
	}
    }

  free (irel);
  free (irela);
}

struct elf_link_sort_rela
{
  bfd_vma offset;
  enum elf_reloc_type_class type;
  union
  {
    Elf_Internal_Rel rel;
    Elf_Internal_Rela rela;
  } u;
};

static int
elf_link_sort_cmp1 (A, B)
     const PTR A;
     const PTR B;
{
  struct elf_link_sort_rela *a = (struct elf_link_sort_rela *) A;
  struct elf_link_sort_rela *b = (struct elf_link_sort_rela *) B;
  int relativea, relativeb;

  relativea = a->type == reloc_class_relative;
  relativeb = b->type == reloc_class_relative;

  if (relativea < relativeb)
    return 1;
  if (relativea > relativeb)
    return -1;
  if (ELF_R_SYM (a->u.rel.r_info) < ELF_R_SYM (b->u.rel.r_info))
    return -1;
  if (ELF_R_SYM (a->u.rel.r_info) > ELF_R_SYM (b->u.rel.r_info))
    return 1;
  if (a->u.rel.r_offset < b->u.rel.r_offset)
    return -1;
  if (a->u.rel.r_offset > b->u.rel.r_offset)
    return 1;
  return 0;
}

static int
elf_link_sort_cmp2 (A, B)
     const PTR A;
     const PTR B;
{
  struct elf_link_sort_rela *a = (struct elf_link_sort_rela *) A;
  struct elf_link_sort_rela *b = (struct elf_link_sort_rela *) B;
  int copya, copyb;

  if (a->offset < b->offset)
    return -1;
  if (a->offset > b->offset)
    return 1;
  copya = (a->type == reloc_class_copy) * 2 + (a->type == reloc_class_plt);
  copyb = (b->type == reloc_class_copy) * 2 + (b->type == reloc_class_plt);
  if (copya < copyb)
    return -1;
  if (copya > copyb)
    return 1;
  if (a->u.rel.r_offset < b->u.rel.r_offset)
    return -1;
  if (a->u.rel.r_offset > b->u.rel.r_offset)
    return 1;
  return 0;
}

static size_t
elf_link_sort_relocs (abfd, info, psec)
     bfd *abfd;
     struct bfd_link_info *info;
     asection **psec;
{
  bfd *dynobj = elf_hash_table (info)->dynobj;
  asection *reldyn, *o;
  boolean rel = false;
  bfd_size_type count, size;
  size_t i, j, ret;
  struct elf_link_sort_rela *rela;
  struct elf_backend_data *bed = get_elf_backend_data (abfd);

  reldyn = bfd_get_section_by_name (abfd, ".rela.dyn");
  if (reldyn == NULL || reldyn->_raw_size == 0)
    {
      reldyn = bfd_get_section_by_name (abfd, ".rel.dyn");
      if (reldyn == NULL || reldyn->_raw_size == 0)
	return 0;
      rel = true;
      count = reldyn->_raw_size / sizeof (Elf_External_Rel);
    }
  else
    count = reldyn->_raw_size / sizeof (Elf_External_Rela);

  size = 0;
  for (o = dynobj->sections; o != NULL; o = o->next)
    if ((o->flags & (SEC_HAS_CONTENTS|SEC_LINKER_CREATED))
	== (SEC_HAS_CONTENTS|SEC_LINKER_CREATED)
	&& o->output_section == reldyn)
      size += o->_raw_size;

  if (size != reldyn->_raw_size)
    return 0;

  rela = (struct elf_link_sort_rela *) bfd_zmalloc (sizeof (*rela) * count);
  if (rela == NULL)
    {
      (*info->callbacks->warning)
	(info, _("Not enough memory to sort relocations"), 0, abfd, 0,
	 (bfd_vma) 0);
      return 0;
    }

  for (o = dynobj->sections; o != NULL; o = o->next)
    if ((o->flags & (SEC_HAS_CONTENTS|SEC_LINKER_CREATED))
	== (SEC_HAS_CONTENTS|SEC_LINKER_CREATED)
	&& o->output_section == reldyn)
      {
	if (rel)
	  {
	    Elf_External_Rel *erel, *erelend;
	    struct elf_link_sort_rela *s;

	    erel = (Elf_External_Rel *) o->contents;
	    erelend = (Elf_External_Rel *) (o->contents + o->_raw_size);
	    s = rela + o->output_offset / sizeof (Elf_External_Rel);
	    for (; erel < erelend; erel++, s++)
	      {
		if (bed->s->swap_reloc_in)
		  (*bed->s->swap_reloc_in) (abfd, (bfd_byte *) erel, &s->u.rel);
		else
		  elf_swap_reloc_in (abfd, erel, &s->u.rel);

		s->type = (*bed->elf_backend_reloc_type_class) (&s->u.rela);
	      }
	  }
	else
	  {
	    Elf_External_Rela *erela, *erelaend;
	    struct elf_link_sort_rela *s;

	    erela = (Elf_External_Rela *) o->contents;
	    erelaend = (Elf_External_Rela *) (o->contents + o->_raw_size);
	    s = rela + o->output_offset / sizeof (Elf_External_Rela);
	    for (; erela < erelaend; erela++, s++)
	      {
		if (bed->s->swap_reloca_in)
		  (*bed->s->swap_reloca_in) (dynobj, (bfd_byte *) erela,
					     &s->u.rela);
		else
		  elf_swap_reloca_in (dynobj, erela, &s->u.rela);

		s->type = (*bed->elf_backend_reloc_type_class) (&s->u.rela);
	      }
	  }
      }

  qsort (rela, (size_t) count, sizeof (*rela), elf_link_sort_cmp1);
  for (ret = 0; ret < count && rela[ret].type == reloc_class_relative; ret++)
    ;
  for (i = ret, j = ret; i < count; i++)
    {
      if (ELF_R_SYM (rela[i].u.rel.r_info) != ELF_R_SYM (rela[j].u.rel.r_info))
	j = i;
      rela[i].offset = rela[j].u.rel.r_offset;
    }
  qsort (rela + ret, (size_t) count - ret, sizeof (*rela), elf_link_sort_cmp2);

  for (o = dynobj->sections; o != NULL; o = o->next)
    if ((o->flags & (SEC_HAS_CONTENTS|SEC_LINKER_CREATED))
	== (SEC_HAS_CONTENTS|SEC_LINKER_CREATED)
	&& o->output_section == reldyn)
      {
	if (rel)
	  {
	    Elf_External_Rel *erel, *erelend;
	    struct elf_link_sort_rela *s;

	    erel = (Elf_External_Rel *) o->contents;
	    erelend = (Elf_External_Rel *) (o->contents + o->_raw_size);
	    s = rela + o->output_offset / sizeof (Elf_External_Rel);
	    for (; erel < erelend; erel++, s++)
	      {
		if (bed->s->swap_reloc_out)
		  (*bed->s->swap_reloc_out) (abfd, &s->u.rel,
					     (bfd_byte *) erel);
		else
		  elf_swap_reloc_out (abfd, &s->u.rel, erel);
	      }
	  }
	else
	  {
	    Elf_External_Rela *erela, *erelaend;
	    struct elf_link_sort_rela *s;

	    erela = (Elf_External_Rela *) o->contents;
	    erelaend = (Elf_External_Rela *) (o->contents + o->_raw_size);
	    s = rela + o->output_offset / sizeof (Elf_External_Rela);
	    for (; erela < erelaend; erela++, s++)
	      {
		if (bed->s->swap_reloca_out)
		  (*bed->s->swap_reloca_out) (dynobj, &s->u.rela,
					      (bfd_byte *) erela);
		else
		  elf_swap_reloca_out (dynobj, &s->u.rela, erela);
	      }
	  }
      }

  free (rela);
  *psec = reldyn;
  return ret;
}

/* Do the final step of an ELF link.  */

boolean
ppc_elf_final_link (abfd, info)
     bfd *abfd;
     struct bfd_link_info *info;
{
  boolean dynamic;
  boolean emit_relocs;
  bfd *dynobj;
  struct elf_final_link_info finfo;
  register asection *o;
  register struct bfd_link_order *p;
  register bfd *sub;
  bfd_size_type max_contents_size;
  bfd_size_type max_external_reloc_size;
  bfd_size_type max_internal_reloc_count;
  bfd_size_type max_sym_count;
  bfd_size_type max_sym_shndx_count;
  bfd_size_type max_datadata_reloc_count;
  file_ptr off;
  Elf_Internal_Sym elfsym;
  unsigned int i;
  Elf_Internal_Shdr *symtab_hdr;
  Elf_Internal_Shdr *symstrtab_hdr;
  struct elf_backend_data *bed = get_elf_backend_data (abfd);
  struct elf_outext_info eoinfo;
  boolean merged;
  size_t relativecount = 0;
  asection *reldyn = 0;
  bfd_size_type amt;
  asection *ddr_sec;
  asection *sdata_sec = NULL;
  asection *sbss_sec = NULL;

  if (! is_elf_hash_table (info))
    return false;

  if (info->shared)
    abfd->flags |= DYNAMIC;

  bfd_set_start_address(abfd, 0);

  dynamic = elf_hash_table (info)->dynamic_sections_created;
  dynobj = elf_hash_table (info)->dynobj;

  emit_relocs = (info->relocateable
		 || info->emitrelocations
		 || bed->elf_backend_emit_relocs);

  finfo.info = info;
  finfo.output_bfd = abfd;
  finfo.symstrtab = elf_stringtab_init ();
  if (finfo.symstrtab == NULL)
    return false;

  if (! dynamic)
    {
      finfo.dynsym_sec = NULL;
      finfo.hash_sec = NULL;
      finfo.symver_sec = NULL;
    }
  else
    {
      finfo.dynsym_sec = bfd_get_section_by_name (dynobj, ".dynsym");
      finfo.hash_sec = bfd_get_section_by_name (dynobj, ".hash");
      BFD_ASSERT (finfo.dynsym_sec != NULL && finfo.hash_sec != NULL);
      finfo.symver_sec = bfd_get_section_by_name (dynobj, ".gnu.version");
      /* Note that it is OK if symver_sec is NULL.  */
    }

  finfo.contents = NULL;
  finfo.external_relocs = NULL;
  finfo.internal_relocs = NULL;
  finfo.external_syms = NULL;
  finfo.locsym_shndx = NULL;
  finfo.internal_syms = NULL;
  finfo.indices = NULL;
  finfo.sections = NULL;
  finfo.symbuf = NULL;
  finfo.symshndxbuf = NULL;
  finfo.symbuf_count = 0;
  finfo.first_tls_sec = NULL;
  for (o = abfd->sections; o != (asection *) NULL; o = o->next)
    if ((o->flags & SEC_THREAD_LOCAL) != 0
	&& (o->flags & SEC_LOAD) != 0)
      {
	finfo.first_tls_sec = o;
	break;
      }

  ddr_sec = bfd_get_section_by_name(abfd, "ddrelocs");
  
  /* Count up the number of relocations we will output for each output
     section, so that we know the sizes of the reloc sections.  We
     also figure out some maximum sizes.  */
  max_contents_size = 0;
  max_external_reloc_size = 0;
  max_internal_reloc_count = 0;
  max_sym_count = 0;
  max_sym_shndx_count = 0;
  max_datadata_reloc_count = 0;
  merged = false;
  for (o = abfd->sections; o != (asection *) NULL; o = o->next)
    {
      o->reloc_count = 0;

      for (p = o->link_order_head; p != NULL; p = p->next)
	{
	  if (p->type == bfd_section_reloc_link_order
	      || p->type == bfd_symbol_reloc_link_order)
	    ++o->reloc_count;
	  else if (p->type == bfd_indirect_link_order)
	    {
	      asection *sec;

	      sec = p->u.indirect.section;

	      /* Mark all sections which are to be included in the
		 link.  This will normally be every section.  We need
		 to do this so that we can identify any sections which
		 the linker has decided to not include.  */
	      sec->linker_mark = true;

	      if (sec->flags & SEC_MERGE)
		merged = true;

	      /* Maximum number of relocations */
	      if (1 || info->relocateable || info->emitrelocations)
		o->reloc_count += sec->reloc_count;
	      else if (bed->elf_backend_count_relocs)
		{
		  Elf_Internal_Rela * relocs;

		  relocs = (NAME(_bfd_elf,link_read_relocs)
			    (abfd, sec, (PTR) NULL,
			     (Elf_Internal_Rela *) NULL, info->keep_memory));

		  o->reloc_count
		    += (*bed->elf_backend_count_relocs) (sec, relocs);

		  if (elf_section_data (o)->relocs != relocs)
		    free (relocs);
		}

	      if (sec->_raw_size > max_contents_size)
		max_contents_size = sec->_raw_size;
	      if (sec->_cooked_size > max_contents_size)
		max_contents_size = sec->_cooked_size;

	      /* We are interested in just local symbols, not all
		 symbols.  */
	      if (bfd_get_flavour (sec->owner) == bfd_target_elf_flavour
		  && (sec->owner->flags & DYNAMIC) == 0)
		{
		  size_t sym_count;

		  if (elf_bad_symtab (sec->owner))
		    sym_count = (elf_tdata (sec->owner)->symtab_hdr.sh_size
				 / sizeof (Elf_External_Sym));
		  else
		    sym_count = elf_tdata (sec->owner)->symtab_hdr.sh_info;

		  if (sym_count > max_sym_count)
		    max_sym_count = sym_count;

		  if (sym_count > max_sym_shndx_count
		      && elf_symtab_shndx (sec->owner) != 0)
		    max_sym_shndx_count = sym_count;

		  if ((sec->flags & SEC_RELOC) != 0)
		    {
		      size_t ext_size;

		      ext_size = elf_section_data (sec)->rel_hdr.sh_size;
		      if (ext_size > max_external_reloc_size)
			max_external_reloc_size = ext_size;
		      if (sec->reloc_count > max_internal_reloc_count)
			max_internal_reloc_count = sec->reloc_count;
		    }
		}
	    }
	}

      if (!strcmp(bfd_section_name(abfd, o), ".sdata"))
	sdata_sec = o;
      else if(!strcmp(bfd_section_name(abfd, o), ".sbss"))
	sbss_sec = o;
      else
	bfd_set_section_vma(abfd, o, 0);

      if (o->reloc_count > 0)
        {
	  o->flags |= SEC_RELOC;
	  if (o == sdata_sec || o == sbss_sec)
	    max_datadata_reloc_count += o->reloc_count;
	}
      else
	{
	  /* Explicitly clear the SEC_RELOC flag.  The linker tends to
	     set it (this is probably a bug) and if it is set
	     assign_section_numbers will create a reloc section.  */
	  o->flags &=~ SEC_RELOC;
	}

      /* If the SEC_ALLOC flag is not set, force the section VMA to
	 zero.  This is done in elf_fake_sections as well, but forcing
	 the VMA to 0 here will ensure that relocs against these
	 sections are handled correctly.  */
      if ((o->flags & SEC_ALLOC) == 0
	  && ! o->user_set_vma)
	o->vma = 0;
    }

  if (sdata_sec)
    {
      if (sbss_sec)
	bfd_set_section_vma(abfd, sbss_sec, sbss_sec->vma - sdata_sec->vma);
      bfd_set_section_vma(abfd, sdata_sec, 0);
    }
  
  if (! info->relocateable && merged)
    elf_link_hash_traverse (elf_hash_table (info),
			    elf_link_sec_merge_syms, (PTR) abfd);

  /* Figure out the file positions for everything but the symbol table
     and the relocs.  We set symcount to force assign_section_numbers
     to create a symbol table.  */
  bfd_get_symcount (abfd) = 1;
  BFD_ASSERT (! abfd->output_has_begun);
  if (! _bfd_elf_compute_section_file_positions (abfd, info))
    goto error_return;

  /* Figure out how many relocations we will have in each section.
     Just using RELOC_COUNT isn't good enough since that doesn't
     maintain a separate value for REL vs. RELA relocations.  */
  if (emit_relocs)
    for (sub = info->input_bfds; sub != NULL; sub = sub->link_next)
      for (o = sub->sections; o != NULL; o = o->next)
	{
	  asection *output_section;

	  if (! o->linker_mark)
	    {
	      /* This section was omitted from the link.  */
	      continue;
	    }

	  output_section = o->output_section;

	  if (output_section != NULL
	      && (o->flags & SEC_RELOC) != 0)
	    {
	      struct bfd_elf_section_data *esdi
		= elf_section_data (o);
	      struct bfd_elf_section_data *esdo
		= elf_section_data (output_section);
	      unsigned int *rel_count;
	      unsigned int *rel_count2;
	      bfd_size_type entsize;
	      bfd_size_type entsize2;

	      /* We must be careful to add the relocations from the
		 input section to the right output count.  */
	      entsize = esdi->rel_hdr.sh_entsize;
	      entsize2 = esdi->rel_hdr2 ? esdi->rel_hdr2->sh_entsize : 0;
	      BFD_ASSERT ((entsize == sizeof (Elf_External_Rel)
			   || entsize == sizeof (Elf_External_Rela))
			  && entsize2 != entsize
			  && (entsize2 == 0
			      || entsize2 == sizeof (Elf_External_Rel)
			      || entsize2 == sizeof (Elf_External_Rela)));
	      if (entsize == esdo->rel_hdr.sh_entsize)
		{
		  rel_count = &esdo->rel_count;
		  rel_count2 = &esdo->rel_count2;
		}
	      else
		{
		  rel_count = &esdo->rel_count2;
		  rel_count2 = &esdo->rel_count;
		}

	      *rel_count += NUM_SHDR_ENTRIES (& esdi->rel_hdr);
	      if (esdi->rel_hdr2)
		*rel_count2 += NUM_SHDR_ENTRIES (esdi->rel_hdr2);
	      output_section->flags |= SEC_RELOC;
	    }
	}

  /* That created the reloc sections.  Set their sizes, and assign
     them file positions, and allocate some buffers.  */
  for (o = abfd->sections; o != NULL; o = o->next)
    {
      if ((o->flags & SEC_RELOC) != 0)
	{
	  if (!elf_link_size_reloc_section (abfd,
					    &elf_section_data (o)->rel_hdr,
					    o))
	    goto error_return;

	  if (elf_section_data (o)->rel_hdr2
	      && !elf_link_size_reloc_section (abfd,
					       elf_section_data (o)->rel_hdr2,
					       o))
	    goto error_return;
	}

      /* Now, reset REL_COUNT and REL_COUNT2 so that we can use them
	 to count upwards while actually outputting the relocations.  */
      elf_section_data (o)->rel_count = 0;
      elf_section_data (o)->rel_count2 = 0;
    }

  /* We have now assigned file positions for all the sections except
     relocations, .symtab, and .strtab.  We start the .symtab section
     at the current file position, and write directly to it.  We build
     the .strtab section in memory.  */
  bfd_get_symcount (abfd) = 0;
  symtab_hdr = &elf_tdata (abfd)->symtab_hdr;
  /* sh_name is set in prep_headers.  */
  symtab_hdr->sh_type = SHT_SYMTAB;
  symtab_hdr->sh_flags = 0;
  symtab_hdr->sh_addr = 0;
  symtab_hdr->sh_size = 0;
  symtab_hdr->sh_entsize = sizeof (Elf_External_Sym);
  /* sh_link is set in assign_section_numbers.  */
  /* sh_info is set below.  */
  /* sh_offset is set just below.  */
  symtab_hdr->sh_addralign = bed->s->file_align;

  off = elf_tdata (abfd)->next_file_pos;
  off = _bfd_elf_assign_file_position_for_section (symtab_hdr, off, true);

  /* Note that at this point elf_tdata (abfd)->next_file_pos is
     incorrect.  We do not yet know the size of the .symtab section.
     We correct next_file_pos below, after we do know the size.  */

  /* Allocate a buffer to hold swapped out symbols.  This is to avoid
     continuously seeking to the right position in the file.  */
  if (! info->keep_memory || max_sym_count < 20)
    finfo.symbuf_size = 20;
  else
    finfo.symbuf_size = max_sym_count;
  amt = finfo.symbuf_size;
  amt *= sizeof (Elf_External_Sym);
  finfo.symbuf = (Elf_External_Sym *) bfd_malloc (amt);
  if (finfo.symbuf == NULL)
    goto error_return;
  if (elf_numsections (abfd) > SHN_LORESERVE)
    {
      amt = finfo.symbuf_size;
      amt *= sizeof (Elf_External_Sym_Shndx);
      finfo.symshndxbuf = (Elf_External_Sym_Shndx *) bfd_malloc (amt);
      if (finfo.symshndxbuf == NULL)
	goto error_return;
    }

  /* Start writing out the symbol table.  The first symbol is always a
     dummy symbol.  */
  elfsym.st_value = 0;
  elfsym.st_size = 0;
  elfsym.st_info = 0;
  elfsym.st_other = 0;
  elfsym.st_shndx = SHN_UNDEF;
  if (! elf_link_output_sym (&finfo, (const char *) NULL,
			     &elfsym, bfd_und_section_ptr))
    goto error_return;

#if 0
  /* Some standard ELF linkers do this, but we don't because it causes
     bootstrap comparison failures.  */
  /* Output a file symbol for the output file as the second symbol.
     We output this even if we are discarding local symbols, although
     I'm not sure if this is correct.  */
  elfsym.st_value = 0;
  elfsym.st_size = 0;
  elfsym.st_info = ELF_ST_INFO (STB_LOCAL, STT_FILE);
  elfsym.st_other = 0;
  elfsym.st_shndx = SHN_ABS;
  if (! elf_link_output_sym (&finfo, bfd_get_filename (abfd),
			     &elfsym, bfd_abs_section_ptr))
    goto error_return;
#endif

  /* Output a symbol for each section.  We output these even if we are
     discarding local symbols, since they are used for relocs.  These
     symbols have no names.  We store the index of each one in the
     index field of the section, so that we can find it again when
     outputting relocs.  */
  elfsym.st_size = 0;
  elfsym.st_info = ELF_ST_INFO (STB_LOCAL, STT_SECTION);
  elfsym.st_other = 0;
  for (i = 1; i < elf_numsections (abfd); i++)
    {
      o = section_from_elf_index (abfd, i);
      if (o != NULL)
	o->target_index = bfd_get_symcount (abfd);
      elfsym.st_shndx = i;
      elfsym.st_value = 0;
      if (! elf_link_output_sym (&finfo, (const char *) NULL,
				 &elfsym, o))
	goto error_return;
      if (i == SHN_LORESERVE)
	i += SHN_HIRESERVE + 1 - SHN_LORESERVE;
    }

  /* Allocate some memory to hold information read in from the input
     files.  */
  if (max_contents_size != 0)
    {
      finfo.contents = (bfd_byte *) bfd_malloc (max_contents_size);
      if (finfo.contents == NULL)
	goto error_return;
    }

  if (max_external_reloc_size != 0)
    {
      finfo.external_relocs = (PTR) bfd_malloc (max_external_reloc_size);
      if (finfo.external_relocs == NULL)
	goto error_return;
    }

  if (max_internal_reloc_count != 0)
    {
      amt = max_internal_reloc_count * bed->s->int_rels_per_ext_rel;
      amt *= sizeof (Elf_Internal_Rela);
      finfo.internal_relocs = (Elf_Internal_Rela *) bfd_malloc (amt);
      if (finfo.internal_relocs == NULL)
	goto error_return;
    }

  if (max_sym_count != 0)
    {
      amt = max_sym_count * sizeof (Elf_External_Sym);
      finfo.external_syms = (Elf_External_Sym *) bfd_malloc (amt);
      if (finfo.external_syms == NULL)
	goto error_return;

      amt = max_sym_count * sizeof (Elf_Internal_Sym);
      finfo.internal_syms = (Elf_Internal_Sym *) bfd_malloc (amt);
      if (finfo.internal_syms == NULL)
	goto error_return;

      amt = max_sym_count * sizeof (long);
      finfo.indices = (long *) bfd_malloc (amt);
      if (finfo.indices == NULL)
	goto error_return;

      amt = max_sym_count * sizeof (asection *);
      finfo.sections = (asection **) bfd_malloc (amt);
      if (finfo.sections == NULL)
	goto error_return;
    }

  if (max_sym_shndx_count != 0)
    {
      amt = max_sym_shndx_count * sizeof (Elf_External_Sym_Shndx);
      finfo.locsym_shndx = (Elf_External_Sym_Shndx *) bfd_malloc (amt);
      if (finfo.locsym_shndx == NULL)
	goto error_return;
    }

  if (finfo.first_tls_sec)
    {
      unsigned int align = 0;
      bfd_vma base = finfo.first_tls_sec->vma, end = 0;
      asection *sec;

      for (sec = finfo.first_tls_sec;
	   sec && (sec->flags & SEC_THREAD_LOCAL);
	   sec = sec->next)
	{
	  bfd_vma size = sec->_raw_size;

	  if (bfd_get_section_alignment (abfd, sec) > align)
	    align = bfd_get_section_alignment (abfd, sec);
	  if (sec->_raw_size == 0 && (sec->flags & SEC_HAS_CONTENTS) == 0)
	    {
	      struct bfd_link_order *o;

	      size = 0;
	      for (o = sec->link_order_head; o != NULL; o = o->next)
		if (size < o->offset + o->size)
		  size = o->offset + o->size;
	    }
	  end = sec->vma + size;
	}
      elf_hash_table (info)->tls_segment
	= bfd_zalloc (abfd, sizeof (struct elf_link_tls_segment));
      if (elf_hash_table (info)->tls_segment == NULL)
	goto error_return;
      elf_hash_table (info)->tls_segment->start = base;
      elf_hash_table (info)->tls_segment->size = end - base;
      elf_hash_table (info)->tls_segment->align = align;
    }

  if (ddr_sec)
    {
      ddr_count = 0;
      ddr_ptr = (unsigned *)bfd_alloc(abfd, 4 * max_datadata_reloc_count + 4);
      if (ddr_ptr)
	++ddr_ptr;
      else
	goto error_return;
    }
  else
    ddr_ptr = NULL;

  /* Since ELF permits relocations to be against local symbols, we
     must have the local symbols available when we do the relocations.
     Since we would rather only read the local symbols once, and we
     would rather not keep them in memory, we handle all the
     relocations for a single input file at the same time.

     Unfortunately, there is no way to know the total number of local
     symbols until we have seen all of them, and the local symbol
     indices precede the global symbol indices.  This means that when
     we are generating relocateable output, and we see a reloc against
     a global symbol, we can not know the symbol index until we have
     finished examining all the local symbols to see which ones we are
     going to output.  To deal with this, we keep the relocations in
     memory, and don't output them until the end of the link.  This is
     an unfortunate waste of memory, but I don't see a good way around
     it.  Fortunately, it only happens when performing a relocateable
     link, which is not the common case.  FIXME: If keep_memory is set
     we could write the relocs out and then read them again; I don't
     know how bad the memory loss will be.  */

  for (sub = info->input_bfds; sub != NULL; sub = sub->link_next)
    sub->output_has_begun = false;
  for (o = abfd->sections; o != NULL; o = o->next)
    {
      for (p = o->link_order_head; p != NULL; p = p->next)
	{
	  if (p->type == bfd_indirect_link_order
	      && (bfd_get_flavour ((sub = p->u.indirect.section->owner))
		  == bfd_target_elf_flavour)
	      && elf_elfheader (sub)->e_ident[EI_CLASS] == bed->s->elfclass)
	    {
	      if (! sub->output_has_begun)
		{
		  if (! elf_link_input_bfd (&finfo, sub))
		    goto error_return;
		  sub->output_has_begun = true;
		}
	    }
	  else if (p->type == bfd_section_reloc_link_order
		   || p->type == bfd_symbol_reloc_link_order)
	    {
	      if (! elf_reloc_link_order (abfd, info, o, p))
		goto error_return;
	    }
	  else
	    {
	      if (! _bfd_default_link_order (abfd, info, o, p))
		goto error_return;
	    }
	}
    }

  /* Output any global symbols that got converted to local in a
     version script or due to symbol visibility.  We do this in a
     separate step since ELF requires all local symbols to appear
     prior to any global symbols.  FIXME: We should only do this if
     some global symbols were, in fact, converted to become local.
     FIXME: Will this work correctly with the Irix 5 linker?  */
  eoinfo.failed = false;
  eoinfo.finfo = &finfo;
  eoinfo.localsyms = true;
  elf_link_hash_traverse (elf_hash_table (info), elf_link_output_extsym,
			  (PTR) &eoinfo);
  if (eoinfo.failed)
    return false;

  /* Set the vma of the sections to 0. We can't do that before, otherwise the
     relocation doesn't work properly for .sbss. */
  {
    int n = elf_elfheader(abfd)->e_shnum;
    Elf_Internal_Shdr **hdr = elf_elfsections(abfd);
    for (i = 1; i < n; ++i, ++hdr)
      (*hdr)->sh_addr = 0;
  }
  
  /* That wrote out all the local symbols.  Finish up the symbol table
     with the global symbols. Even if we want to strip everything we
     can, we still need to deal with those global symbols that got
     converted to local in a version script.  */

  /* The sh_info field records the index of the first non local symbol.  */
  symtab_hdr->sh_info = bfd_get_symcount (abfd);

  if (dynamic
      && finfo.dynsym_sec->output_section != bfd_abs_section_ptr)
    {
      Elf_Internal_Sym sym;
      Elf_External_Sym *dynsym =
	(Elf_External_Sym *) finfo.dynsym_sec->contents;
      long last_local = 0;

      /* Write out the section symbols for the output sections.  */
      if (info->shared)
	{
	  asection *s;

	  sym.st_size = 0;
	  sym.st_name = 0;
	  sym.st_info = ELF_ST_INFO (STB_LOCAL, STT_SECTION);
	  sym.st_other = 0;

	  for (s = abfd->sections; s != NULL; s = s->next)
	    {
	      int indx;
	      Elf_External_Sym *dest;

	      indx = elf_section_data (s)->this_idx;
	      BFD_ASSERT (indx > 0);
	      sym.st_shndx = indx;
	      sym.st_value = s->vma;
	      dest = dynsym + elf_section_data (s)->dynindx;
	      elf_swap_symbol_out (abfd, &sym, (PTR) dest, (PTR) 0);
	    }

	  last_local = bfd_count_sections (abfd);
	}

      /* Write out the local dynsyms.  */
      if (elf_hash_table (info)->dynlocal)
	{
	  struct elf_link_local_dynamic_entry *e;
	  for (e = elf_hash_table (info)->dynlocal; e ; e = e->next)
	    {
	      asection *s;
	      Elf_External_Sym *dest;

	      sym.st_size = e->isym.st_size;
	      sym.st_other = e->isym.st_other;

	      /* Copy the internal symbol as is.
		 Note that we saved a word of storage and overwrote
		 the original st_name with the dynstr_index.  */
	      sym = e->isym;

	      if (e->isym.st_shndx != SHN_UNDEF
		  && (e->isym.st_shndx < SHN_LORESERVE
		      || e->isym.st_shndx > SHN_HIRESERVE))
		{
		  s = bfd_section_from_elf_index (e->input_bfd,
						  e->isym.st_shndx);

		  sym.st_shndx =
		    elf_section_data (s->output_section)->this_idx;
		  sym.st_value = (s->output_section->vma
				  + s->output_offset
				  + e->isym.st_value);
		}

	      if (last_local < e->dynindx)
		last_local = e->dynindx;

	      dest = dynsym + e->dynindx;
	      elf_swap_symbol_out (abfd, &sym, (PTR) dest, (PTR) 0);
	    }
	}

      elf_section_data (finfo.dynsym_sec->output_section)->this_hdr.sh_info =
	last_local + 1;
    }

  /* We get the global symbols from the hash table.  */
  eoinfo.failed = false;
  eoinfo.localsyms = false;
  eoinfo.finfo = &finfo;
  elf_link_hash_traverse (elf_hash_table (info), elf_link_output_extsym,
			  (PTR) &eoinfo);
  if (eoinfo.failed)
    return false;

  /* If backend needs to output some symbols not present in the hash
     table, do it now.  */
  if (bed->elf_backend_output_arch_syms)
    {
      typedef boolean (*out_sym_func) PARAMS ((PTR, const char *,
					       Elf_Internal_Sym *,
					       asection *));

      if (! ((*bed->elf_backend_output_arch_syms)
	     (abfd, info, (PTR) &finfo, (out_sym_func) elf_link_output_sym)))
	return false;
    }

  /* Flush all symbols to the file.  */
  if (! elf_link_flush_output_syms (&finfo))
    return false;

  /* Now we know the size of the symtab section.  */
  off += symtab_hdr->sh_size;

  /* Add the __datadata_relocs table. */
  if (ddr_sec)
    {
      Elf_Internal_Shdr *hdr = elf_elfsections(abfd)[_bfd_elf_section_from_bfd_section(abfd, ddr_sec)];
      ddr_sec->_cooked_size = ddr_sec->_raw_size = hdr->sh_size = 4 * ddr_count + 4;
      hdr->sh_addralign = 2;
      off = _bfd_elf_assign_file_position_for_section (hdr, off, true);
      ddr_ptr -= ddr_count + 1;
      *ddr_ptr = ddr_count ? ddr_count : -1;
      bfd_set_section_contents(abfd, ddr_sec, ddr_ptr, 0, hdr->sh_size);
    }
  
  /* Finish up and write out the symbol string table (.strtab)
     section.  */
  symstrtab_hdr = &elf_tdata (abfd)->strtab_hdr;
  /* sh_name was set in prep_headers.  */
  symstrtab_hdr->sh_type = SHT_STRTAB;
  symstrtab_hdr->sh_flags = 0;
  symstrtab_hdr->sh_addr = 0;
  symstrtab_hdr->sh_size = _bfd_stringtab_size (finfo.symstrtab);
  symstrtab_hdr->sh_entsize = 0;
  symstrtab_hdr->sh_link = 0;
  symstrtab_hdr->sh_info = 0;
  /* sh_offset is set just below.  */
  symstrtab_hdr->sh_addralign = 1;

  off = _bfd_elf_assign_file_position_for_section (symstrtab_hdr, off, true);
  elf_tdata (abfd)->next_file_pos = off;

  if (bfd_get_symcount (abfd) > 0)
    {
      if (bfd_seek (abfd, symstrtab_hdr->sh_offset, SEEK_SET) != 0
	  || ! _bfd_stringtab_emit (abfd, finfo.symstrtab))
	return false;
    }

  /* Adjust the relocs to have the correct symbol indices.  */
  for (o = abfd->sections; o != NULL; o = o->next)
    {
      if ((o->flags & SEC_RELOC) == 0)
	continue;

      elf_link_adjust_relocs (abfd, &elf_section_data (o)->rel_hdr,
			      elf_section_data (o)->rel_count,
			      elf_section_data (o)->rel_hashes);
      if (elf_section_data (o)->rel_hdr2 != NULL)
	elf_link_adjust_relocs (abfd, elf_section_data (o)->rel_hdr2,
				elf_section_data (o)->rel_count2,
				(elf_section_data (o)->rel_hashes
				 + elf_section_data (o)->rel_count));

      elf_section_data (o)->rel_hdr.sh_size =
	o->reloc_count * elf_section_data (o)->rel_hdr.sh_entsize;
      
      /* Set the reloc_count field to 0 to prevent write_relocs from
	 trying to swap the relocs out itself.  */
      o->reloc_count = 0;
    }

  _bfd_elf_assign_file_positions_for_relocs (abfd);

  if (dynamic && info->combreloc && dynobj != NULL)
    relativecount = elf_link_sort_relocs (abfd, info, &reldyn);

  /* If we are linking against a dynamic object, or generating a
     shared library, finish up the dynamic linking information.  */
  if (dynamic)
    {
      Elf_External_Dyn *dyncon, *dynconend;

      /* Fix up .dynamic entries.  */
      o = bfd_get_section_by_name (dynobj, ".dynamic");
      BFD_ASSERT (o != NULL);

      dyncon = (Elf_External_Dyn *) o->contents;
      dynconend = (Elf_External_Dyn *) (o->contents + o->_raw_size);
      for (; dyncon < dynconend; dyncon++)
	{
	  Elf_Internal_Dyn dyn;
	  const char *name;
	  unsigned int type;

	  elf_swap_dyn_in (dynobj, dyncon, &dyn);

	  switch (dyn.d_tag)
	    {
	    default:
	      break;
	    case DT_NULL:
	      if (relativecount > 0 && dyncon + 1 < dynconend)
		{
		  switch (elf_section_data (reldyn)->this_hdr.sh_type)
		    {
		    case SHT_REL: dyn.d_tag = DT_RELCOUNT; break;
		    case SHT_RELA: dyn.d_tag = DT_RELACOUNT; break;
		    default: break;
		    }
		  if (dyn.d_tag != DT_NULL)
		    {
		      dyn.d_un.d_val = relativecount;
		      elf_swap_dyn_out (dynobj, &dyn, dyncon);
		      relativecount = 0;
		    }
		}
	      break;
	    case DT_INIT:
	      name = info->init_function;
	      goto get_sym;
	    case DT_FINI:
	      name = info->fini_function;
	    get_sym:
	      {
		struct elf_link_hash_entry *h;

		h = elf_link_hash_lookup (elf_hash_table (info), name,
					  false, false, true);
		if (h != NULL
		    && (h->root.type == bfd_link_hash_defined
			|| h->root.type == bfd_link_hash_defweak))
		  {
		    dyn.d_un.d_val = h->root.u.def.value;
		    o = h->root.u.def.section;
		    if (o->output_section != NULL)
		      dyn.d_un.d_val += (o->output_section->vma
					 + o->output_offset);
		    else
		      {
			/* The symbol is imported from another shared
			   library and does not apply to this one.  */
			dyn.d_un.d_val = 0;
		      }

		    elf_swap_dyn_out (dynobj, &dyn, dyncon);
		  }
	      }
	      break;

	    case DT_PREINIT_ARRAYSZ:
	      name = ".preinit_array";
	      goto get_size;
	    case DT_INIT_ARRAYSZ:
	      name = ".init_array";
	      goto get_size;
	    case DT_FINI_ARRAYSZ:
	      name = ".fini_array";
	    get_size:
	      o = bfd_get_section_by_name (abfd, name);
	      if (o == NULL)
		{
		  (*_bfd_error_handler)
		    (_("%s: could not find output section %s"),
		     bfd_get_filename (abfd), name);
		  goto error_return;
		}
	      if (o->_raw_size == 0)
		(*_bfd_error_handler)
		  (_("warning: %s section has zero size"), name);
	      dyn.d_un.d_val = o->_raw_size;
	      elf_swap_dyn_out (dynobj, &dyn, dyncon);
	      break;

	    case DT_PREINIT_ARRAY:
	      name = ".preinit_array";
	      goto get_vma;
	    case DT_INIT_ARRAY:
	      name = ".init_array";
	      goto get_vma;
	    case DT_FINI_ARRAY:
	      name = ".fini_array";
	      goto get_vma;

	    case DT_HASH:
	      name = ".hash";
	      goto get_vma;
	    case DT_STRTAB:
	      name = ".dynstr";
	      goto get_vma;
	    case DT_SYMTAB:
	      name = ".dynsym";
	      goto get_vma;
	    case DT_VERDEF:
	      name = ".gnu.version_d";
	      goto get_vma;
	    case DT_VERNEED:
	      name = ".gnu.version_r";
	      goto get_vma;
	    case DT_VERSYM:
	      name = ".gnu.version";
	    get_vma:
	      o = bfd_get_section_by_name (abfd, name);
	      if (o == NULL)
		{
		  (*_bfd_error_handler)
		    (_("%s: could not find output section %s"),
		     bfd_get_filename (abfd), name);
		  goto error_return;
		}
	      dyn.d_un.d_ptr = o->vma;
	      elf_swap_dyn_out (dynobj, &dyn, dyncon);
	      break;

	    case DT_REL:
	    case DT_RELA:
	    case DT_RELSZ:
	    case DT_RELASZ:
	      if (dyn.d_tag == DT_REL || dyn.d_tag == DT_RELSZ)
		type = SHT_REL;
	      else
		type = SHT_RELA;
	      dyn.d_un.d_val = 0;
	      for (i = 1; i < elf_numsections (abfd); i++)
		{
		  Elf_Internal_Shdr *hdr;

		  hdr = elf_elfsections (abfd)[i];
		  if (hdr->sh_type == type
		      && (hdr->sh_flags & SHF_ALLOC) != 0)
		    {
		      if (dyn.d_tag == DT_RELSZ || dyn.d_tag == DT_RELASZ)
			dyn.d_un.d_val += hdr->sh_size;
		      else
			{
			  if (dyn.d_un.d_val == 0
			      || hdr->sh_addr < dyn.d_un.d_val)
			    dyn.d_un.d_val = hdr->sh_addr;
			}
		    }
		}
	      elf_swap_dyn_out (dynobj, &dyn, dyncon);
	      break;
	    }
	}
    }

  /* If we have created any dynamic sections, then output them.  */
  if (dynobj != NULL)
    {
      if (! (*bed->elf_backend_finish_dynamic_sections) (abfd, info))
	goto error_return;

      for (o = dynobj->sections; o != NULL; o = o->next)
	{
	  if ((o->flags & SEC_HAS_CONTENTS) == 0
	      || o->_raw_size == 0
	      || o->output_section == bfd_abs_section_ptr)
	    continue;
	  if ((o->flags & SEC_LINKER_CREATED) == 0)
	    {
	      /* At this point, we are only interested in sections
		 created by elf_link_create_dynamic_sections.  */
	      continue;
	    }
	  if ((elf_section_data (o->output_section)->this_hdr.sh_type
	       != SHT_STRTAB)
	      || strcmp (bfd_get_section_name (abfd, o), ".dynstr") != 0)
	    {
	      if (! bfd_set_section_contents (abfd, o->output_section,
					      o->contents,
					      (file_ptr) o->output_offset,
					      o->_raw_size))
		goto error_return;
	    }
	  else
	    {
	      /* The contents of the .dynstr section are actually in a
		 stringtab.  */
	      off = elf_section_data (o->output_section)->this_hdr.sh_offset;
	      if (bfd_seek (abfd, off, SEEK_SET) != 0
		  || ! _bfd_elf_strtab_emit (abfd,
					     elf_hash_table (info)->dynstr))
		goto error_return;
	    }
	}
    }

  if (info->relocateable)
    {
      boolean failed = false;

      bfd_map_over_sections (abfd, bfd_elf_set_group_contents, &failed);
      if (failed)
	goto error_return;
    }

  /* If we have optimized stabs strings, output them.  */
  if (elf_hash_table (info)->stab_info != NULL)
    {
      if (! _bfd_write_stab_strings (abfd, &elf_hash_table (info)->stab_info))
	goto error_return;
    }

  if (info->eh_frame_hdr && elf_hash_table (info)->dynobj)
    {
      o = bfd_get_section_by_name (elf_hash_table (info)->dynobj,
				   ".eh_frame_hdr");
      if (o
	  && (elf_section_data (o)->sec_info_type
	      == ELF_INFO_TYPE_EH_FRAME_HDR))
	{
	  if (! _bfd_elf_write_section_eh_frame_hdr (abfd, o))
	    goto error_return;
	}
    }

  if (finfo.symstrtab != NULL)
    _bfd_stringtab_free (finfo.symstrtab);
  if (finfo.contents != NULL)
    free (finfo.contents);
  if (finfo.external_relocs != NULL)
    free (finfo.external_relocs);
  if (finfo.internal_relocs != NULL)
    free (finfo.internal_relocs);
  if (finfo.external_syms != NULL)
    free (finfo.external_syms);
  if (finfo.locsym_shndx != NULL)
    free (finfo.locsym_shndx);
  if (finfo.internal_syms != NULL)
    free (finfo.internal_syms);
  if (finfo.indices != NULL)
    free (finfo.indices);
  if (finfo.sections != NULL)
    free (finfo.sections);
  if (finfo.symbuf != NULL)
    free (finfo.symbuf);
  if (finfo.symshndxbuf != NULL)
    free (finfo.symbuf);
  for (o = abfd->sections; o != NULL; o = o->next)
    {
      if ((o->flags & SEC_RELOC) != 0
	  && elf_section_data (o)->rel_hashes != NULL)
	free (elf_section_data (o)->rel_hashes);
    }

  elf_tdata (abfd)->linker = true;

  return true;

 error_return:
  if (finfo.symstrtab != NULL)
    _bfd_stringtab_free (finfo.symstrtab);
  if (finfo.contents != NULL)
    free (finfo.contents);
  if (finfo.external_relocs != NULL)
    free (finfo.external_relocs);
  if (finfo.internal_relocs != NULL)
    free (finfo.internal_relocs);
  if (finfo.external_syms != NULL)
    free (finfo.external_syms);
  if (finfo.locsym_shndx != NULL)
    free (finfo.locsym_shndx);
  if (finfo.internal_syms != NULL)
    free (finfo.internal_syms);
  if (finfo.indices != NULL)
    free (finfo.indices);
  if (finfo.sections != NULL)
    free (finfo.sections);
  if (finfo.symbuf != NULL)
    free (finfo.symbuf);
  if (finfo.symshndxbuf != NULL)
    free (finfo.symbuf);
  for (o = abfd->sections; o != NULL; o = o->next)
    {
      if ((o->flags & SEC_RELOC) != 0
	  && elf_section_data (o)->rel_hashes != NULL)
	free (elf_section_data (o)->rel_hashes);
    }

  return false;
}

/* Add a symbol to the output symbol table.  */

static boolean
elf_link_output_sym (finfo, name, elfsym, input_sec)
     struct elf_final_link_info *finfo;
     const char *name;
     Elf_Internal_Sym *elfsym;
     asection *input_sec;
{
  Elf_External_Sym *dest;
  Elf_External_Sym_Shndx *destshndx;

  boolean (*output_symbol_hook) PARAMS ((bfd *,
					 struct bfd_link_info *info,
					 const char *,
					 Elf_Internal_Sym *,
					 asection *));

  output_symbol_hook = get_elf_backend_data (finfo->output_bfd)->
    elf_backend_link_output_symbol_hook;
  if (output_symbol_hook != NULL)
    {
      if (! ((*output_symbol_hook)
	     (finfo->output_bfd, finfo->info, name, elfsym, input_sec)))
	return false;
    }

  if (name == (const char *) NULL || *name == '\0')
    elfsym->st_name = 0;
  else if (input_sec->flags & SEC_EXCLUDE)
    elfsym->st_name = 0;
  else
    {
      elfsym->st_name = (unsigned long) _bfd_stringtab_add (finfo->symstrtab,
							    name, true, false);
      if (elfsym->st_name == (unsigned long) -1)
	return false;
    }

  if (finfo->symbuf_count >= finfo->symbuf_size)
    {
      if (! elf_link_flush_output_syms (finfo))
	return false;
    }

  dest = finfo->symbuf + finfo->symbuf_count;
  destshndx = finfo->symshndxbuf;
  if (destshndx != NULL)
    destshndx += finfo->symbuf_count;
  elf_swap_symbol_out (finfo->output_bfd, elfsym, (PTR) dest, (PTR) destshndx);
  ++finfo->symbuf_count;

  ++ bfd_get_symcount (finfo->output_bfd);

  return true;
}

/* Flush the output symbols to the file.  */

static boolean
elf_link_flush_output_syms (finfo)
     struct elf_final_link_info *finfo;
{
  if (finfo->symbuf_count > 0)
    {
      Elf_Internal_Shdr *hdr;
      file_ptr pos;
      bfd_size_type amt;

      hdr = &elf_tdata (finfo->output_bfd)->symtab_hdr;
      pos = hdr->sh_offset + hdr->sh_size;
      amt = finfo->symbuf_count * sizeof (Elf_External_Sym);
      if (bfd_seek (finfo->output_bfd, pos, SEEK_SET) != 0
	  || bfd_bwrite ((PTR) finfo->symbuf, amt, finfo->output_bfd) != amt)
	return false;

      hdr->sh_size += amt;

      if (finfo->symshndxbuf != NULL)
	{
	  hdr = &elf_tdata (finfo->output_bfd)->symtab_shndx_hdr;
	  pos = hdr->sh_offset + hdr->sh_size;
	  amt = finfo->symbuf_count * sizeof (Elf_External_Sym_Shndx);
	  if (bfd_seek (finfo->output_bfd, pos, SEEK_SET) != 0
	      || (bfd_bwrite ((PTR) finfo->symshndxbuf, amt, finfo->output_bfd)
		  != amt))
	    return false;

	  hdr->sh_size += amt;
	}

      finfo->symbuf_count = 0;
    }

  return true;
}

/* Adjust all external symbols pointing into SEC_MERGE sections
   to reflect the object merging within the sections.  */

static boolean
elf_link_sec_merge_syms (h, data)
     struct elf_link_hash_entry *h;
     PTR data;
{
  asection *sec;

  if (h->root.type == bfd_link_hash_warning)
    h = (struct elf_link_hash_entry *) h->root.u.i.link;

  if ((h->root.type == bfd_link_hash_defined
       || h->root.type == bfd_link_hash_defweak)
      && ((sec = h->root.u.def.section)->flags & SEC_MERGE)
      && elf_section_data (sec)->sec_info_type == ELF_INFO_TYPE_MERGE)
    {
      bfd *output_bfd = (bfd *) data;

      h->root.u.def.value =
	_bfd_merged_section_offset (output_bfd,
				    &h->root.u.def.section,
				    elf_section_data (sec)->sec_info,
				    h->root.u.def.value, (bfd_vma) 0);
    }

  return true;
}

/* For DSOs loaded in via a DT_NEEDED entry, emulate ld.so in
   allowing an unsatisfied unversioned symbol in the DSO to match a
   versioned symbol that would normally require an explicit version.  */

static boolean
elf_link_check_versioned_symbol (info, h)
     struct bfd_link_info *info;
     struct elf_link_hash_entry *h;
{
  bfd *undef_bfd = h->root.u.undef.abfd;
  struct elf_link_loaded_list *loaded;

  if ((undef_bfd->flags & DYNAMIC) == 0
      || info->hash->creator->flavour != bfd_target_elf_flavour
      || elf_dt_soname (h->root.u.undef.abfd) == NULL)
    return false;

  for (loaded = elf_hash_table (info)->loaded;
       loaded != NULL;
       loaded = loaded->next)
    {
      bfd *input;
      Elf_Internal_Shdr *hdr;
      bfd_size_type symcount;
      bfd_size_type extsymcount;
      bfd_size_type extsymoff;
      Elf_Internal_Shdr *versymhdr;
      Elf_Internal_Sym *isym;
      Elf_Internal_Sym *isymend;
      Elf_Internal_Sym *isymbuf;
      Elf_External_Versym *ever;
      Elf_External_Versym *extversym;

      input = loaded->abfd;

      /* We check each DSO for a possible hidden versioned definition.  */
      if (input == undef_bfd
	  || (input->flags & DYNAMIC) == 0
	  || elf_dynversym (input) == 0)
	continue;

      hdr = &elf_tdata (input)->dynsymtab_hdr;

      symcount = hdr->sh_size / sizeof (Elf_External_Sym);
      if (elf_bad_symtab (input))
	{
	  extsymcount = symcount;
	  extsymoff = 0;
	}
      else
	{
	  extsymcount = symcount - hdr->sh_info;
	  extsymoff = hdr->sh_info;
	}

      if (extsymcount == 0)
	continue;

      isymbuf = bfd_elf_get_elf_syms (input, hdr, extsymcount, extsymoff,
				      NULL, NULL, NULL);
      if (isymbuf == NULL)
	return false;

      /* Read in any version definitions.  */
      versymhdr = &elf_tdata (input)->dynversym_hdr;
      extversym = (Elf_External_Versym *) bfd_malloc (versymhdr->sh_size);
      if (extversym == NULL)
	goto error_ret;

      if (bfd_seek (input, versymhdr->sh_offset, SEEK_SET) != 0
	  || (bfd_bread ((PTR) extversym, versymhdr->sh_size, input)
	      != versymhdr->sh_size))
	{
	  free (extversym);
	error_ret:
	  free (isymbuf);
	  return false;
	}

      ever = extversym + extsymoff;
      isymend = isymbuf + extsymcount;
      for (isym = isymbuf; isym < isymend; isym++, ever++)
	{
	  const char *name;
	  Elf_Internal_Versym iver;

	  if (ELF_ST_BIND (isym->st_info) == STB_LOCAL
	      || isym->st_shndx == SHN_UNDEF)
	    continue;

	  name = bfd_elf_string_from_elf_section (input,
						  hdr->sh_link,
						  isym->st_name);
	  if (strcmp (name, h->root.root.string) != 0)
	    continue;

	  _bfd_elf_swap_versym_in (input, ever, &iver);

	  if ((iver.vs_vers & VERSYM_HIDDEN) == 0)
	    {
	      /* If we have a non-hidden versioned sym, then it should
		 have provided a definition for the undefined sym.  */
	      abort ();
	    }

	  if ((iver.vs_vers & VERSYM_VERSION) == 2)
	    {
	      /* This is the oldest (default) sym.  We can use it.  */
	      free (extversym);
	      free (isymbuf);
	      return true;
	    }
	}

      free (extversym);
      free (isymbuf);
    }

  return false;
}

/* Add an external symbol to the symbol table.  This is called from
   the hash table traversal routine.  When generating a shared object,
   we go through the symbol table twice.  The first time we output
   anything that might have been forced to local scope in a version
   script.  The second time we output the symbols that are still
   global symbols.  */

static boolean
elf_link_output_extsym (h, data)
     struct elf_link_hash_entry *h;
     PTR data;
{
  struct elf_outext_info *eoinfo = (struct elf_outext_info *) data;
  struct elf_final_link_info *finfo = eoinfo->finfo;
  boolean strip;
  Elf_Internal_Sym sym;
  asection *input_sec;

  if (h->root.type == bfd_link_hash_warning)
    {
      h = (struct elf_link_hash_entry *) h->root.u.i.link;
      if (h->root.type == bfd_link_hash_new)
	return true;
    }

  /* Decide whether to output this symbol in this pass.  */
  if (eoinfo->localsyms)
    {
      if ((h->elf_link_hash_flags & ELF_LINK_FORCED_LOCAL) == 0)
	return true;
    }
  else
    {
      if ((h->elf_link_hash_flags & ELF_LINK_FORCED_LOCAL) != 0)
	return true;
    }

  /* If we are not creating a shared library, and this symbol is
     referenced by a shared library but is not defined anywhere, then
     warn that it is undefined.  If we do not do this, the runtime
     linker will complain that the symbol is undefined when the
     program is run.  We don't have to worry about symbols that are
     referenced by regular files, because we will already have issued
     warnings for them.  */
  if (! finfo->info->relocateable
      && ! finfo->info->allow_shlib_undefined
      && ! finfo->info->shared
      && h->root.type == bfd_link_hash_undefined
      && (h->elf_link_hash_flags & ELF_LINK_HASH_REF_DYNAMIC) != 0
      && (h->elf_link_hash_flags & ELF_LINK_HASH_REF_REGULAR) == 0
      && ! elf_link_check_versioned_symbol (finfo->info, h))
    {
      if (! ((*finfo->info->callbacks->undefined_symbol)
	     (finfo->info, h->root.root.string, h->root.u.undef.abfd,
	      (asection *) NULL, (bfd_vma) 0, true)))
	{
	  eoinfo->failed = true;
	  return false;
	}
    }

  /* We don't want to output symbols that have never been mentioned by
     a regular file, or that we have been told to strip.  However, if
     h->indx is set to -2, the symbol is used by a reloc and we must
     output it.  */
  if (h->indx == -2)
    strip = false;
  else if (((h->elf_link_hash_flags & ELF_LINK_HASH_DEF_DYNAMIC) != 0
	    || (h->elf_link_hash_flags & ELF_LINK_HASH_REF_DYNAMIC) != 0)
	   && (h->elf_link_hash_flags & ELF_LINK_HASH_DEF_REGULAR) == 0
	   && (h->elf_link_hash_flags & ELF_LINK_HASH_REF_REGULAR) == 0)
    strip = true;
  else if (finfo->info->strip == strip_all
	   || (finfo->info->strip == strip_some
	       && bfd_hash_lookup (finfo->info->keep_hash,
				   h->root.root.string,
				   false, false) == NULL))
    strip = true;
  else
    strip = false;

  /* If we're stripping it, and it's not a dynamic symbol, there's
     nothing else to do unless it is a forced local symbol.  */
  if (strip
      && h->dynindx == -1
      && (h->elf_link_hash_flags & ELF_LINK_FORCED_LOCAL) == 0)
    return true;

  sym.st_value = 0;
  sym.st_size = h->size;
  sym.st_other = h->other;
  if ((h->elf_link_hash_flags & ELF_LINK_FORCED_LOCAL) != 0)
    sym.st_info = ELF_ST_INFO (STB_LOCAL, h->type);
  else if (h->root.type == bfd_link_hash_undefweak
	   || h->root.type == bfd_link_hash_defweak)
    sym.st_info = ELF_ST_INFO (STB_WEAK, h->type);
  else
    sym.st_info = ELF_ST_INFO (STB_GLOBAL, h->type);

  switch (h->root.type)
    {
    default:
    case bfd_link_hash_new:
    case bfd_link_hash_warning:
      abort ();
      return false;

    case bfd_link_hash_undefined:
    case bfd_link_hash_undefweak:
      input_sec = bfd_und_section_ptr;
      sym.st_shndx = SHN_UNDEF;
      break;

    case bfd_link_hash_defined:
    case bfd_link_hash_defweak:
      {
	input_sec = h->root.u.def.section;
	if (input_sec->output_section != NULL)
	  {
	    sym.st_shndx =
	      _bfd_elf_section_from_bfd_section (finfo->output_bfd,
						 input_sec->output_section);
	    if (sym.st_shndx == SHN_BAD)
	      {
		(*_bfd_error_handler)
		  (_("%s: could not find output section %s for input section %s"),
		   bfd_get_filename (finfo->output_bfd),
		   input_sec->output_section->name,
		   input_sec->name);
		eoinfo->failed = true;
		return false;
	      }

	    /* ELF symbols in relocateable files are section relative,
	       but in nonrelocateable files they are virtual
	       addresses.  */
	    sym.st_value = h->root.u.def.value + input_sec->output_offset;
#if 0
	    if (! finfo->info->relocateable)
	      {
		sym.st_value += input_sec->output_section->vma;
		if (h->type == STT_TLS)
		  {
		    /* STT_TLS symbols are relative to PT_TLS segment
		       base.  */
		    BFD_ASSERT (finfo->first_tls_sec != NULL);
		    sym.st_value -= finfo->first_tls_sec->vma;
		  }
	      }
#endif
	  }
	else
	  {
	    BFD_ASSERT (input_sec->owner == NULL
			|| (input_sec->owner->flags & DYNAMIC) != 0);
	    sym.st_shndx = SHN_UNDEF;
	    input_sec = bfd_und_section_ptr;
	  }
      }
      break;

    case bfd_link_hash_common:
      input_sec = h->root.u.c.p->section;
      sym.st_shndx = SHN_COMMON;
      sym.st_value = 1 << h->root.u.c.p->alignment_power;
      break;

    case bfd_link_hash_indirect:
      /* These symbols are created by symbol versioning.  They point
	 to the decorated version of the name.  For example, if the
	 symbol foo@@GNU_1.2 is the default, which should be used when
	 foo is used with no version, then we add an indirect symbol
	 foo which points to foo@@GNU_1.2.  We ignore these symbols,
	 since the indirected symbol is already in the hash table.  */
      return true;
    }

  /* Give the processor backend a chance to tweak the symbol value,
     and also to finish up anything that needs to be done for this
     symbol.  FIXME: Not calling elf_backend_finish_dynamic_symbol for
     forced local syms when non-shared is due to a historical quirk.  */
  if ((h->dynindx != -1
       || (h->elf_link_hash_flags & ELF_LINK_FORCED_LOCAL) != 0)
      && (finfo->info->shared
	  || (h->elf_link_hash_flags & ELF_LINK_FORCED_LOCAL) == 0)
      && elf_hash_table (finfo->info)->dynamic_sections_created)
    {
      struct elf_backend_data *bed;

      bed = get_elf_backend_data (finfo->output_bfd);
      if (! ((*bed->elf_backend_finish_dynamic_symbol)
	     (finfo->output_bfd, finfo->info, h, &sym)))
	{
	  eoinfo->failed = true;
	  return false;
	}
    }

  /* If we are marking the symbol as undefined, and there are no
     non-weak references to this symbol from a regular object, then
     mark the symbol as weak undefined; if there are non-weak
     references, mark the symbol as strong.  We can't do this earlier,
     because it might not be marked as undefined until the
     finish_dynamic_symbol routine gets through with it.  */
  if (sym.st_shndx == SHN_UNDEF
      && (h->elf_link_hash_flags & ELF_LINK_HASH_REF_REGULAR) != 0
      && (ELF_ST_BIND (sym.st_info) == STB_GLOBAL
	  || ELF_ST_BIND (sym.st_info) == STB_WEAK))
    {
      int bindtype;

      if ((h->elf_link_hash_flags & ELF_LINK_HASH_REF_REGULAR_NONWEAK) != 0)
	bindtype = STB_GLOBAL;
      else
	bindtype = STB_WEAK;
      sym.st_info = ELF_ST_INFO (bindtype, ELF_ST_TYPE (sym.st_info));
    }

  /* If a symbol is not defined locally, we clear the visibility
     field.  */
  if (! finfo->info->relocateable
      && (h->elf_link_hash_flags & ELF_LINK_HASH_DEF_REGULAR) == 0)
    sym.st_other ^= ELF_ST_VISIBILITY (sym.st_other);

  /* If this symbol should be put in the .dynsym section, then put it
     there now.  We already know the symbol index.  We also fill in
     the entry in the .hash section.  */
  if (h->dynindx != -1
      && elf_hash_table (finfo->info)->dynamic_sections_created)
    {
      size_t bucketcount;
      size_t bucket;
      size_t hash_entry_size;
      bfd_byte *bucketpos;
      bfd_vma chain;
      Elf_External_Sym *esym;

      sym.st_name = h->dynstr_index;
      esym = (Elf_External_Sym *) finfo->dynsym_sec->contents + h->dynindx;
      elf_swap_symbol_out (finfo->output_bfd, &sym, (PTR) esym, (PTR) 0);

      bucketcount = elf_hash_table (finfo->info)->bucketcount;
      bucket = h->elf_hash_value % bucketcount;
      hash_entry_size
	= elf_section_data (finfo->hash_sec)->this_hdr.sh_entsize;
      bucketpos = ((bfd_byte *) finfo->hash_sec->contents
		   + (bucket + 2) * hash_entry_size);
      chain = bfd_get (8 * hash_entry_size, finfo->output_bfd, bucketpos);
      bfd_put (8 * hash_entry_size, finfo->output_bfd, (bfd_vma) h->dynindx,
	       bucketpos);
      bfd_put (8 * hash_entry_size, finfo->output_bfd, chain,
	       ((bfd_byte *) finfo->hash_sec->contents
		+ (bucketcount + 2 + h->dynindx) * hash_entry_size));

      if (finfo->symver_sec != NULL && finfo->symver_sec->contents != NULL)
	{
	  Elf_Internal_Versym iversym;
	  Elf_External_Versym *eversym;

	  if ((h->elf_link_hash_flags & ELF_LINK_HASH_DEF_REGULAR) == 0)
	    {
	      if (h->verinfo.verdef == NULL)
		iversym.vs_vers = 0;
	      else
		iversym.vs_vers = h->verinfo.verdef->vd_exp_refno + 1;
	    }
	  else
	    {
	      if (h->verinfo.vertree == NULL)
		iversym.vs_vers = 1;
	      else
		iversym.vs_vers = h->verinfo.vertree->vernum + 1;
	    }

	  if ((h->elf_link_hash_flags & ELF_LINK_HIDDEN) != 0)
	    iversym.vs_vers |= VERSYM_HIDDEN;

	  eversym = (Elf_External_Versym *) finfo->symver_sec->contents;
	  eversym += h->dynindx;
	  _bfd_elf_swap_versym_out (finfo->output_bfd, &iversym, eversym);
	}
    }

  /* If we're stripping it, then it was just a dynamic symbol, and
     there's nothing else to do.  */
  if (strip || (input_sec->flags & SEC_EXCLUDE) != 0)
    return true;

  h->indx = bfd_get_symcount (finfo->output_bfd);

  if (! elf_link_output_sym (finfo, h->root.root.string, &sym, input_sec))
    {
      eoinfo->failed = true;
      return false;
    }

  return true;
}

/* Copy the relocations indicated by the INTERNAL_RELOCS (which
   originated from the section given by INPUT_REL_HDR) to the
   OUTPUT_BFD.  */

static boolean
elf_link_output_relocs (output_bfd, input_section, input_rel_hdr,
			internal_relocs)
     bfd *output_bfd;
     asection *input_section;
     Elf_Internal_Shdr *input_rel_hdr;
     Elf_Internal_Rela *internal_relocs;
{
  Elf_Internal_Rela *irela;
  Elf_Internal_Rela *irelaend;
  Elf_Internal_Shdr *output_rel_hdr;
  asection *output_section;
  unsigned int *rel_countp = NULL;
  struct elf_backend_data *bed;
  bfd_size_type amt;

  output_section = input_section->output_section;
  output_rel_hdr = NULL;

  if (elf_section_data (output_section)->rel_hdr.sh_entsize
      == input_rel_hdr->sh_entsize)
    {
      output_rel_hdr = &elf_section_data (output_section)->rel_hdr;
      rel_countp = &elf_section_data (output_section)->rel_count;
    }
  else if (elf_section_data (output_section)->rel_hdr2
	   && (elf_section_data (output_section)->rel_hdr2->sh_entsize
	       == input_rel_hdr->sh_entsize))
    {
      output_rel_hdr = elf_section_data (output_section)->rel_hdr2;
      rel_countp = &elf_section_data (output_section)->rel_count2;
    }
  else
    {
      (*_bfd_error_handler)
	(_("%s: relocation size mismatch in %s section %s"),
	 bfd_get_filename (output_bfd),
	 bfd_archive_filename (input_section->owner),
	 input_section->name);
      bfd_set_error (bfd_error_wrong_object_format);
      return false;
    }

  bed = get_elf_backend_data (output_bfd);
  irela = internal_relocs;
  irelaend = irela + (NUM_SHDR_ENTRIES (input_rel_hdr)
		      * bed->s->int_rels_per_ext_rel);

  if (input_rel_hdr->sh_entsize == sizeof (Elf_External_Rel))
    {
      Elf_External_Rel *erel;
      Elf_Internal_Rel *irel;

      amt = bed->s->int_rels_per_ext_rel * sizeof (Elf_Internal_Rel);
      irel = (Elf_Internal_Rel *) bfd_zmalloc (amt);
      if (irel == NULL)
	{
	  (*_bfd_error_handler) (_("Error: out of memory"));
	  abort ();
	}

      erel = ((Elf_External_Rel *) output_rel_hdr->contents + *rel_countp);
      for (; irela < irelaend; irela += bed->s->int_rels_per_ext_rel, erel++)
	{
	  unsigned int i;

	  for (i = 0; i < bed->s->int_rels_per_ext_rel; i++)
	    {
	      irel[i].r_offset = irela[i].r_offset;
	      irel[i].r_info = irela[i].r_info;
	      BFD_ASSERT (irela[i].r_addend == 0);
	    }

	  if (bed->s->swap_reloc_out)
	    (*bed->s->swap_reloc_out) (output_bfd, irel, (PTR) erel);
	  else
	    elf_swap_reloc_out (output_bfd, irel, erel);
	}

      free (irel);
    }
  else
    {
      Elf_External_Rela *erela;

      BFD_ASSERT (input_rel_hdr->sh_entsize == sizeof (Elf_External_Rela));

      erela = ((Elf_External_Rela *) output_rel_hdr->contents + *rel_countp);
      for (; irela < irelaend; irela += bed->s->int_rels_per_ext_rel, erela++)
	if (bed->s->swap_reloca_out)
	  (*bed->s->swap_reloca_out) (output_bfd, irela, (PTR) erela);
	else
	  elf_swap_reloca_out (output_bfd, irela, erela);
    }

  /* Bump the counter, so that we know where to add the next set of
     relocations.  */
  *rel_countp += NUM_SHDR_ENTRIES (input_rel_hdr);

  return true;
}

/* Link an input file into the linker output file.  This function
   handles all the sections and relocations of the input file at once.
   This is so that we only have to read the local symbols once, and
   don't have to keep them in memory.  */

static boolean
elf_link_input_bfd (finfo, input_bfd)
     struct elf_final_link_info *finfo;
     bfd *input_bfd;
{
  boolean (*relocate_section) PARAMS ((bfd *, struct bfd_link_info *,
				       bfd *, asection *, bfd_byte *,
				       Elf_Internal_Rela *,
				       Elf_Internal_Sym *, asection **));
  bfd *output_bfd;
  Elf_Internal_Shdr *symtab_hdr;
  size_t locsymcount;
  size_t extsymoff;
  Elf_Internal_Sym *isymbuf;
  Elf_Internal_Sym *isym;
  Elf_Internal_Sym *isymend;
  long *pindex;
  asection **ppsection;
  asection *o;
  struct elf_backend_data *bed;
  boolean emit_relocs;
  struct elf_link_hash_entry **sym_hashes;

  output_bfd = finfo->output_bfd;
  bed = get_elf_backend_data (output_bfd);
  relocate_section = bed->elf_backend_relocate_section;

  /* If this is a dynamic object, we don't want to do anything here:
     we don't want the local symbols, and we don't want the section
     contents.  */
  if ((input_bfd->flags & DYNAMIC) != 0)
    return true;

  emit_relocs = (finfo->info->relocateable
		 || finfo->info->emitrelocations
		 || bed->elf_backend_emit_relocs);

  symtab_hdr = &elf_tdata (input_bfd)->symtab_hdr;
  if (elf_bad_symtab (input_bfd))
    {
      locsymcount = symtab_hdr->sh_size / sizeof (Elf_External_Sym);
      extsymoff = 0;
    }
  else
    {
      locsymcount = symtab_hdr->sh_info;
      extsymoff = symtab_hdr->sh_info;
    }

  /* Read the local symbols.  */
  isymbuf = (Elf_Internal_Sym *) symtab_hdr->contents;
  if (isymbuf == NULL && locsymcount != 0)
    {
      isymbuf = bfd_elf_get_elf_syms (input_bfd, symtab_hdr, locsymcount, 0,
				      finfo->internal_syms,
				      finfo->external_syms,
				      finfo->locsym_shndx);
      if (isymbuf == NULL)
	return false;
    }

  /* Find local symbol sections and adjust values of symbols in
     SEC_MERGE sections.  Write out those local symbols we know are
     going into the output file.  */
  isymend = isymbuf + locsymcount;
  for (isym = isymbuf, pindex = finfo->indices, ppsection = finfo->sections;
       isym < isymend;
       isym++, pindex++, ppsection++)
    {
      asection *isec;
      const char *name;
      Elf_Internal_Sym osym;

      *pindex = -1;

      if (elf_bad_symtab (input_bfd))
	{
	  if (ELF_ST_BIND (isym->st_info) != STB_LOCAL)
	    {
	      *ppsection = NULL;
	      continue;
	    }
	}

      if (isym->st_shndx == SHN_UNDEF)
	isec = bfd_und_section_ptr;
      else if (isym->st_shndx < SHN_LORESERVE
	       || isym->st_shndx > SHN_HIRESERVE)
	{
	  isec = section_from_elf_index (input_bfd, isym->st_shndx);
	  if (isec
	      && elf_section_data (isec)->sec_info_type == ELF_INFO_TYPE_MERGE
	      && ELF_ST_TYPE (isym->st_info) != STT_SECTION)
	    isym->st_value =
	      _bfd_merged_section_offset (output_bfd, &isec,
					  elf_section_data (isec)->sec_info,
					  isym->st_value, (bfd_vma) 0);
	}
      else if (isym->st_shndx == SHN_ABS)
	isec = bfd_abs_section_ptr;
      else if (isym->st_shndx == SHN_COMMON)
	isec = bfd_com_section_ptr;
      else
	{
	  /* Who knows?  */
	  isec = NULL;
	}

      *ppsection = isec;

      /* Don't output the first, undefined, symbol.  */
      if (ppsection == finfo->sections)
	continue;

      if (ELF_ST_TYPE (isym->st_info) == STT_SECTION)
	{
	  /* We never output section symbols.  Instead, we use the
	     section symbol of the corresponding section in the output
	     file.  */
	  continue;
	}

      /* If we are stripping all symbols, we don't want to output this
	 one.  */
      if (finfo->info->strip == strip_all)
	continue;

      /* If we are discarding all local symbols, we don't want to
	 output this one.  If we are generating a relocateable output
	 file, then some of the local symbols may be required by
	 relocs; we output them below as we discover that they are
	 needed.  */
      if (finfo->info->discard == discard_all)
	continue;

      /* If this symbol is defined in a section which we are
	 discarding, we don't need to keep it, but note that
	 linker_mark is only reliable for sections that have contents.
	 For the benefit of the MIPS ELF linker, we check SEC_EXCLUDE
	 as well as linker_mark.  */
      if ((isym->st_shndx < SHN_LORESERVE || isym->st_shndx > SHN_HIRESERVE)
	  && isec != NULL
	  && ((! isec->linker_mark && (isec->flags & SEC_HAS_CONTENTS) != 0)
	      || (! finfo->info->relocateable
		  && (isec->flags & SEC_EXCLUDE) != 0)))
	continue;

      /* Get the name of the symbol.  */
      name = bfd_elf_string_from_elf_section (input_bfd, symtab_hdr->sh_link,
					      isym->st_name);
      if (name == NULL)
	return false;

      /* See if we are discarding symbols with this name.  */
      if ((finfo->info->strip == strip_some
	   && (bfd_hash_lookup (finfo->info->keep_hash, name, false, false)
	       == NULL))
	  || (((finfo->info->discard == discard_sec_merge
		&& (isec->flags & SEC_MERGE) && ! finfo->info->relocateable)
	       || finfo->info->discard == discard_l)
	      && bfd_is_local_label_name (input_bfd, name)))
	continue;

      /* If we get here, we are going to output this symbol.  */

      osym = *isym;

      /* Adjust the section index for the output file.  */
      osym.st_shndx = _bfd_elf_section_from_bfd_section (output_bfd,
							 isec->output_section);
      if (osym.st_shndx == SHN_BAD)
	return false;

      *pindex = bfd_get_symcount (output_bfd);

      /* ELF symbols in relocateable files are section relative, but
	 in executable files they are virtual addresses.  Note that
	 this code assumes that all ELF sections have an associated
	 BFD section with a reasonable value for output_offset; below
	 we assume that they also have a reasonable value for
	 output_section.  Any special sections must be set up to meet
	 these requirements.  */
      osym.st_value += isec->output_offset;
      if (! finfo->info->relocateable)
	{
	  osym.st_value += isec->output_section->vma;
	  if (ELF_ST_TYPE (osym.st_info) == STT_TLS)
	    {
	      /* STT_TLS symbols are relative to PT_TLS segment base.  */
	      BFD_ASSERT (finfo->first_tls_sec != NULL);
	      osym.st_value -= finfo->first_tls_sec->vma;
	    }
	}

      if (! elf_link_output_sym (finfo, name, &osym, isec))
	return false;
    }

  /* Relocate the contents of each section.  */
  sym_hashes = elf_sym_hashes (input_bfd);
  for (o = input_bfd->sections; o != NULL; o = o->next)
    {
      bfd_byte *contents;

      if (! o->linker_mark)
	{
	  /* This section was omitted from the link.  */
	  continue;
	}

      if ((o->flags & SEC_HAS_CONTENTS) == 0
	  || (o->_raw_size == 0 && (o->flags & SEC_RELOC) == 0))
	continue;

      if ((o->flags & SEC_LINKER_CREATED) != 0)
	{
	  /* Section was created by elf_link_create_dynamic_sections
	     or somesuch.  */
	  continue;
	}

      /* Get the contents of the section.  They have been cached by a
	 relaxation routine.  Note that o is a section in an input
	 file, so the contents field will not have been set by any of
	 the routines which work on output files.  */
      if (elf_section_data (o)->this_hdr.contents != NULL)
	contents = elf_section_data (o)->this_hdr.contents;
      else
	{
	  contents = finfo->contents;
	  if (! bfd_get_section_contents (input_bfd, o, contents,
					  (file_ptr) 0, o->_raw_size))
	    return false;
	}

      if ((o->flags & SEC_RELOC) != 0)
	{
	  Elf_Internal_Rela *internal_relocs;

	  /* Get the swapped relocs.  */
	  internal_relocs = (NAME(_bfd_elf,link_read_relocs)
			     (input_bfd, o, finfo->external_relocs,
			      finfo->internal_relocs, false));
	  if (internal_relocs == NULL
	      && o->reloc_count > 0)
	    return false;

	  /* Run through the relocs looking for any against symbols
	     from discarded sections and section symbols from
	     removed link-once sections.  Complain about relocs
	     against discarded sections.  Zero relocs against removed
	     link-once sections.  We should really complain if
	     anything in the final link tries to use it, but
	     DWARF-based exception handling might have an entry in
	     .eh_frame to describe a routine in the linkonce section,
	     and it turns out to be hard to remove the .eh_frame
	     entry too.  FIXME.  */
	  if (!finfo->info->relocateable
	      && !elf_section_ignore_discarded_relocs (o))
	    {
	      Elf_Internal_Rela *rel, *relend;

	      rel = internal_relocs;
	      relend = rel + o->reloc_count * bed->s->int_rels_per_ext_rel;
	      for ( ; rel < relend; rel++)
		{
		  unsigned long r_symndx = ELF_R_SYM (rel->r_info);

		  if (r_symndx >= locsymcount
		      || (elf_bad_symtab (input_bfd)
			  && finfo->sections[r_symndx] == NULL))
		    {
		      struct elf_link_hash_entry *h;

		      h = sym_hashes[r_symndx - extsymoff];
		      while (h->root.type == bfd_link_hash_indirect
			     || h->root.type == bfd_link_hash_warning)
			h = (struct elf_link_hash_entry *) h->root.u.i.link;

		      /* Complain if the definition comes from a
			 discarded section.  */
		      if ((h->root.type == bfd_link_hash_defined
			   || h->root.type == bfd_link_hash_defweak)
			  && elf_discarded_section (h->root.u.def.section))
			{
			  if ((o->flags & SEC_DEBUGGING) != 0)
			    {
			      BFD_ASSERT (r_symndx != 0);
			      memset (rel, 0, sizeof (*rel));
			    }
			  else
			    {
			      if (! ((*finfo->info->callbacks->undefined_symbol)
				     (finfo->info, h->root.root.string,
				      input_bfd, o, rel->r_offset,
				      true)))
				return false;
			    }
			}
		    }
		  else
		    {
		      asection *sec = finfo->sections[r_symndx];

		      if (sec != NULL && elf_discarded_section (sec))
			{
			  if ((o->flags & SEC_DEBUGGING) != 0
			      || (sec->flags & SEC_LINK_ONCE) != 0)
			    {
			      BFD_ASSERT (r_symndx != 0);
			      rel->r_info
				= ELF_R_INFO (0, ELF_R_TYPE (rel->r_info));
			      rel->r_addend = 0;
			    }
			  else
			    {
			      boolean ok;
			      const char *msg
				= _("local symbols in discarded section %s");
			      bfd_size_type amt
				= strlen (sec->name) + strlen (msg) - 1;
			      char *buf = (char *) bfd_malloc (amt);

			      if (buf != NULL)
				sprintf (buf, msg, sec->name);
			      else
				buf = (char *) sec->name;
			      ok = (*finfo->info->callbacks
				    ->undefined_symbol) (finfo->info, buf,
							 input_bfd, o,
							 rel->r_offset,
							 true);
			      if (buf != sec->name)
				free (buf);
			      if (!ok)
				return false;
			    }
			}
		    }
		}
	    }

	  /* Relocate the section by invoking a back end routine.

	     The back end routine is responsible for adjusting the
	     section contents as necessary, and (if using Rela relocs
	     and generating a relocateable output file) adjusting the
	     reloc addend as necessary.

	     The back end routine does not have to worry about setting
	     the reloc address or the reloc symbol index.

	     The back end routine is given a pointer to the swapped in
	     internal symbols, and can access the hash table entries
	     for the external symbols via elf_sym_hashes (input_bfd).

	     When generating relocateable output, the back end routine
	     must handle STB_LOCAL/STT_SECTION symbols specially.  The
	     output symbol is going to be a section symbol
	     corresponding to the output section, which will require
	     the addend to be adjusted.  */

	  if (! (*relocate_section) (output_bfd, finfo->info,
				     input_bfd, o, contents,
				     internal_relocs,
				     isymbuf,
				     finfo->sections))
	    return false;

	  if (emit_relocs)
	    {
	      Elf_Internal_Rela *irela;
	      Elf_Internal_Rela *irelaend;
	      struct elf_link_hash_entry **rel_hash;
	      Elf_Internal_Shdr *input_rel_hdr, *input_rel_hdr2;
	      unsigned int next_erel;
	      boolean (*reloc_emitter) PARAMS ((bfd *, asection *,
						Elf_Internal_Shdr *,
						Elf_Internal_Rela *));
	      boolean rela_normal;

	      input_rel_hdr = &elf_section_data (o)->rel_hdr;
	      rela_normal = (bed->rela_normal
			     && (input_rel_hdr->sh_entsize
				 == sizeof (Elf_External_Rela)));

	      /* Adjust the reloc addresses and symbol indices.  */

	      irela = internal_relocs;
	      irelaend = irela + o->reloc_count * bed->s->int_rels_per_ext_rel;
	      rel_hash = (elf_section_data (o->output_section)->rel_hashes
			  + elf_section_data (o->output_section)->rel_count
			  + elf_section_data (o->output_section)->rel_count2);
	      for (next_erel = 0; irela < irelaend; irela++, next_erel++)
		{
		  unsigned long r_symndx;
		  asection *sec;
		  Elf_Internal_Sym sym;

		  if (next_erel == bed->s->int_rels_per_ext_rel)
		    {
		      rel_hash++;
		      next_erel = 0;
		    }

		  irela->r_offset += o->output_offset;

		  /* Relocs in an executable have to be virtual addresses.  */
		  if (!finfo->info->relocateable)
		    irela->r_offset += o->output_section->vma;

		  r_symndx = ELF_R_SYM (irela->r_info);

		  if (r_symndx == 0)
		    continue;

		  if (r_symndx >= locsymcount
		      || (elf_bad_symtab (input_bfd)
			  && finfo->sections[r_symndx] == NULL))
		    {
		      struct elf_link_hash_entry *rh;
		      unsigned long indx;

		      /* This is a reloc against a global symbol.  We
			 have not yet output all the local symbols, so
			 we do not know the symbol index of any global
			 symbol.  We set the rel_hash entry for this
			 reloc to point to the global hash table entry
			 for this symbol.  The symbol index is then
			 set at the end of elf_bfd_final_link.  */
		      indx = r_symndx - extsymoff;
		      rh = elf_sym_hashes (input_bfd)[indx];
		      while (rh->root.type == bfd_link_hash_indirect
			     || rh->root.type == bfd_link_hash_warning)
			rh = (struct elf_link_hash_entry *) rh->root.u.i.link;

		      /* Setting the index to -2 tells
			 elf_link_output_extsym that this symbol is
			 used by a reloc.  */
		      BFD_ASSERT (rh->indx < 0);
		      rh->indx = -2;

		      *rel_hash = rh;

		      continue;
		    }

		  /* This is a reloc against a local symbol.  */

		  *rel_hash = NULL;
		  sym = isymbuf[r_symndx];
		  sec = finfo->sections[r_symndx];
		  if (ELF_ST_TYPE (sym.st_info) == STT_SECTION)
		    {
		      /* I suppose the backend ought to fill in the
			 section of any STT_SECTION symbol against a
			 processor specific section.  If we have
			 discarded a section, the output_section will
			 be the absolute section.  */
		      if (bfd_is_abs_section (sec)
			  || (sec != NULL
			      && bfd_is_abs_section (sec->output_section)))
			r_symndx = 0;
		      else if (sec == NULL || sec->owner == NULL)
			{
			  bfd_set_error (bfd_error_bad_value);
			  return false;
			}
		      else
			{
			  r_symndx = sec->output_section->target_index;
			  BFD_ASSERT (r_symndx != 0);
			}

		      /* Adjust the addend according to where the
			 section winds up in the output section.  */
		      if (rela_normal)
			irela->r_addend += sec->output_offset;
		    }
		  else
		    {
		      if (finfo->indices[r_symndx] == -1)
			{
			  unsigned long shlink;
			  const char *name;
			  asection *osec;

			  if (finfo->info->strip == strip_all)
			    {
			      /* You can't do ld -r -s.  */
			      bfd_set_error (bfd_error_invalid_operation);
			      return false;
			    }

			  /* This symbol was skipped earlier, but
			     since it is needed by a reloc, we
			     must output it now.  */
			  shlink = symtab_hdr->sh_link;
			  name = (bfd_elf_string_from_elf_section
				  (input_bfd, shlink, sym.st_name));
			  if (name == NULL)
			    return false;

			  osec = sec->output_section;
			  sym.st_shndx =
			    _bfd_elf_section_from_bfd_section (output_bfd,
							       osec);
			  if (sym.st_shndx == SHN_BAD)
			    return false;

			  sym.st_value += sec->output_offset;
#if 0
			  if (! finfo->info->relocateable)
			    {
			      sym.st_value += osec->vma;
			      if (ELF_ST_TYPE (sym.st_info) == STT_TLS)
				{
				  /* STT_TLS symbols are relative to PT_TLS
				     segment base.  */
				  BFD_ASSERT (finfo->first_tls_sec != NULL);
				  sym.st_value -= finfo->first_tls_sec->vma;
				}
			    }
#endif

			  finfo->indices[r_symndx]
			    = bfd_get_symcount (output_bfd);

			  if (! elf_link_output_sym (finfo, name, &sym, sec))
			    return false;
			}

		      r_symndx = finfo->indices[r_symndx];
		    }

		  irela->r_info = ELF_R_INFO (r_symndx,
					      ELF_R_TYPE (irela->r_info));
		}

	      /* Swap out the relocs.  */
	      if (bed->elf_backend_emit_relocs
		  && !(finfo->info->relocateable
		       || finfo->info->emitrelocations))
		reloc_emitter = bed->elf_backend_emit_relocs;
	      else
		reloc_emitter = elf_link_output_relocs;

	      if (input_rel_hdr->sh_size != 0
		  && ! (*reloc_emitter) (output_bfd, o, input_rel_hdr,
					 internal_relocs))
		return false;

	      input_rel_hdr2 = elf_section_data (o)->rel_hdr2;
	      if (input_rel_hdr2 && input_rel_hdr2->sh_size != 0)
		{
		  internal_relocs += (NUM_SHDR_ENTRIES (input_rel_hdr)
				      * bed->s->int_rels_per_ext_rel);
		  if (! (*reloc_emitter) (output_bfd, o, input_rel_hdr2,
					  internal_relocs))
		    return false;
		}
	    }
	}

      /* Write out the modified section contents.  */
      if (bed->elf_backend_write_section
	  && (*bed->elf_backend_write_section) (output_bfd, o, contents))
	{
	  /* Section written out.  */
	}
      else switch (elf_section_data (o)->sec_info_type)
	{
	case ELF_INFO_TYPE_STABS:
	  if (! (_bfd_write_section_stabs
		 (output_bfd,
		  &elf_hash_table (finfo->info)->stab_info,
		  o, &elf_section_data (o)->sec_info, contents)))
	    return false;
	  break;
	case ELF_INFO_TYPE_MERGE:
	  if (! (_bfd_write_merged_section
		 (output_bfd, o, elf_section_data (o)->sec_info)))
	    return false;
	  break;
	case ELF_INFO_TYPE_EH_FRAME:
	  {
	    asection *ehdrsec;

	    ehdrsec
	      = bfd_get_section_by_name (elf_hash_table (finfo->info)->dynobj,
					 ".eh_frame_hdr");
	    if (! (_bfd_elf_write_section_eh_frame (output_bfd, o, ehdrsec,
						    contents)))
	      return false;
	  }
	  break;
	default:
	  {
	    bfd_size_type sec_size;

	    sec_size = (o->_cooked_size != 0 ? o->_cooked_size : o->_raw_size);
	    if (! (o->flags & SEC_EXCLUDE)
		&& ! bfd_set_section_contents (output_bfd, o->output_section,
					       contents,
					       (file_ptr) o->output_offset,
					       sec_size))
	      return false;
	  }
	  break;
	}
    }

  return true;
}

/* Generate a reloc when linking an ELF file.  This is a reloc
   requested by the linker, and does come from any input file.  This
   is used to build constructor and destructor tables when linking
   with -Ur.  */

static boolean
elf_reloc_link_order (output_bfd, info, output_section, link_order)
     bfd *output_bfd;
     struct bfd_link_info *info;
     asection *output_section;
     struct bfd_link_order *link_order;
{
  reloc_howto_type *howto;
  long indx;
  bfd_vma offset;
  bfd_vma addend;
  struct elf_link_hash_entry **rel_hash_ptr;
  Elf_Internal_Shdr *rel_hdr;
  struct elf_backend_data *bed = get_elf_backend_data (output_bfd);

  howto = bfd_reloc_type_lookup (output_bfd, link_order->u.reloc.p->reloc);
  if (howto == NULL)
    {
      bfd_set_error (bfd_error_bad_value);
      return false;
    }

  addend = link_order->u.reloc.p->addend;

  /* Figure out the symbol index.  */
  rel_hash_ptr = (elf_section_data (output_section)->rel_hashes
		  + elf_section_data (output_section)->rel_count
		  + elf_section_data (output_section)->rel_count2);
  if (link_order->type == bfd_section_reloc_link_order)
    {
      indx = link_order->u.reloc.p->u.section->target_index;
      BFD_ASSERT (indx != 0);
      *rel_hash_ptr = NULL;
    }
  else
    {
      struct elf_link_hash_entry *h;

      /* Treat a reloc against a defined symbol as though it were
	 actually against the section.  */
      h = ((struct elf_link_hash_entry *)
	   bfd_wrapped_link_hash_lookup (output_bfd, info,
					 link_order->u.reloc.p->u.name,
					 false, false, true));
      if (h != NULL
	  && (h->root.type == bfd_link_hash_defined
	      || h->root.type == bfd_link_hash_defweak))
	{
	  asection *section;

	  section = h->root.u.def.section;
	  indx = section->output_section->target_index;
	  *rel_hash_ptr = NULL;
	  /* It seems that we ought to add the symbol value to the
	     addend here, but in practice it has already been added
	     because it was passed to constructor_callback.  */
	  addend += section->output_section->vma + section->output_offset;
	}
      else if (h != NULL)
	{
	  /* Setting the index to -2 tells elf_link_output_extsym that
	     this symbol is used by a reloc.  */
	  h->indx = -2;
	  *rel_hash_ptr = h;
	  indx = 0;
	}
      else
	{
	  if (! ((*info->callbacks->unattached_reloc)
		 (info, link_order->u.reloc.p->u.name, (bfd *) NULL,
		  (asection *) NULL, (bfd_vma) 0)))
	    return false;
	  indx = 0;
	}
    }

  /* If this is an inplace reloc, we must write the addend into the
     object file.  */
  if (howto->partial_inplace && addend != 0)
    {
      bfd_size_type size;
      bfd_reloc_status_type rstat;
      bfd_byte *buf;
      boolean ok;
      const char *sym_name;

      size = bfd_get_reloc_size (howto);
      buf = (bfd_byte *) bfd_zmalloc (size);
      if (buf == (bfd_byte *) NULL)
	return false;
      rstat = _bfd_relocate_contents (howto, output_bfd, (bfd_vma) addend, buf);
      switch (rstat)
	{
	case bfd_reloc_ok:
	  break;

	default:
	case bfd_reloc_outofrange:
	  abort ();

	case bfd_reloc_overflow:
	  if (link_order->type == bfd_section_reloc_link_order)
	    sym_name = bfd_section_name (output_bfd,
					 link_order->u.reloc.p->u.section);
	  else
	    sym_name = link_order->u.reloc.p->u.name;
	  if (! ((*info->callbacks->reloc_overflow)
		 (info, sym_name, howto->name, addend,
		  (bfd *) NULL, (asection *) NULL, (bfd_vma) 0)))
	    {
	      free (buf);
	      return false;
	    }
	  break;
	}
      ok = bfd_set_section_contents (output_bfd, output_section, (PTR) buf,
				     (file_ptr) link_order->offset, size);
      free (buf);
      if (! ok)
	return false;
    }

  /* The address of a reloc is relative to the section in a
     relocateable file, and is a virtual address in an executable
     file.  */
  offset = link_order->offset;
  if (! info->relocateable)
    offset += output_section->vma;

  rel_hdr = &elf_section_data (output_section)->rel_hdr;

  if (rel_hdr->sh_type == SHT_REL)
    {
      bfd_size_type size;
      Elf_Internal_Rel *irel;
      Elf_External_Rel *erel;
      unsigned int i;

      size = bed->s->int_rels_per_ext_rel * sizeof (Elf_Internal_Rel);
      irel = (Elf_Internal_Rel *) bfd_zmalloc (size);
      if (irel == NULL)
	return false;

      for (i = 0; i < bed->s->int_rels_per_ext_rel; i++)
	irel[i].r_offset = offset;
      irel[0].r_info = ELF_R_INFO (indx, howto->type);

      erel = ((Elf_External_Rel *) rel_hdr->contents
	      + elf_section_data (output_section)->rel_count);

      if (bed->s->swap_reloc_out)
	(*bed->s->swap_reloc_out) (output_bfd, irel, (bfd_byte *) erel);
      else
	elf_swap_reloc_out (output_bfd, irel, erel);

      free (irel);
    }
  else
    {
      bfd_size_type size;
      Elf_Internal_Rela *irela;
      Elf_External_Rela *erela;
      unsigned int i;

      size = bed->s->int_rels_per_ext_rel * sizeof (Elf_Internal_Rela);
      irela = (Elf_Internal_Rela *) bfd_zmalloc (size);
      if (irela == NULL)
	return false;

      for (i = 0; i < bed->s->int_rels_per_ext_rel; i++)
	irela[i].r_offset = offset;
      irela[0].r_info = ELF_R_INFO (indx, howto->type);
      irela[0].r_addend = addend;

      erela = ((Elf_External_Rela *) rel_hdr->contents
	       + elf_section_data (output_section)->rel_count);

      if (bed->s->swap_reloca_out)
	(*bed->s->swap_reloca_out) (output_bfd, irela, (bfd_byte *) erela);
      else
	elf_swap_reloca_out (output_bfd, irela, erela);
    }

  ++elf_section_data (output_section)->rel_count;

  return true;
}

static boolean
elf_section_ignore_discarded_relocs (sec)
     asection *sec;
{
  struct elf_backend_data *bed;

  switch (elf_section_data (sec)->sec_info_type)
    {
    case ELF_INFO_TYPE_STABS:
    case ELF_INFO_TYPE_EH_FRAME:
      return true;
    default:
      break;
    }

  bed = get_elf_backend_data (sec->owner);
  if (bed->elf_backend_ignore_discarded_relocs != NULL
      && (*bed->elf_backend_ignore_discarded_relocs) (sec))
    return true;

  return false;
}

#define TARGET_BIG_SYM		bfd_elf32_morphos_vec
#define TARGET_BIG_NAME		"elf32-morphos"
#define ELF_ARCH		bfd_arch_powerpc
#define ELF_MACHINE_CODE	EM_PPC
#define ELF_MAXPAGESIZE		0x10000
#define elf_info_to_howto	ppc_elf_info_to_howto

#ifdef  EM_CYGNUS_POWERPC
#define ELF_MACHINE_ALT1	EM_CYGNUS_POWERPC
#endif

#ifdef EM_PPC_OLD
#define ELF_MACHINE_ALT2	EM_PPC_OLD
#endif

#define elf_backend_plt_not_loaded	1
#define elf_backend_got_symbol_offset	4
#define elf_backend_can_gc_sections	1
#define elf_backend_can_refcount	1
#define elf_backend_got_header_size	12
#define elf_backend_plt_header_size	PLT_INITIAL_ENTRY_SIZE
#define elf_backend_rela_normal		1

#define bfd_elf32_bfd_merge_private_bfd_data	ppc_elf_merge_private_bfd_data
#define bfd_elf32_bfd_relax_section             ppc_elf_relax_section
#define bfd_elf32_bfd_reloc_type_lookup		ppc_elf_reloc_type_lookup
#define bfd_elf32_bfd_set_private_flags		ppc_elf_set_private_flags
/*#define bfd_elf32_bfd_final_link		_bfd_elf32_gc_common_final_link*/
#define bfd_elf32_bfd_final_link                ppc_elf_final_link

#define elf_backend_object_p			ppc_elf_object_p
#define elf_backend_gc_mark_hook		ppc_elf_gc_mark_hook
#define elf_backend_gc_sweep_hook		ppc_elf_gc_sweep_hook
#define elf_backend_section_from_shdr		ppc_elf_section_from_shdr
#define elf_backend_relocate_section		ppc_elf_relocate_section
#define elf_backend_create_dynamic_sections	ppc_elf_create_dynamic_sections
#define elf_backend_check_relocs		ppc_elf_check_relocs
#define elf_backend_adjust_dynamic_symbol	ppc_elf_adjust_dynamic_symbol
/*#define elf_backend_add_symbol_hook		ppc_elf_add_symbol_hook*/
#define elf_backend_size_dynamic_sections	ppc_elf_size_dynamic_sections
#define elf_backend_finish_dynamic_symbol	ppc_elf_finish_dynamic_symbol
#define elf_backend_finish_dynamic_sections	ppc_elf_finish_dynamic_sections
#define elf_backend_fake_sections		ppc_elf_fake_sections
#define elf_backend_additional_program_headers	ppc_elf_additional_program_headers
#define elf_backend_modify_segment_map		ppc_elf_modify_segment_map
#define elf_backend_grok_prstatus		ppc_elf_grok_prstatus
#define elf_backend_grok_psinfo			ppc_elf_grok_psinfo
#define elf_backend_reloc_type_class		ppc_elf_reloc_type_class

#include "elf32-target.h"
