# Esercizio di Reti di Calcolatori

Si implementi un server web che alla richiesta della pagina / decida attraverlo l'utilizzo dell'header `Accept-Language` di mostrare il file `lang.html` corrispondente, e se non esiste mandare in output `en.html`, il server deve supportare almeno 4 lingue a vostra scelta.

Gli header sono descritti nella RFC 2616 sezione 14.4 e 14.12. L'header `Accept-Language` accetta un quality value nel range tra 0 e 1.
`Accept-Language: da, en-gb;q=0.8, en;q=0.7` significa 
> I prefer Danish, but will accept British English and other types of English.

