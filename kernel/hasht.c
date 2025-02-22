#include <kernel/buddy.h>
#include <kernel/hasht.h>

struct ht_table *ht_create(int size) {
  struct ht_table *ht = (struct ht_table *)bd_malloc(sizeof(struct ht_table));
  if (ht == 0) {
    return 0;
  }
  ht->size = size;
  ht->count = 0;
  ht->items = (struct ht_item **)bd_malloc(sizeof(struct ht_item *) * ht->size);
  if (ht->items == 0) {
    bd_free(ht);
    return 0;
  }
  // populate the hash table with null pointers
  for (int i = 0; i < ht->size; i++) {
    ht->items[i] = 0;
  }
  return ht;
}

void ht_delete(struct ht_table *ht) {
  for (int i = 0; i < ht->size; i++) {
    struct ht_item *item = ht->items[i];
    while (item != 0) {
      struct ht_item *next = item->next;
      bd_free(item);
      item = next;
    }
  }
  bd_free(ht->items);
  bd_free(ht);
}

void ht_insert(struct ht_table *ht, int key, struct proc *value) {
  struct ht_item *item = (struct ht_item *)bd_malloc(sizeof(struct ht_item));
  if (item == 0) {
    return;
  }
  item->key = key;
  item->value = value;
  item->next = 0;

  int threshold_up =
      (ht->size * 7 * 52) >> 9; // 7*(52/512) almost equals 0.7 (70%)
  int threshold_down =
      (ht->size * 3 * 52) >> 9; // 3*(52/512) almost equals 0.3 (30%)

  struct ht_table *resized_ht = ht;
  if (ht->count >= threshold_up) {
    ht_resize(&resized_ht, ht->size * 2);
  } else if (ht->size>=HT_INITIAL_BASE_SIZE*2 && ht->count <= threshold_down) { // Be carreful not to resize too much
    ht_resize(&resized_ht, ht->size / 2);
  }
  ht = resized_ht;

  int index = ht_hash(key, ht->size);
  if (ht->items[index] == 0) {
    ht->items[index] = item;
    ht->count++;
  } else {
    struct ht_item *current = ht->items[index];
    item->next = current;
    ht->items[index] = item;
  }
}


void ht_insert_no_resize(struct ht_table *ht, int key, struct proc *value) {
  // Assume that new size is already calculated in ht_resize
  // Avoid looping between ht_insert and ht_resize
  struct ht_item *item = (struct ht_item *)bd_malloc(sizeof(struct ht_item));
  if (item == 0) {
    return;
  }
  item->key = key;
  item->value = value;
  item->next = 0;
  int index = ht_hash(key, ht->size);
  if (ht->items[index] == 0) {
    ht->items[index] = item;
    ht->count++;
  } else {
    struct ht_item *current = ht->items[index];
    item->next = current;
    ht->items[index] = item;
  }
}


int ht_remove_item(struct ht_table *ht, int key) {
  int index = ht_hash(key, ht->size);
  struct ht_item *item = ht->items[index];
  struct ht_item *prev = 0;

  if (!item) return -1;  // On empty list

  // Two cases :
  // - item is the first of the list and next null (count--)
  // - item is not alone in the list (count stays the same)
  if (item->key == key && item->next == 0) {
    bd_free(item);
    ht->items[index] = 0;
    ht->count--;
    return 0;
  } else{
    while (item != 0) {
      if (item->key == key) {
        if (prev) {
          prev->next = item->next;
        } else {
          ht->items[index] = item->next;
        }
        bd_free(item);
        return 0;
      }
      prev = item;
      item = item->next;
    }
  }
  return -1; // item not found
}

struct proc *ht_get(struct ht_table *ht, int key) {
  int index = ht_hash(key, ht->size);
  struct ht_item *item = ht->items[index];
  while (item != 0) {
    if (item->key == key) {
      return item->value;
    }
    item = item->next;
  }
  return 0;
}


void ht_resize(struct ht_table **ht, int new_size) {
  struct ht_table *temp_ht = ht_create(new_size);
  if (!temp_ht) return;

  for (int i = 0; i < (*ht)->size; i++) {
    struct ht_item *item = (*ht)->items[i];
    while (item) {
      ht_insert_no_resize(temp_ht, item->key, item->value);
      struct ht_item *to_free = item;
      item = item->next;
      bd_free(to_free);
    }
  }

  // Remove the old items array
  bd_free((*ht)->items);
  (*ht)->items = temp_ht->items;
  (*ht)->size = new_size;

  bd_free(temp_ht); // Delete temporary hash table
}



int ht_hash(int key, int size) { return key % size; }
