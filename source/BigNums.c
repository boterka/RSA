#include <stdio.h>
#include "stdlib.h"
#include "inttypes.h"
#include "stdbool.h"
#include "debugmalloc.h"
#include "BigNums.h"
#include "mtwister.h"
#include "time.h"

//UBint----------------------------------------------------------------------

typedef struct uint64s {

    uint64_t number;
    struct uint64s* next;
    struct uint64s* prev;

} Uints64s;

typedef struct unboundInt {

    int size;
    bool sign;
    Uints64s *start;
    Uints64s *end;

} UnboundInt;

//fel kell szabadítani
UnboundInt* CreateUBint()
{
    UnboundInt* UB = (UnboundInt*) malloc(sizeof(UnboundInt));
    UB->size = 0;
    UB->sign = false; // mivel a 2'C-ben ha az előjel bit 1 akkor negatív, itt is ezt követjük, alapból így pozitív
    UB->start = NULL;
    UB->end = NULL;

    return UB;
}

//UBint----------------------------------------------------------------------


UnboundInt* Copy(UnboundInt* UB)
{
    UnboundInt* ret = CreateUBint();
    Uints64s* mozgo = UB->end;

    while (mozgo != NULL)
    {
        ConnecttoStart(ret, mozgo->number);
        mozgo = mozgo->prev;
    }

    ret->sign = UB->sign;

    return ret;
}

void ConnecttoEnd(UnboundInt *dest, uint64_t num);

void RemoveZeroFromStart(UnboundInt* UB)
{


    if(isZero(UB))
        return;


    Uints64s* mozgo = UB->start;
    while (mozgo->number == 0llu)
    {
        mozgo = mozgo->next;
        RemovefromStart(UB);

        if(mozgo == NULL)
            break;
    }
}

//felszabadítja az előző két számot
//fel kell szabadítani a tömböt amit vissza ad!!! 0. a quotient 1. a maradék
// a maradék előjele mindig pozitív, csak egész osztani lehet negatív számokat :/ evvan
UnboundInt** Divide(UnboundInt* divident, UnboundInt* divisor)
{



    bool Sign = signwork(divident, divisor);
    RemoveZeroFromStart(divident);
    RemoveZeroFromStart(divisor);
    UnboundInt* quotient = CreateUBint();
    ConnecttoStart(quotient, 0llu);


    // ha az osztó nagyobb mint az osztandó
    if(!AgreaterorequalthanB(divident, divisor))
    {
        if(Sign)
            TwosComp(quotient);


        FreeUB(divisor);
        UnboundInt **lista = (UnboundInt**) calloc(2 ,sizeof(UnboundInt*));
        lista[0] = quotient;
        lista[1] = Copy(divident);
        FreeUB(divident);

        return lista;
    }



    UnboundInt* remainder = CreateUBint();
    ConnecttoStart(remainder, 0llu);
    bool Start = false;

    //osztás bitenként
    uint64_t i = 64 * divident->size;
    while (i != 0)
    {
        GettingBigger(remainder,Shift(divident, false, false));
        if(AgreaterorequalthanB(remainder, divisor))
        {
            Subtract(remainder, divisor);
            GettingBigger(quotient,true);

            if(!Start)
                Start = true;
        }
        else if(Start)
            GettingBigger(quotient,false);

        --i;
    }



    if(Sign)
        TwosComp(quotient);



    FreeUB(divisor);
    FreeUB(divident);
    UnboundInt **lista = (UnboundInt**) calloc(2 ,sizeof(UnboundInt*));
    lista[0] = quotient;
    lista[1] = remainder;
    return lista;
}

UnboundInt* DivideWhole(UnboundInt* divident, UnboundInt* divisor)
{
    UnboundInt **lista = Divide(divident, divisor);
    UnboundInt* q = lista[0];
    FreeUB(lista[1]);
    free(lista);
    return q;
}

UnboundInt* Mod(UnboundInt* divident, UnboundInt* divisor)
{
    UnboundInt **lista = Divide(divident, divisor);
    UnboundInt* mod = lista[1];
    FreeUB(lista[0]);
    free(lista);
    return mod;
}

//remmoveolja az első számot, ha üres a szám akkor törli
void RemovefromStart(UnboundInt* dest)
{

    if(dest->start == NULL)
    {
        free(dest);
        return;
    }
    else if(dest->size == 1)
    {
        free(dest->end);
        dest->end = NULL;
        dest->start = NULL;
        dest->size = 0;
        return;
    }

    dest->size -= 1;
    Uints64s* temp = dest->start->next;
    temp->prev = NULL;
    free(dest->start);
    dest->start = temp;

}

//remmoveolja az utolsó számot, ha üres a szám akkor törli
void RemovefromEnd(UnboundInt* dest)
{

    if(dest->end == NULL)
    {
        free(dest);
        return;
    }
    else if(dest->size == 1)
    {
        free(dest->end);
        dest->end = NULL;
        dest->start = NULL;
        dest->size = 0;
        return;
    }

    dest->size -= 1;
    Uints64s* temp = dest->end->prev;
    temp->next = NULL;
    free(dest->end);
    dest->end = temp;

}

void ConnecttoStart(UnboundInt *dest, uint64_t num)
{
    Uints64s *new = (Uints64s*) malloc(sizeof(Uints64s));

    new->number = num;
    new->prev = NULL;
    new->next = dest->start;

    if(dest->end == NULL)
        dest->end = new;
    else
        dest->start->prev = new;
    dest->start = new;
    dest->size += 1;

}

void PrintLLUasHex(uint64_t num)
{
    char chars[] = "0123456789abcdef";
    char hex[17] = {0};
    uint64_t mask = 0xf000000000000000;
    uint64_t masked;

    for (int i = 0; i < 16; ++i)
    {
        masked = ( mask & num ) >> (60 - i*4);
        hex[i] = chars[masked];
        mask >>= 4;
    }
    printf("%s", hex);
}

bool rekShift(Uints64s* mozgo, bool dir, bool carry_in)
{

    bool carry_over = false;
    if((mozgo->number % 2 == 1) && dir || !dir && ((0x8000000000000000llu & mozgo->number) != 0))
    {
        carry_over = true;
    }



    if(dir)
    {
        mozgo->number >>= 1;
        if(carry_in)
            mozgo->number += 0x8000000000000000llu;
    } else
    {
        mozgo->number <<= 1;
        if(carry_in)
            mozgo->number += 1;
    }



    if((dir? mozgo->next : mozgo->prev) == NULL)
        return carry_over;
    else
        return rekShift(dir? mozgo->next : mozgo->prev, dir, carry_over);
}

//az előjelet nem shifteli
// dir: az irány 1: jobbra, 0: balra, carry_in:a betöltendő szám, 1: 1 0: 0, visszatér a kiesett bittel, helyben végzi el a műveletet
bool Shift(UnboundInt* A, bool dir, bool carry_in)
{
    Uints64s* mozgo;
    bool carry_over = false;



    if(dir)
    {//jobbra shift

        mozgo = A->start;
        while (mozgo != NULL)
        {
            if(mozgo->number % 2 == 1)
                carry_over = true;
            else
                carry_over = false;
            mozgo->number >>= 1;

            if(carry_in)
                mozgo->number += 0x8000000000000000llu;

            carry_in = carry_over;
            mozgo = mozgo->next;
        }


    } else
    {//balra shift

        mozgo = A->end;
        while (mozgo != NULL)
        {
            if((0x8000000000000000llu & mozgo->number) != 0)
                carry_over = true;
            else
                carry_over = false;

            mozgo->number <<= 1;

            if(carry_in)
                mozgo->number += 1llu;

            carry_in = carry_over;
            mozgo = mozgo->prev;
        }

    }



    return carry_over;
}

//visszatér a szorzat vagy osztalék előjelével, fasle: +, true -, és abszolút értékét veszi az argumentumoknak
bool signwork(UnboundInt* A, UnboundInt* B)
{
    bool Aisneg = false;
    bool Bisneg = false;


    if(A->sign)
    {
        TwosComp(A);
        Aisneg = true;
    }

    if(B->sign)
    {
        TwosComp(B);
        Bisneg = true;
    }



    return Aisneg != Bisneg;
}

//balra shifteli a számot, ha kicsúszik akkor 1-est rak az elejére
void GettingBigger(UnboundInt* A, bool carry_in)
{
    if(Shift(A, false, carry_in))
        ConnecttoStart(A, 1llu);
}


// Visszaadja a szorzatot, később fel kell szabadítani, felszabadítja az aurgumentumját
UnboundInt* Multiply(UnboundInt* A, UnboundInt* B)
{
    bool Sign = signwork(A,B);

    UnboundInt* ret = CreateUBint();
    ConnecttoStart(ret, 0llu);

    while(!isZero(B))
    {
        if(Shift(B,true, false))
            Add(ret, A);


        GettingBigger(A,false);


        RemoveZeroFromStart(B);
    }

    FreeUB(B);
    FreeUB(A);

    if(Sign)
        TwosComp(ret);

    return ret;

}

// az első szám helyén végzi el a műveletet
void Subtract(UnboundInt* diff, UnboundInt* toSub)
{
    TwosComp(toSub);
    Add(diff, toSub);
    TwosComp(toSub);
}


//rekurziv összeadás, csak azonos size-u Uints64-et lehet összeadni, a carry vagy 0llu vagy 1llu!
bool addrek(Uints64s* sum, Uints64s* toAdd, uint64_t carry_in)
{

    if(sum == NULL)
        return carry_in == 1llu;

    uint64_t carry_out = 0;

    //tulcsordulás vizsgálat
    if(sum->number > UINT64_MAX - toAdd->number ||
    sum->number > UINT64_MAX - carry_in ||
    toAdd->number > UINT64_MAX - carry_in ||
    sum->number > UINT64_MAX - toAdd->number - carry_in)
    {
        carry_out += 1;
    }


    sum->number += toAdd->number + carry_in;

    addrek(sum->prev, toAdd->prev, carry_out);

}


// az első szám helyén adja össze
void Add(UnboundInt* sum, UnboundInt* toAdd)
{

    unsigned int sizeDiff = abs(sum->size - toAdd->size);
    //kibővítjuk a számokat amivel kell
    if(sum->size > toAdd->size)
    {
        for (int i = 0; i < sizeDiff; ++i) {
            ConnecttoStart(toAdd, toAdd->sign? UINT64_MAX : 0llu);
        }
    }
    else if(sum->size < toAdd->size)
    {
        for (int i = 0; i < sizeDiff; ++i) {
            ConnecttoStart(sum, sum->sign? UINT64_MAX : 0llu);
        }
    }


    bool pluszminusz = sum->sign != toAdd->sign;


    //összeadás
    //----------------------------------------------------------------------------------------------

    uint64_t carry_in = 0;
    uint64_t carry_out;
    Uints64s* mozgoSum = sum->end;
    Uints64s* mozgotoAdd = toAdd->end;

    while (mozgotoAdd != NULL)
    {
        //tulcsordulás vizsgálat
        if(mozgoSum->number > UINT64_MAX - mozgotoAdd->number ||
           mozgoSum->number > UINT64_MAX - carry_in ||
           mozgotoAdd->number > UINT64_MAX - carry_in ||
           mozgoSum->number > UINT64_MAX - mozgotoAdd->number - carry_in)
        {
            carry_out = 1;
        } else
            carry_out = 0;


        mozgoSum->number += mozgotoAdd->number + carry_in;

        carry_in = carry_out;

        mozgoSum = mozgoSum->prev;
        mozgotoAdd = mozgotoAdd->prev;
    }

    bool carry = (carry_out == 1)? true : false;
    //----------------------------------------------------------------------------------------------


    if(pluszminusz && carry)
        sum->sign = false;
    else if(pluszminusz && !carry)
        sum->sign = true;
    else if(!pluszminusz && carry && !sum->sign )// ha pozitívak és van carry
    {
        ConnecttoStart(sum, 1llu);
    }
    else if(!pluszminusz && carry && sum->sign )// ha negatívak és van carry
    {
        ConnecttoStart(sum, UINT64_MAX);
    }
    else if(!pluszminusz && !carry && sum->sign )// ha negatívak és nincs carry
    {
        ConnecttoStart(sum, UINT64_MAX - 1);
    }
}


void FreeUB(UnboundInt* UB)
{
    Uints64s* mozgo = UB->start;
    Uints64s* temp;
    free(UB);
    while (mozgo != NULL) {
         temp = mozgo->next;
         free(mozgo);
         mozgo = temp;
    }
}


void TwosComp(UnboundInt* UB)
{

    //invertálás
    UB->sign = UB->sign? false : true;
    uint64_t mask = UINT64_MAX;
    Uints64s* mozgo = UB->end;
    while (mozgo != NULL)
    {
        mozgo->number = mozgo->number ^ mask;
        mozgo = mozgo->prev;
    }


    //+1
    UnboundInt* uno = CreateUBint();
    ConnecttoStart(uno, 1);
    Add(UB, uno);
    FreeUB(uno);
}



void PrintasHex(UnboundInt* UB)
{
    printf("%c", UB->sign? '-' : '+');

    if(UB->sign)
        TwosComp(UB);

    for (Uints64s* mozgo = UB->start; mozgo != NULL; mozgo = mozgo->next) {
        PrintLLUasHex(mozgo->number);
    }

    printf("\n");
}

bool isZero(UnboundInt* A)
{
    Uints64s* mozgo = A->start;
    while (mozgo != NULL)
    {
        if(mozgo->number != 0llu)
            return false;

        mozgo = mozgo->next;
    }

    return true;
}

//csak pozitiv számokkal működik
bool AgreaterorequalthanB(UnboundInt* A, UnboundInt* B)
{

    //ha valamelyik szám 0, akkor ne törölje őket
    RemoveZeroFromStart(A);
    RemoveZeroFromStart(B);


    if(A->size != B->size)
    {
        return A->size > B->size;
    }


    Uints64s* mozgoA = A->start;
    Uints64s* mozgoB = B->start;
    while (mozgoA != NULL)
    {
        if(mozgoA->number > mozgoB->number)
            return true;
        else if (mozgoA->number < mozgoB->number)
            return false;


        mozgoA = mozgoA->next;
        mozgoB = mozgoB->next;
    }

    return true;
}



bool isPrime(UnboundInt* M, unsigned short number_of_witnesses)
{
    //0. lépés: a létezik? és a szám páros?
    //-----------------------------------------------------------
    if(M->size == 0)
        return false;
    else if ((M->end->number & 1llu) == 0)
        return false;
    else if(M->end->number < 2 && M->size == 1)
        false;

    RemoveZeroFromStart(M);
    //-----------------------------------------------------------






    //1. lépés k és m egészek kiszámítása
    //------------------------------------------------------------
    uint64_t k = 0;
    UnboundInt* c = Copy(M);

    //kivonunk egyet, de mivel tudjuk hogy páratlan ez okosan csináljuk
    c->end->number -= 1;



    do{
        Shift(c, true, false);
        ++k;
    } while ((c->end->number & 1llu) == 0llu);


    //------------------------------------------------------------




    //seed Mersene Twister
    srand(time((NULL)));
    MTRand r = seedRand(rand());
    genRandLong(&r);



    for (int i = 1; i <= number_of_witnesses; ++i)
    {


        //2.lépés generálunk egy 1 <= a <= m - 1 számot
        //------------------------------------------------------------

        UnboundInt* a = CreateUBint();
        do {

            if(a->start != NULL)
            {
                FreeUB(a);
                a = CreateUBint();
            }



            uint64_t Size = genRandLong(&r) % (M->size);
            if(Size == 0llu)
                Size = 1;

            for (int j = 0; j < Size; ++j) {
                ConnecttoStart(a, ((unsigned long long)genRandLong(&r) << 32) ^ (unsigned long long) genRandLong(&r));
            }

        }while (AgreaterorequalthanB(a, M));


        //------------------------------------------------------------






        //3.lépés tesztelés
        //------------------------------------------------------------

        if(!Miller_Rabin(M,c,k,a))
        {
            FreeUB(c);
            return false;
        }

        //------------------------------------------------------------
    }

    FreeUB(c);
    return true;

}

//felszabadítja a random számot amit kap
bool Miller_Rabin(UnboundInt* M, UnboundInt* c, uint64_t k, UnboundInt* a)
{

    // M a prím amit tesztelünk
    // c = (M-1) / 2^k (páratlan)
    // k = log2((M-1)/c)
    // 1< a <M random szám


    UnboundInt* Mcopy = Copy(M);
    UnboundInt* Ccopy = Copy(c);
    UnboundInt* x = A_to_the_B_Mod_M(a, Ccopy, Mcopy);


    UnboundInt* Xcopy1;
    UnboundInt* Xcopy2;
    UnboundInt* y;

    UnboundInt* uno = CreateUBint();
    ConnecttoStart(uno, 1llu);

    UnboundInt* minusUno = Copy(M);
    minusUno->end->number -= 1;

    for (int i = 0; i < k; ++i)
    {

        Xcopy1 = Copy(x);


        Xcopy2 = Copy(Xcopy1);


        y = Multiply(Xcopy1, Xcopy2);
        Mcopy = Copy(M);
        y = Mod(y,Mcopy);


        //teszt
        if(CMP(y,uno) && !CMP(x, uno) && !CMP(x, minusUno))
        {
            FreeUB(uno);
            FreeUB(minusUno);
            FreeUB(x);
            FreeUB(y);
            return false;
        }



        FreeUB(x);
        x = y;
    }

    bool itsprobablyprime = CMP(y,uno);


    FreeUB(uno);
    FreeUB(minusUno);
    FreeUB(y);


    return itsprobablyprime;
}

//true ha egyenlőek, false, ha nem.
bool CMP(UnboundInt* A, UnboundInt* B)
{
    RemoveZeroFromStart(A);
    RemoveZeroFromStart(B);


    //ha nem ugyan akkorák akkor gatya
    if(A->size != B->size)
        return false;


    Uints64s* mozgoA = A->start;
    Uints64s* mozgoB = B->start;
    while (mozgoA != NULL)
    {
        if(mozgoA->number != mozgoB->number)
            return false;

        mozgoA = mozgoA->next;
        mozgoB = mozgoB->next;
    }

    return true;

}


//felszabadítja mind a 3 argumentumát
UnboundInt* A_to_the_B_Mod_M(UnboundInt* A, UnboundInt* B, UnboundInt* M)
{

    UnboundInt* current;
    UnboundInt* sofar;


    if(!isZero(A))
        RemoveZeroFromStart(A);

    if(!isZero(B))
        RemoveZeroFromStart(B);

    RemoveZeroFromStart(M);




    UnboundInt* Mcopy = Copy(M);
    current = Mod(A, Mcopy);



    if(Shift(B, true, false))
    {
        sofar = Copy(current);
    }
    else
    {
        sofar = CreateUBint();
        ConnecttoStart(sofar, 1llu);
    }


    RemoveZeroFromStart(B);



    while(!isZero(B))
    {
        UnboundInt* currentCopy = Copy(current);
        current = Multiply(current, currentCopy);

        Mcopy = Copy(M);
        current = Mod(current, Mcopy);


        if(Shift(B, true, false))
        {
            currentCopy = Copy(current);
            sofar = Multiply(sofar, currentCopy);

            Mcopy = Copy(M);
            sofar = Mod(sofar, Mcopy);
        }


        RemoveZeroFromStart(B);
    }

    FreeUB(B);
    FreeUB(M);
    FreeUB(current);

    return sofar;

}

//generates a prime with the given number of digits in hex
//fel kell szabadítani, legalább 16 számjegyű prímet tud generálni
UnboundInt* GenerateBigPrime(uint64_t digits, unsigned long seed)
{

    //seed Mersene Twister
    MTRand r = seedRand(seed);
    srand(time(NULL));
    unsigned int length = rand() % 100;
    for (int i = 0; i < length; ++i) {
        genRandLong(&r);
    }


    uint64_t buffer = 0;
    uint64_t random = genRandLong(&r);
    uint64_t mask = 0xf;
    UnboundInt* ret = NULL;


    do {

        if(ret != NULL)
            FreeUB(ret);
        ret = CreateUBint();

        uint64_t wholes = digits / 16;
        uint64_t last = digits % 16;


        for (int i = 0; i < wholes; ++i)
        {
            ConnecttoStart(ret, ((unsigned long long)genRandLong(&r) << 32) ^ (unsigned long long) genRandLong(&r));
        }


        if(last != 0)
        {
            random = ((unsigned long long)genRandLong(&r) << 32) ^ (unsigned long long) genRandLong(&r);
            for (int i = 0; i < last; ++i)
            {
                buffer += random & mask;
                buffer <<= 4;
                random >>= 4;
            }
            ConnecttoStart(ret, buffer);
        }

    } while (!isPrime(ret, 5));

    return ret;
}

//fel kell szabadítani a listát, amit ad
UnboundInt** Generate_512bitRSAKey(unsigned long seed1, unsigned long seed2)
{
    srand(time((NULL)));
    int Qsize;
    int Psize;

    do {
        Qsize  = rand() % 80;
    }while (Qsize < 60);

    Psize = 128 - Qsize;



    UnboundInt* P = GenerateBigPrime(Psize, seed1);
    UnboundInt* Q = GenerateBigPrime(Qsize, seed2);

    UnboundInt** keys = (UnboundInt**)malloc(sizeof(UnboundInt*) * 3);
    keys[0] = Copy(P);
    keys[1] = Copy(Q);
    keys[2] = Multiply(P, Q);
    return keys;
}

//fel kell szabadítani, felszabadítja az argumentumokat
UnboundInt* LNKO(UnboundInt* A, UnboundInt* B)
{
    if(CMP(A,B))
    {
        FreeUB(B);
        return A;
    }


    if(!AgreaterorequalthanB(A, B))
    {
        UnboundInt* temp = A;
        A = B;
        B = temp;
    }


    //A nagyobb mint B

    UnboundInt* x = Copy(B);
    UnboundInt* y = Mod(A,B);
    UnboundInt* temp;
    while (!isZero(y))
    {
        temp = Copy(y);
        y = Mod(x, y);
        x = temp;
    }

    FreeUB(y);
    return x;
}


//csak pozitív számokra működik, felszabadítja az argumentumokat, visszadja egy tömben: x-et, és m/(m,a)-t;
//fel kell szabadítani
UnboundInt** SolveLinCong(UnboundInt* a, UnboundInt* b, UnboundInt* m)
{
    UnboundInt* ACopy = Copy(a);
    UnboundInt* MCopy = Copy(m);
    UnboundInt* lnko = LNKO(ACopy, MCopy);

    //ha m egy töbszöröse a-nak akkor nincs megoldás
    if(CMP(lnko, a))
    {
        FreeUB(a);
        FreeUB(b);
        FreeUB(m);
        FreeUB(lnko);
        return NULL;
    }



    UnboundInt* uno = CreateUBint();
    ConnecttoStart(uno, 1llu);

    //relatív prímek?
    if(!CMP(lnko, uno))
    {
        //ha nem:
        UnboundInt* lnkoCopy = Copy(lnko);
        UnboundInt** b_divided_lnko = Divide(b, lnkoCopy);


        // ha nem osztahtó az lnko-val akkor nincs megoldás
        if(!isZero(b_divided_lnko[1]))
        {
            FreeUB(uno);
            FreeUB(lnko);
            FreeUB(a);
            FreeUB(m);
            FreeUB(b_divided_lnko[0]);
            FreeUB(b_divided_lnko[1]);
            free(b_divided_lnko);
            return NULL;
        }

        b = b_divided_lnko[0];

        lnkoCopy = Copy(lnko);
        a = DivideWhole(a, lnkoCopy);
        m = DivideWhole(m, lnko);
        FreeUB(b_divided_lnko[1]);
        free(b_divided_lnko);

    } else
        FreeUB(lnko);



    UnboundInt* baltemp;
    UnboundInt* jobbtemp;
    UnboundInt* baloldal_elozo = a;
    UnboundInt* jobboldal_elozo = Copy(b);
    UnboundInt* baloldal;
    UnboundInt* jobboldal;

    //0. a q, 1. a maradék
    UnboundInt** div = Division(m, a);

    //baloldal
    baloldal = div[1];

    //jobboldal
    jobboldal = Multiply(div[0], b);
    TwosComp(jobboldal);
    free(div);



    while (!CMP(baloldal, uno))
    {
        jobbtemp = Copy(jobboldal);
        baltemp = Copy(baloldal);

        div = Divide(baloldal_elozo, baloldal);

        baloldal = div[1];

        jobboldal = Multiply(div[0], jobboldal);
        Subtract(jobboldal_elozo, jobboldal);
        FreeUB(jobboldal);
        jobboldal = jobboldal_elozo;

        baloldal_elozo = baltemp;
        jobboldal_elozo = jobbtemp;

        free(div);
    }



    if(jobboldal->sign)
    {
        MCopy = Copy(m);
        jobboldal = Mod(jobboldal, MCopy);
        MCopy = Copy(m);
        Subtract(MCopy,jobboldal);
        FreeUB(jobboldal);
        jobboldal = MCopy;
    }


    FreeUB(baloldal);
    FreeUB(baloldal_elozo);
    FreeUB(jobboldal_elozo);
    FreeUB(uno);

    UnboundInt** ret = (UnboundInt**) calloc(2, sizeof(UnboundInt*));
    MCopy = Copy(m);
    ret[0] = Mod(jobboldal, MCopy);
    MCopy = Copy(m);
    FreeUB(m);
    ret[1] = MCopy;
    return ret;

}


UnboundInt* Plus(UnboundInt* A, UnboundInt* B)
{
    UnboundInt* Acopy = Copy(A);
    Add(Acopy, B);
    return Acopy;
}

UnboundInt* Minus(UnboundInt* A, UnboundInt* B)
{
    UnboundInt* Acopy = Copy(A);
    Subtract(Acopy, B);
    return Acopy;
}

UnboundInt* Times(UnboundInt* A, UnboundInt* B)
{
    UnboundInt* Acopy = Copy(A);
    UnboundInt* BCopy = Copy(B);
    return Multiply(Acopy, BCopy);
}

UnboundInt* Quotient(UnboundInt* A, UnboundInt* B)
{
    UnboundInt* Acopy = Copy(A);
    UnboundInt* BCopy = Copy(B);
    return DivideWhole(Acopy, BCopy);
}

UnboundInt* Negative(UnboundInt* A)
{
    UnboundInt* Acopy = Copy(A);
    TwosComp(Acopy);
    return Acopy;
}

UnboundInt* GCD(UnboundInt* A, UnboundInt* B)
{
    UnboundInt* Acopy = Copy(A);
    UnboundInt* BCopy = Copy(B);
    return LNKO(Acopy, BCopy);
}

//only with positive numbers
UnboundInt* Modulo(UnboundInt* A, UnboundInt* B)
{
    UnboundInt* Acopy = Copy(A);
    UnboundInt* BCopy = Copy(B);
    return Mod(Acopy, BCopy);
}

UnboundInt** Division(UnboundInt* A, UnboundInt* B)
{
    UnboundInt* Acopy = Copy(A);
    UnboundInt* BCopy = Copy(B);
    return Divide(Acopy, BCopy);
}


UnboundInt** Generate_RSA_Keyspairs(unsigned long seed1, unsigned long seed2, UnboundInt* public_Key)
{
    UnboundInt* min = CreateUBint();
    ConnecttoStart(min, 1000llu);

    if(AgreaterorequalthanB(min, public_Key))
    {
        FreeUB(min);
        return NULL;
    }
    FreeUB(min);

    UnboundInt** modulo = Generate_512bitRSAKey(seed1, seed2);

    modulo[0]->end->number -= 1;
    modulo[1]->end->number -= 1;

    UnboundInt* phiN = Multiply(modulo[0], modulo[1]);

    UnboundInt* uno = CreateUBint();
    ConnecttoStart(uno, 1llu);
    UnboundInt* pkc = Copy(public_Key);
    UnboundInt* phiNCopy = Copy(phiN);
    UnboundInt** secret_key = SolveLinCong(public_Key,uno, phiNCopy);

    if(!CMP(secret_key[1], phiN))
    {
        FreeUB(phiN);
        FreeUB(secret_key[0]);
        FreeUB(secret_key[1]);
        free(secret_key);
        FreeUB(modulo[2]);
        free(modulo);
        return NULL;
    }

    FreeUB(secret_key[1]);
    FreeUB(phiN);
    modulo[0] = secret_key[0];
    modulo[1] = pkc;
    free(secret_key);
    return modulo;

}

int main() {


    UnboundInt* publickey = CreateUBint();
    ConnecttoStart(publickey, 1667);

    UnboundInt** RSA = Generate_RSA_Keyspairs(8492, 13845, publickey);//random seedek

    PrintasHex(RSA[0]);
    PrintasHex(RSA[1]);
    PrintasHex(RSA[2]);

    FreeUB(RSA[0]);
    FreeUB(RSA[1]);
    FreeUB(RSA[2]);
    free(RSA);

    return 0;
}
