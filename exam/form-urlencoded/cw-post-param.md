Esercizio di Reti di Calcolatori

Modificare cw.c in modo che possa inviare attraverso il message body del metodo POST (v. RFC 2616 Cap. 4) e la codifica detta www-form-urlencoded (v. RFC 1866 Cap. 8.2) dei parametri per riempire il seguente form https://httpbin.org/forms/post:
- custname
- custtel
- custemail
- size 
- topping
- delivery
- comments

I dati da inviare devono essere presi da args, se non vengono passati dei dati necessari il invia la richeista senza i suddetti dati. Il client stampa poi il risultato ricevuto dal server.

Prestare attenzione ad eventuali codifiche ed escape di caratteri speciali e prestare attenzione alla RFC 2616 Cap. 4.4 relativamente al message length (non è necessario implementare il Transfer-Encoding per il Message Body).