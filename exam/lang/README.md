L'header `Accept-Language` dell'HTTP è una richiesta verso il server web che indica la lingua naturale e le impostazioni locali preferite dal client. Il server utilizza la negoziazione del contenuto per selezionare una delle proposte e informa il cliente della scelta con l'header `Content-Language` nella risposta.

Gli header sono descritti nella RFC 2616 sezione 14.4 e 14.12. L'header `Accept-Language` accetta un quality value nel range tra 0 e 1.
`Accept-Language: da, en-gb;q=0.8, en;q=0.7` significa 
> I prefer Danish, but will accept British English and other types of English.