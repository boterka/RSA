//
// Created by Boterk on 2023. 10. 22..
//

#ifndef NAGYHAZI_FUNCS_H
#define NAGYHAZI_FUNCS_H

#include <stdbool.h>
#include "inttypes.h"

unsigned long LNKO(unsigned long a, unsigned  long b);

unsigned long A_mod_m(unsigned long a, unsigned long m);

char* RSA_Encrypt(char *text, unsigned long c, char t);

char* RSA_Decrypt(char *text, unsigned long d, char t);

bool Fermat_Test(unsigned long m);

bool Miller_Rabin_Test(unsigned long m);


#endif //NAGYHAZI_FUNCS_H


