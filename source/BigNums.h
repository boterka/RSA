#ifndef PRIMEGENERATOR_BIGNUMS_H
#define PRIMEGENERATOR_BIGNUMS_H

#include <stdio.h>
#include "stdlib.h"
#include "inttypes.h"
#include "stdbool.h"
#include "debugmalloc.h"
#include "mtwister.h"


//UBint----------------------------------------------------------------------

typedef struct unboundInt UnboundInt;
typedef struct uint64s Uints64s;
UnboundInt* CreateUBint();

//UBint----------------------------------------------------------------------

//Miller-Rabin teszt
bool isPrime(UnboundInt* M, unsigned short number_of_witnesses);

bool Miller_Rabin(UnboundInt* M, UnboundInt* c, uint64_t k, UnboundInt* a);

UnboundInt* GenerateBigPrime(uint64_t digits, unsigned long seed);

bool CMP(UnboundInt* A, UnboundInt* B);

bool isZero(UnboundInt* A);

UnboundInt* A_to_the_B_Mod_M(UnboundInt* A, UnboundInt* B, UnboundInt* M);

void RemoveZeroFromStart(UnboundInt* UB);

UnboundInt* Copy(UnboundInt* UB);

UnboundInt** Divide(UnboundInt* divident, UnboundInt* divisor);

UnboundInt* DivideWhole(UnboundInt* divident, UnboundInt* divisor);

UnboundInt* Mod(UnboundInt* divident, UnboundInt* divisor);

bool signwork(UnboundInt* A, UnboundInt* B);

bool rekShift(Uints64s* A, bool dir, bool carry_in);

bool Shift(UnboundInt* A, bool dir, bool carry_in);

void GettingBigger(UnboundInt* A, bool carry_in);

UnboundInt* Multiply(UnboundInt* A, UnboundInt* B);

void RemovefromStart(UnboundInt* dest);

void RemovefromEnd(UnboundInt* dest);

void ConnecttoStart(UnboundInt *dest, uint64_t num);

void PrintLLUasHex(uint64_t num);

void Subtract(UnboundInt* diff, UnboundInt* toSub);

void Add(UnboundInt* sum, UnboundInt* toAdd);

void FreeUB(UnboundInt* UB);

void TwosComp(UnboundInt* UB);

void PrintasHex(UnboundInt* UB);

bool AgreaterorequalthanB(UnboundInt* A, UnboundInt* B);

UnboundInt** Generate_512bitRSAKey(unsigned long seed1, unsigned long seed2);

UnboundInt* LNKO(UnboundInt* A, UnboundInt* B);

UnboundInt** SolveLinCong(UnboundInt* a, UnboundInt* b, UnboundInt* m);



//Basic operations
//-----------------------------------------------------
UnboundInt* Plus(UnboundInt* A, UnboundInt* B);
UnboundInt* Minus(UnboundInt* A, UnboundInt* B);
UnboundInt* Times(UnboundInt* A, UnboundInt* B);
UnboundInt* Quotient(UnboundInt* A, UnboundInt* B);
UnboundInt* Negative(UnboundInt* A);
UnboundInt* GCD(UnboundInt* A, UnboundInt* B);
//only with positive numbers
UnboundInt* Modulo(UnboundInt* A, UnboundInt* B);
UnboundInt** Division(UnboundInt* A, UnboundInt* B);
//------------------------------------------------------

UnboundInt** Generate_RSA_Keyspairs(unsigned long seed1, unsigned long seed2, UnboundInt* public_Key);


#endif //PRIMEGENERATOR_BIGNUMS_H
