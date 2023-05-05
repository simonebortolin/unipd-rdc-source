Esercizio di Reti di Calcolatori

Modificare cw.c in modo che possa inviare attraverso il message body del metodo POST (v. RFC 2616 Cap. 4) e la codifica `multipart/form-data` (v. RFC 1867 e 7578).

Utilizzare il server web che risponde alla pagina web https://httpbin.org/anything (e/o se preferisci il sw-post-multipart) per inviare dei test con il multipart ed il metodo POST vedere la relativa risposta.

I nomi dei file da inviare devono inviare devono essere presi da args, se non vengono passati dei dati necessari il invia la richeista senza i suddetti dati. Il client stampa poi il risultato ricevuto dal server.

Prestare attenzione alla RFC 2616 Cap. 4.4 relativamente al message length, nel caso di invio di più file potrebbe essere necessario implementare il transfer-encoding chunk.