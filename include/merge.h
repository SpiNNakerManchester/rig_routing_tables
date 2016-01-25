#include "bitset.h"
#include "routing_table.h"

#ifndef __MERGE_H__

typedef struct _merge_t
{
  bitset_t entries;  // Set of entries included in the merge
  table_t* table;    // Table against which the merge is defined

  int goodness;      // Goodness of the merge
  keymask_t keymask; // Keymask resulting from the merge
} merge_t;


// Initialise a merge
static inline bool merge_init(merge_t m, table_t* table)
{
  // Store the table pointer, initialise the keymask and goodness
  m.table = table;

  // Initialise the bitset
  if (!bitset_init(m.entries, table->size))
  {
    return false;
  }
  else
  {
    merge_clear(m);
    return true;
  }
}


// Destruct a merge
static inline void merge_delete(merge_t m)
{
  // Free the bitset
  bitset_delete(m.entries);
}


// Clear a merge
static inline void merge_clear(merge_t m)
{
  // Clear the bitset
  bitset_clear(m.entries);

  // Initialise the keymask and goodness
  m.keymask = {0xffffffff, 0x0};  // Matches nothing
  m.goodness = -1;  // Contains no entries at present
}


// Add an entry to the merge
static inline void merge_add(merge_t m, unsigned int i)
{
  entry_t e = m.table->entries[i];

  // Get the keymask
  if (m.entries.bitset.count == 0)
  {
    // If this is the first entry in the merge then the merge keymask is a copy
    // of the entry keymask.
    m.keymask = e.keymask;
  }
  else
  {
    // Otherwise update the key and mask associated with the merge
    m.keymask = keymask_merge(m.keymask, e.keymask);
  }

  // Add the entry to the bitset contained in the merge
  bitset_add(m.entries, i);

  // Update the recorded goodness of the merge
  m.goodness++;
}


// See if an entry is contained within a merge
static inline bool merge_contains(merge_t m, unsigned int i)
{
  return bitset_contains(merge.entries, i);
}


// Remove an entry from the merge
static inline void merge_remove(merge_t m, unsigned int i)
{
  // Remove the entry from the bitset contained in the merge
  bitset_remove(m.entries, i);

  // Rebuild the key and mask from scratch
  keymask_t km;
  bool init = false;
  for (unsigned int j = 0; j < m.table->size; j++)
  {
    entry_t e = m.table->entries[j];

    if (bitset_contains(m.entries, j))
    {
      if (!init)
      {
        // Initialise the keymask
        km = e.keymask;
        init = true;
      }
      else
      {
        // Merge the keymask
        km = keymask_merge(km, e.keymask);
      }
    }
  }

  // Update the recorded goodness of the merge
  m.goodness--;
}


#define __MERGE_H__
#endif  // __MERGE_H__
