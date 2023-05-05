L'utilizzo di siti web in HTTP non è sicuro, in quanto le informazioni possono essere facilmente intercettate da attacchi di tipo "man-in-the-middle". Pertanto, è sempre consigliabile utilizzare siti web che utilizzano HTTPS per garantire la sicurezza delle tue informazioni personali e della navigazione in generale.

Per creare un client che supporti SSL/TLS è necessario installare la libreria OpenSSL-dev sulla macchina (`sudo apt install libssl-dev`). Dopo aver fatto ciò, includi le librerie necessarie nel tuo codice.

```
#include <openssl/ssl.h>
#include <openssl/err.h>
```

```
SSL_CTX *ssl_ctx = NULL;
```

Inizializza il contesto SSL con il seguente codice:
```
SSL_library_init();
SSL_load_error_strings();
OpenSSL_add_all_algorithms();
```

Successivamente, crea un contesto SSL, che definirà i parametri della connessione SSL/TLS e crea una connessione SSL usando il socket creato. Inizializza un oggetto SSL e assegnalo alla connessione SSL/TLS. Il codice seguente è un esempio di come creare una connessione SSL sicura.

```
SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
SSL *ssl = SSL_new(ctx);

if(!ssl){
    printf("Errore nella creazione del contesto SSL.\n");
    return 1;
}

if(!SSL_set_fd(ssl, s)){
    printf("Errore nell'associazione del socket con la connessione SSL.\n");
    return 1;
}

if(SSL_connect(ssl) != 1){
    printf("Errore nella connessione SSL.\n");
    return 1;
}
```
Dopo di che è necessario modificare la lettura e la scrittura dei dati, usando le funzioni `SSL_read()` e `SSL_write()` invece di `read()` e `write()`.

Infine, dopo aver terminato la connessione, è necessario liberare la memoria allocata per il contesto SSL e la connessione SSL:
```
SSL_shutdown(ssl);
SSL_free(ssl);
SSL_CTX_free(ctx);
```

N.B. per compilare è necessario usare `gcc cw_ssl.c -lssl -lcrypto -o cw_ssl`
