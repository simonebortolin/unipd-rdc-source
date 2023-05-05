# Esercizio di Reti di Calcolatori

Modificare cw.c affiche si possano passare parametri attraverso la codifica detta www-form-urlencoded nelle query string degli URL (v. RFC 1866 Cap. 8.2).

Utilizzare http://httpbin.org/anything per i propri test. Il programma chiede gli argomenti da passare tramite la riga di comando, in particolare i parametri in ordine sono:
- custname
- custtel
- custemail
- size 
- topping
- delivery
- comments

I dati da inviare devono essere presi da args, se non vengono passati dei dati necessari il invia la richeista senza i suddetti dati. Il client stampa poi il risultato ricevuto dal server.

Prestare attenzione ad eventuali codifiche ed escape di caratteri speciali.