HTTP/1.1 utilizza un meccanismo di caching più flessibile e sofisticato rispetto alla versione precedente. In sostanza, l'idea di base è che le risorse richieste dal client vengono memorizzate nella cache del server o del client stesso, in modo che possano essere riutilizzate in futuro senza dover essere scaricate nuovamente dalla rete. Ciò può ridurre significativamente il traffico di rete e migliorare le prestazioni complessive del sistema.

L'RFC 2616 descrive in dettaglio come il caching funziona all'interno di HTTP/1.1, inclusi i vari header HTTP utilizzati per controllare il comportamento della cache. In particolare, i capitoli 13 e 14 forniscono informazioni su come i client e i server devono gestire i dati della cache.

Non c'è una unico tipo di HTTP Cahce, ma è un insieme di header e regole descritte nei capitoli 13 e 14 della RFC, in parrticolare, i principali Header sono:

- Request header like `If-None-Match` and `If-Modified-Since`
- Response header like `Cache-Control`, `ETag`, `Last-Modified`

più informazioni su https://web.dev/http-cache/



