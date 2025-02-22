// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.
#include <kernel/buddy.h>
#include <kernel/types.h>
#include <kernel/param.h>
#include <kernel/kalloc.h>
#include <kernel/printf.h>

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

void
kinit()
{
  char *p = (char *) PGROUNDUP((uint64) end);
  bd_init(p, (void*)PHYSTOP);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  // if pa is poined to by more than one ref_count, decrement ref_count and return
  if (ref_count[(uint64)pa/PGSIZE] > 1) {
    ref_count[(uint64)pa/PGSIZE]--;
  } else {
    ref_count[(uint64)pa/PGSIZE] = 0;
    bd_free(pa);
  }
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  void *pa = bd_malloc(PGSIZE);
  if (pa) {
    ref_count[(uint64)pa/PGSIZE] = 1;
    return pa;
  } else {
    panic("kalloc out of memory");
  }
}
