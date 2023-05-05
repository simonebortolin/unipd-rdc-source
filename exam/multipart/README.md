# Multipart

`multipart/form-data` entra in gioco quando la codifica `application/x-www-form-urlencoded` definita in RFC 1866 Cap. 8.2 non è più sufficente, cioè per caricare i File, in quanto la codifica Base64 è nettamente inefficente, la prima specifica è nella RFC 1867, modificato in 2388 e 7578. Il meccanismo Multipat originariamente è stato ideato per inviare gli allegati nelle e-mail.

Con questo metodo di trasmissione al posto di trasmettere coppie nome/valore si trasmettono coppie nome/parte, ed ogni parte ci ha un proprio MIME. Le parti sono separate da un particolare stringa, boundary, (scelta in modo specifico in modo che questa stringa non si verifichi in nessuno dei payload delle varie parti e questo potrebbe rappresentare un problema nei file binari ma questo non è un problema per `application/x-www-form-urlencoded` e la codifica in Base64). Ogni parte ha il proprio set di intestazioni MIME come `Content-Type`, e in particolare `Content-Disposition`, che contiene il "nome". Il separatore o `boundary` deve esspere specificato nell'header `Content-Type: multipart/form-data; boundary=AaB03x` e questo è un esempio di message-body di `multipart/form-data`:

```
--AaB03x
content-disposition: form-data; name="field1"

Joe Blow
--AaB03x
content-disposition: form-data; name="pics"; filename="file1.txt"
Content-Type: text/plain

    ... contents of file1.txt ...
--AaB03x--
```

Si noti che il boundary `AaB03x` ci ha due `--` in meno rispetto al reale boundary, perché la grammatica prevede di incapulsare la barriera con `--` all'inizio e `--` alla fine nell'ultimo boundary. 

L'header `Content-Disposition` indica il nome (chiave in termini di `application/x-www-form-urlencoded`) e il nome del file inviato in caso che esso debba essere salvato, Il mimetype invece è presente nell'header `Content-Type`, ma è opzionale.

Il server legge i dati fino alla successiva stringa limite. Il browser deve scegliere un limite che non verrà visualizzato in nessuno dei campi, quindi questo è il motivo per cui il limite può variare tra le richieste.

Chicche varie: Come scegliere ottimalmente il boundary: https://cs.stackexchange.com/questions/39687/find-the-shortest-string-that-is-not-a-sub-string-of-a-set-of-strings


Un esempio di dati inviati tramite multipart è il seguente:

```
Content-Type: multipart/form-data; boundary=---------------------------735323031399963166993862150
Content-Length: 834

-----------------------------735323031399963166993862150
Content-Disposition: form-data; name="text1"

text default
-----------------------------735323031399963166993862150
Content-Disposition: form-data; name="text2"

aωb
-----------------------------735323031399963166993862150
Content-Disposition: form-data; name="file1"; filename="a.txt"
Content-Type: text/plain

Content of a.txt.

-----------------------------735323031399963166993862150
Content-Disposition: form-data; name="file2"; filename="a.html"
Content-Type: text/html

<!DOCTYPE html><title>Content of a.html.</title>

-----------------------------735323031399963166993862150
Content-Disposition: form-data; name="file3"; filename="binary"
Content-Type: application/octet-stream

aωb
-----------------------------735323031399963166993862150--
```

