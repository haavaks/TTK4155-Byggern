#include "XMEM.h"

void XMEM_init(void)
{
    MCUCR = (1 << SRE);
    SFIOR = (1 << XMM2);
}