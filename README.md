# Aplikace pro zachytávání paketů
##### autor Karolína Virágová
##### datum 29.9. 2024

## Popis programu
Program zobrazí aktuální přenosové rychlosti pro jednotlivé komunikující ip adresy. Program po spuštění začne zachytávat provoz na síťovém rozhraní pomocí knihovny libpcap a počítat přenosovou rychlost v bitech/sekundu a paketech/sekundu.

Je implementován přepínač -t, kterým si zvolíme časový interval zobrazeni statistik.


## Příklady spuštění
### Spuštění aplikace
```
./isa-top -i wlp3s0 
./isa-top -i wlp3s0 -s b
./isa-top -i wlp3s0 -s b -t 2
./isa-top -i wlp3s0 -s p 
./isa-top -i wlp3s0 -s p -t
```
### Spuštění manuálu isa-top.1
```
man ./isa-top.1
```


https://liw.fi/manpages/

https://pdw.ex-parrot.com/iftop/


https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/


https://github.com/ondryaso/leoAtFit