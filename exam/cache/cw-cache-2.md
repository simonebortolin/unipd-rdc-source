# Esame di Reti di Calcolatori - 30 Agosto 2018

Si modifichi il programma web client wc18.c in modo tale che sia in grado di gestire il meccanismo di controllo
del caching basato sugli header `Last-Modified` e `If-Modified-Since` descritti nella RFC 2616 ai capitoli 14.29,
14.25.

Il programma wc18.c modificato, per la verifica del corretto funzionamento, dovrà essere azionato due volte. La
prima volta, a cache vuota, dovrà acquisire la data di ultima modifica della risorsa e la salverà insieme al entity
body sulla cache (file su disco).
La seconda volta (e le successive) ri-scaricherà la risorsa solo se è questa è stata modificata nel frattempo sul
server, altrimenti dovrà accedere alla copia in cache.
