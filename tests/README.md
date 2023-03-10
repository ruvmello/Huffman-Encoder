# Testen

### Functie testen

In src vind je een folder tests. In tests.c vind je verschillende testen 
die vooral testen of de boom werkt. Om deze testen uit te voeren
geef je -t mee bij het uitvoeren van het programma.
Dit doe je in plaats van -c of -d.

Voorbeeld:
    ./persen -t

### Volledige testen

Om te testen of het programma werkt, vind je in deze map een bash script nl. integration.sh.
Deze neemt 1 parameter naar het pad van het gecompileerde bestand en voert vervolgens verschillende  tests uit.

Voorbeeld: 
    ./integration ../persen

In bovenstaand voorbeeld zal dus ../persen gebruikt worden om de testen uit te voeren.
