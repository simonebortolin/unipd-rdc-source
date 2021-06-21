# Come funziona il Keep Alive

Il Keep Alive fa un modo che in una unica connessione il socket rimane in vita e permette di inviare più pagine, sullo stesso socket senza dover rinegoziare una connessione, in Keep alive ci sono meno aperture di socket e una unica connessione che viene usata per scaricare più elementi senza dover riconnetersi

![Connessioni Multiple e Keep Alive](keep-alive.png "Connessioni Multiple e Keep Alive")

Affinché sia possibile avere una connessione Keep Alive o Persistente  è necessario che il server utilizzi un sistema per determinare la fine di una richiesta e l'inizio della successiva diversa dalla chiusura della connessione. Le principali strategie sono
* `Content-Length`: nel caso di file generati dinamicamente richiede un aver generato tutta la pagina prima di poterla inviare, richiede una dimensione del buffer molto elevata
* `Transfer-Encoding: chunked`: è una variante del `Content-Length` che invia tanti blocchi detti chunked fino a fine pagina, richiede una dimensione del buffer ridotta.
Un esempio di chunked è:
```
4\r\n        (bytes to send)
Wiki\r\n     (data)
6\r\n        (bytes to send)
pedia \r\n   (data)
E\r\n        (bytes to send)
in \r\n
\r\n
chunks.\r\n  (data)
0\r\n        (final byte - 0)
\r\n         (end message)
```
Che decodificati porta al messaggio:
```
Wikipedia in

chunks.
```

La somma dei vari chunk-size è la `Content-Length` della pagina. Nel caso in cui la fine di un file viene determinata dalla chiusura del socket, tipico di HTTP/1.0 e 0.9 non è possibile in nessun modo fare richieste multiple, questo vare anche nel caso di inoltro di richieste, come nei Proxy, in quanto la connessione viene brutalmente chiusa.

P.S. se hai dubbi su questa affermazione e se pensi che sia una cosa trovata su stackowerflow fatti questa domanda: *Come è possibile determinare quando finisce una richiesta e quando inizia quella successiva senza un delimitatore?*

N.B: nelle implementazioni di server e proxy keep alive è **importante** farlo **multithread** in quanto la connessione è persistente e se non fosse **multithread** fino a chiusura della connessione nuove richieste sarebbero in attesa.

P.S. se hai dubbi sull'affermazione del N.B. prova ad implementarlo senza thread e fai una richiesta da due browser diversi e poi vedi se funziona.





