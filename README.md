# RSA

cmake project, clion autamtikusan felismeri és tudja fordítani

A program executeable-je mellett kell lennie egy „Encrypted” és egy „Deciphered” mappának, ha ezek nincsenek, le kell futtatni egyszer a programot és létrehozza őket.
Ha titkosítani szeretnénk fileokat, másoljuk őket a Deciphered mappába, ha visszafejteni akkor az Encrypted mappába.
Elindításkor 1-est kell nyomnunk, ha titkosítani szeretnénk,
2-est, ha visszafejteni.
•
Ha egyest nyomtunk ismét 1-est vagy 2-est kell nyomnunk:
o
1-est, ha meglévő kulccsal szeretnénk titkosítani
o
2-est ha generálni szeretnénk új kulcsokat:
▪
Ha új kulcsot generálunk, meg kell adni a mappa elérési útvonalát, ahol tárolni szeretnénk a kulcsokat, és 4 számot „Enter”-rel elválasztva.
▪
Az első 3 szám a seed-ek, a 4. pedig a modulus nagysága bit-ben tehát ha nagy számot ütünk be akkor nem a kávé fog kihülni hanem az univerzum.
▪
A 4. szám legyen maximum 1024, annál nagyobbat még nem generáltam vele és nem tudom mennyi idő lenne, valamint az RSA és a beolvasó algoritmus miatt 64-el osztható számnak kell lennie
▪
A generálás hossza függ a seedektől is, szóval ha sok időbe telik egy bizonyos seeddel a generálás lehet jobban megéri beírni újakat és előről kezdeni
▪
Ha a megadott mappa nem létezik, akkor létrehozza
•
A következő lépés, ha Nem generáltunk új kulcsokat, meg kell adni az abszolút elérési útvonalat a kulcsokat tartalmazó mappához
Ha a titkosítandó vagy visszafejtendő file nagyobb volt mint a modulus 1-essel lép ki a program
Ha az elérési útvonal a kulcsokhoz nem helyes akkor is 1-essel lép ki a program
Ha 0-val lép ki akkor a titkosított vagy visszafejtett fileok megjelennek a megfelelő mappában
Előre generáltam egy 512 bites és egy 1024 bites kulcsot, hogy ne legyen sok idő tesztelni