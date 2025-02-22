#ifndef HASHT_H
#define HASHT_H

#define HT_INITIAL_BASE_SIZE 7

struct ht_item {
  int key;
  struct proc *value;
  struct ht_item *next;
};

struct ht_table {
  int size;
  int count;
  struct ht_item **items;
};

struct ht_table *ht_create(int size);
void ht_delete(struct ht_table *ht);
void ht_insert(struct ht_table *ht, int key, struct proc *value);
int ht_remove_item(struct ht_table *ht, int key);
struct proc *ht_get(struct ht_table *ht, int key);
int ht_hash(int key, int size);
void ht_resize(struct ht_table **ht, int new_size);

#endif // HASHT_H
