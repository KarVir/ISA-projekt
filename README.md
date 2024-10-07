##### autor Karolína Virágová
##### login xvirag00
##### login 29.9. 2024

## Popis programu
Program zobrazí aktuální přenosové rychlosti pro jednotlivé komunikující ip adresy. Program po spuštění začne zachytávat provoz na síťovém rozhraní pomocí knihovny libpcap a počítat přenosovou rychlost v bitech/sekundu a paketech/sekundu.

Je implementován přepínač -t, kterým si zvolíme časový interval zobrazeni statistik.


## Příklady spuštění
```
./isa-top -i wlp3s0 
./isa-top -i wlp3s0 -s b
./isa-top -i wlp3s0 -s b -t 2
./isa-top -i wlp3s0 -s p 
./isa-top -i wlp3s0 -s p -t
```
## Odevzdané soubory
* Makefile - překlad c souboru na spustitelný soubor isa-top
* isa-top.c - hlavni soubour programu,
* packet_handler.c/h
* display,c/h
* vector.c/h
* speed.c/h
* sorting.c - pomocné funkce pro řadísí funkci gsort
* LICENSE


# Smazat
soubor README obsahující jméno a login autora, datum vytvoření, krátký textový popis programu s případnými rozšířeními či omezeními, příklad spuštění a seznam odevzdaných souborů,

https://liw.fi/manpages/

https://pdw.ex-parrot.com/iftop/


TODO
b/s p/s

https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/


https://github.com/ondryaso/leoAtFit