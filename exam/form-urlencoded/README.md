# Form URL-encoded

Form URL-encoded è una codifica per trasmettere delle particolari query (coppie nome/valore) ad un server web, esse sono separate da un `=` e concatenate da un `&`. Il mimetype è `application/x-www-form-urlencoded`, non permettono la definizione di tipi di dato complessi e permettono la codifica di file tramite esclusivamente Base64.

Numerosi caratteri speciali e non vengono codificati, in accordo alle specifiche RFC 1866 Cap. 8.2:

> [Reserved and] non-alphanumeric characters are replaced by `%HH', a percent sign and two hexadecimal digits representing the ASCII code of the character


Un message body con `application/x-www-form-urlencoded` è il seguente:

```
Content-Type: application/x-www-form-urlencoded
Content-Length: 51

text1=text+default&text2=a%CF%89b&file1=a.txt&file2=a.html&file3=binary
```

Per quanto riguarda il campo di testo, vediamo che i soliti caratteri stampabili come ae bvenivano inviati in un byte, mentre quelli non stampabili come 0xCFe 0x89occupavano 3 byte ciascuno: %CF%89!