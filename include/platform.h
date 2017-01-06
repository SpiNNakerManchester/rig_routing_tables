#ifndef __PLATFORM_H__

#ifdef SPINNAKER
  static inline void * safe_malloc(uint bytes)
  {
    void* p = sark_xalloc(sv->sdram_heap, bytes, 0, ALLOC_LOCK);
    if (p == NULL)
    {
      io_printf(IO_BUF, "Failed to malloc %u bytes.\n", bytes);
      rt_error(RTE_MALLOC);
    }
    return p;
  }

  static inline void safe_xfree(void *ptr){
    sark_xfree(sv->sdram_heap, ptr, ALLOC_LOCK);
  }

  #ifdef PROFILED
    void profile_init();
    void *profiled_malloc(uint bytes);
    void profiled_free(void * ptr);

    #define MALLOC profiled_malloc
    #define FREE   profiled_free
  #else
    #define MALLOC safe_malloc
    #define FREE   safe_xfree
  #endif

#else
  #include <stdlib.h>

  #define MALLOC safe_malloc
  #define FREE   safe_xfree
#endif

#define __PLATFORM_H__
#endif  // __PLATFORN_H__
