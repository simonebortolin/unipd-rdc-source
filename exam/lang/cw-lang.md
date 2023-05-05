Esercizio di Reti di Calcolatori

Il sito web https://stem.elearning.unipd.it/ supporta l'utilizzo dell'header `Accept-Language` per selezionare la lingua se italiano o inglese. Dato che il sito web, non supporta HTTP ma solo HTTPS si modifichi il file [cw-ssl.c](../ssl-tls/cw-ssl.c) affinché supporti l'invio dell'header `Accept-Language`. Controllare che l'header di risposta sia corretto  `Content-Language`.

Gli header sono descritti nella RFC 2616 sezione 14.4 e 14.12. L'header `Accept-Language` accetta un quality value nel range tra 0 e 1.
`Accept-Language: da, en-gb;q=0.8, en;q=0.7` significa 
> I prefer Danish, but will accept British English and other types of English.

