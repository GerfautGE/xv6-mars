#include "hasht.h"
#include "param.h"
#include "types.h"
#include "riscv.h"
#include "defs.h"

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

  if (ht->count >= threshold_up) {
    ht_resize(ht, ht->size * 2);
  } else if (ht->count <= threshold_down) {
    ht_resize(ht, ht->size / 2);
  }

  int index = ht_hash(key, ht->size);
  if (ht->items[index] == 0) {
    ht->items[index] = item;
    ht->count++;
  } else {
    struct ht_item *current = ht->items[index];
    while (current->next != 0) {
      current = current->next;
    }
    current->next = item;
  }
}
int ht_remove_item(struct ht_table *ht, int key) {
  int index = ht_hash(key, ht->size);
  struct ht_item *item = ht->items[index];
  struct ht_item *prev = 0;
  while (item != 0) {
    if (item->key == key) {
      if (prev == 0) {
        ht->items[index] = item->next;
      } else {
        prev->next = item->next;
      }
      bd_free(item);
      ht->count--;
      return item->key;
    }
    prev = item;
    item = item->next;
  }
  return -1;
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

void ht_resize(struct ht_table *ht, int new_size) {
  struct ht_table *new_ht = ht_create(new_size);
  for (int i = 0; i < ht->size; i++) {
    struct ht_item *item = ht->items[i];
    while (item != 0) {
      ht_insert(new_ht, item->key, item->value);
      item = item->next;
    }
  }
  ht = new_ht;
  ht_delete(new_ht);
}

int ht_hash(int key, int size) { return key+1 % size; }
