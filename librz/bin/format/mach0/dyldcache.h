// SPDX-FileCopyrightText: 2021 Florian Märkl <info@florianmaerkl.de>
// SPDX-FileCopyrightText: 2020 Francesco Tamagni <mrmacete@protonmail.ch>
// SPDX-License-Identifier: LGPL-3.0-only

#ifndef RZ_BIN_FORMAT_DYLDCACHE_H
#define RZ_BIN_FORMAT_DYLDCACHE_H

#include <rz_util.h>
#include <set.h>

#define RZ_BIN_MACH064 1

#include "mach0.h"

typedef struct rz_dyld_rebase_info_t {
	ut8 version;
	ut64 slide;
	ut8 *one_page_buf;
	ut32 page_size;
	ut64 start_of_data;
} RzDyldRebaseInfo;

typedef struct rz_dyld_rebase_infos_entry_t {
	ut64 start;
	ut64 end;
	RzDyldRebaseInfo *info;
} RzDyldRebaseInfosEntry;

typedef struct rz_dyld_rebase_infos_t {
	RzDyldRebaseInfosEntry *entries;
	size_t length;
} RzDyldRebaseInfos;

typedef struct rz_dyld_rebase_info_3_t {
	ut8 version;
	ut64 slide;
	ut8 *one_page_buf;
	ut32 page_size;
	ut64 start_of_data;
	ut16 *page_starts;
	ut32 page_starts_count;
	ut64 delta_mask;
	ut32 delta_shift;
	ut64 auth_value_add;
} RzDyldRebaseInfo3;

typedef struct rz_dyld_rebase_info_2_t {
	ut8 version;
	ut64 slide;
	ut8 *one_page_buf;
	ut32 page_size;
	ut64 start_of_data;
	ut16 *page_starts;
	ut32 page_starts_count;
	ut16 *page_extras;
	ut32 page_extras_count;
	ut64 delta_mask;
	ut64 value_mask;
	ut32 delta_shift;
	ut64 value_add;
} RzDyldRebaseInfo2;

typedef struct rz_dyld_rebase_info_1_t {
	ut8 version;
	ut64 slide;
	ut8 *one_page_buf;
	ut32 page_size;
	ut64 start_of_data;
	ut16 *toc;
	ut32 toc_count;
	ut8 *entries;
	ut32 entries_size;
} RzDyldRebaseInfo1;

typedef struct rz_dyld_loc_sym_t {
	ut64 local_symbols_offset;
	ut64 nlists_offset;
	ut64 nlists_count;
	ut64 strings_offset;
	ut64 strings_size;
} RzDyldLocSym;

typedef struct rz_bin_dyld_image_t {
	char *file;
	ut64 header_at;
	ut64 hdr_offset;
	ut64 symbols_off;
	ut64 va;
	ut32 nlist_start_index;
	ut32 nlist_count;
} RzDyldBinImage;

typedef struct rz_dyldcache_t {
	ut8 magic[8];

	cache_hdr_t *hdr;
	ut64 *hdr_offset;
	ut64 symbols_off_base;
	ut32 *maps_index;
	ut32 n_hdr;
	cache_map_t *maps;
	ut32 n_maps;

	RzList /*<RzDyldBinImage *>*/ *bins;
	RzBuffer *buf;
	RzDyldRebaseInfos *rebase_infos;
	cache_accel_t *accel;
	RzDyldLocSym *locsym;
	objc_cache_opt_info *oi;
	bool objc_opt_info_loaded;
} RzDyldCache;

RZ_API bool rz_dyldcache_check_magic(const char *magic);
RZ_API RzDyldCache *rz_dyldcache_new_buf(RzBuffer *buf);
RZ_API void rz_dyldcache_free(RzDyldCache *cache);
RZ_API ut64 rz_dyldcache_va2pa(RzDyldCache *cache, uint64_t vaddr, ut32 *offset, ut32 *left);
RZ_API ut64 rz_dyldcache_get_slide(RzDyldCache *cache);
RZ_API objc_cache_opt_info *rz_dyldcache_get_objc_opt_info(RzBinFile *bf, RzDyldCache *cache);
RZ_API void rz_dyldcache_symbols_from_locsym(RzDyldCache *cache, RzDyldBinImage *bin, RzList /*<RzBinSymbol *>*/ *symbols, SetU *hash);

RZ_API RzBuffer *rz_dyldcache_new_rebasing_buf(RzDyldCache *cache);
RZ_API bool rz_dyldcache_needs_rebasing(RzDyldCache *cache);
RZ_API bool rz_dyldcache_range_needs_rebasing(RzDyldCache *cache, ut64 paddr, ut64 size);

#endif
