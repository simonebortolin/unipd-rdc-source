# Esercizio di Reti di Calcoaltori

Si modifichi il programma che implementa il Web Client in modo che legga un entity body utilizzando il transfer coding chuncked, come descritto dalla grammatica riportata nella sezione 3.6.1 della RFC 2616, evitando tutte le parti opzionali.

```
Chunked-Body   = *chunk
                        last-chunk
                        trailer
                        CRLF

       chunk          = chunk-size [ chunk-extension ] CRLF
                        chunk-data CRLF
       chunk-size     = 1*HEX
       last-chunk     = 1*("0") [ chunk-extension ] CRLF

       chunk-extension= *( ";" chunk-ext-name [ "=" chunk-ext-val ] )
       chunk-ext-name = token
       chunk-ext-val  = token | quoted-string
       chunk-data     = chunk-size(OCTET)
       trailer        = *(entity-header CRLF)

```
