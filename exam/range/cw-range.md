# Esame di Reti di Calcolatori - 16 Luglio 2020

Si modifichi il web client sviluppato durante il corso per renderlo in grado di scaricare file di grandi dimensioni in
presenza di connettività di rete non affidabile caratterizzata da una frequente interruzione delle connessioni.
In queste condizioni, lo scaricamento di una risorsa di grandi dimensioni rischia di essere interrotto dalla perdita
della connessione. Risulta evidente che anche ripetendo più volte lo scaricamento del file intero si rischia ogni
volta l’interruzione e di conseguenza la probabilità di terminare con successo l’operazione diviene molto bassa.
Per ovviare a questo inconveniente il client web dev’essere modificato in modo tale che scarichi a piccoli pezzi il
file tramite una sequenza di più richieste che scaricano ciascuna un segmento del file (ad esempio di 10
Kbytes) che verranno alla fine giustapposti in un buffer per salvare il contenuto completo su un file locale.
  
Il protocollo HTTP/1.1 supporta questa funzione per mezzo dell’header “Range”. Documentato nella RFC 2616
in particolare nelle sezioni 14.35 e 14.16 e 3.12.
  
Scrivere (come /* commento */ all'inizio del file esame.c) una breve relazione su
          
1. Funzionalità puntuali da aggiungere
2. Punti di intervento nel programma
3. Eventuali scelte implementative
4. Descrizione dell'esperimento e della stampa (printf) di dati di controllo
5. Descrizione dell'esito e verifica correttezza.
          
Per la sperimentazione collegarsi con il web client all’URL http://88.80.187.84:80/image.jpg (in alternativa usare https://i.imgur.com/z4d4kWk.jpg)

NB: Negli esperimenti non occorre simulare la perdita di connessione.
