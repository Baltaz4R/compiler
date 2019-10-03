# PROJEKTNI ZADATAK POČEV OD JUNA 2019.

# Napomene

Rok za predaju rešenja projektnog zadatka je nekoliko dana pred termin ispita. Način predaje
rešenja i termin odbrane biće naknadno objavljeni. Rešenje se sastoji od sledećih stavki:

1. dokumentacija koja sadrži opis rešenja kao i uputstvo za prevođenje i pokretanje čitavog sistema,

2. testovi (ulazni fajlovi) koji prikazuju sve funkcionalnosti realizovanog sistema,

3. izvorni kod rešenja i

4. izvršni kod rešenja.

Obaveštenje o početku i pravilima predaje kao i tačan termin odbrane rešenja projektnog zadatka biće blagovremeno poslati na mejling listu predmeta (13e113ss@lists.etf.rs, 13s113ss@lists.etf.rs). Prilikom odbrane biće korišćena verzija rešenja projektnog zadatka koja je predata i neće biti dozvoljene naknadne izmene. Nepoštovanje pravila u vidu propusta u okviru dokumentacije i testova uzrokuje dobijanje manjeg broja poena.

# Opis okruženja

Izrada rešenja projektnog zadatka radi se pod operativnim sistemom Linux na x86 arhitekturi
u programskim jezicima C, C++ ili asembleru (moguća je kombinacija). Potrebni alati su opisani u
okviru materijala za predavanja i vežbe.

Za one koji nemaju instaliran Linux ponuđena je instalacija u okviru virtuelne mašine VMware Player. Virtuelna mašina se može preuzeti u paviljonu Rašović. U okviru ove virtuelne mašine već su instalirani potrebni alati. Odbrana rešenja projektnog zadatka vrši se isključivo pod prethodno opisanim okruženjem u okviru pomenute virtuelne mašine.

# Zadatak – Asembler (25 bodova)

Napisati dvoprolazni asembler za procesor opisan u prilogu. Svi potrebni argumenti asemblera zadaju se kroz komandnu liniju prilikom pokretanja. Ulaz asemblera je tekstualni fajl u skladu sa sintaksom opisanom u nastavku. Za potrebe leksičke analize ulaznog tekstualnog fajla dozvoljeno je koristiti generatore leksera. Izlaz asemblera treba da bude predmetni program zapisan u tekstualnom fajlu. Za potrebe učitavanja u emulator dozvoljeno je generisati i binarni fajl pored tekstualnog.

Format predmetnog programa bazirati na školskoj varijanti ELF formata (tekstualni fajl kakav je korišćen u zadatku 9 u prezentaciji V3_Konstrukcija asemblera.ppt) i predložiti izmene u formatu u skladu sa potrebama ciljne arhitekture (nove sekcije, novi tipovi zapisa o relokacijama, dodatna polja u postojećim tipovima zapisa, novi podaci o predmetnom programu i slično).

Prilikom generisanja izlaza asemblera voditi se principima koje koristi GNU asembler. Sve sekcije smeštaju se počev od nulte adrese. Sintaksa asemblera i ostali zahtevi:

- jedna linija izvornog koda sadrži najviše jednu asemblersku naredbu/direktivu,

- labela, koja se završava dvotačkom, se može naći na početku linije izvornog koda nakon proizvoljnog broja belih znakova,

- labela može da stoji samostalno, bez prateće asemblerske naredbe/direktive u istoj liniji izvornog koda, što je ekvivalentno tome da stoji u liniji izvornog koda sa prvim sledećim sadržajem,

- simboli se izvoze i uvoze navođenjem asemblerskih direktiva .global <ime_simbola> i .extern <ime_simbola>, respektivno, pri čemu u okviru jedne direktive može da se navede i više simbola razdvojenih zapetama,

- direktiva ekvivalencije .equ <ime_simbola>, < vrednost> uvodi simbol sa datom vrednošću,

- celokupan izvorni kod je podeljen na sekcije definisane pomoću sledećih asemblerskih direktiva:
    - .text – sekcija sa mašinskim kodom,
    - .data – sekcija za podatke sa inicijalnim vrednostima,
    - .bss – sekcija za podatke bez inicijalnih vrednosti,
    - .section <ime_sekcije> [, "< flegovi>"] – sekcija sa navedenim proizvoljnim imenom (datu asemblersku direktivu je moguće navesti više puta sa različitim imenom),

- fajl sa izvornim kodom se završava (ostatak fajla se odbacuje tj. ne prevodi se) pomoću asemblerske direktive .end,

- asemblerske direktive .byte, .word, .align i .skip imaju identične funkcionalnosti kao u okviru GNU asemblera,

- ostatak sintakse, ukoliko nije definisan u prilogu, definisati po sopstvenom nahođenju.

Primer komande, kojom se pokreće asembliranje izvornog koda u okviru fajla _ulaz1.s_, dat je u nastavku: asembler -o ulaz1.o ulaz1.s
