## Jak symulować płytkę:

- Pobieramy quartus (najlepiej 17.1)
- Odpalamy eclipse(nios II)
- Nowy projekt
    - Nios II aplication and bsp from template
    - Wybieramy .sopcinfo to powyżej Computer_System.sopcinfo
    - Jako szablon OSII hello world (od lab4)



- Edytujemy kod
    - Dajemy swoje ale dodajemy redefinicje adresów bazowych
```
#define HEX_BASE HEX3_HEX0_BASE
#define SW_SLIDERS_BASE SLIDER_SWITCHES_BASE
```

- Kompilujemy projekt do *.elf (Project > build all)
- Elfa przenosimy np. na pulpit
- Wbijamy na stronę: https://cpulator.01xz.net/?sys=nios-de1soc
- Ładujemy binarkę File > Load ELF
- W lewym dolnym rogu w ustawieniach wyłączamy wszystkie debugging checks
- Kontynułujemy (F3)
- Gotowe 


## Symulacja OSII / OSIII w WINDOWS

- Bierzemy kod od chińczyka: https://github.com/John-Yu/ucos-vs
- Odpalamy solution w vs studio
- Bawimy się z kodem
- Polecam poprzednią metodę
