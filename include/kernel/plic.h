#ifndef PLIC_H
#define PLIC_H

void            plicinit(void);
void            plicinithart(void);
int             plic_claim(void);
void            plic_complete(int);

#endif // PLIC_H
