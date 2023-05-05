# Esame di Reti di Calcolatori - 1 Luglio 2022

La pagina HTML prova.html che trovate nella vostra /home/<matricola> richiama diversi file
di immagini (1.png, 2.png,…,9.png) anch’essi presenti nella “home”. Il client, interpretando il
testo HTML in prova.html, effettua automaticamente tutte le request necessarie comporre la
pagina reperendo, ad una ad una, tutte le 9 immagini ed incorporandole nella pagina

Il programma <matricola>.c (che è il Web Server sviluppato a lezione) dopo che ne avrete
personalizzato il port[^1] consentirà al browser del vostro PC locale di accedere all’URL http://88.80.187.84:<port>/prova.html con tutte le 9 immagini incorporate, ricevendo le relative request
ed inviando le conseguenti response (transazioni) secondo la RFC2616,
- In sequenza
- Una per ogni connessione.

Si applichino al programma <matricola>.c le minime modifiche tali che il server
- Supporti più transazioni (request/response) nell’ambito della stessa connessione
- Supporti transazioni (request/response) su più connessioni concorrenti

Il programma <matricola>.c deve riportare in output la sequenza di richieste effettuate indicando
nome file, socket di riferimento e contatore delle transazione per connessione, ad esempio:

```
/prova.html Socket 4 trans 0
/1.png Socket 4 trans 1
/7.png Socket 6 trans 0
/9.png Socket 5 trans 0
/4.png Socket 4 trans 2
/5.png Socket 5 trans 1
... 
```
Scrivere (come /* commento */ all'inizio del file <matricola>.c ) una breve relazione su
1. Funzionalità puntuali da aggiungere
2. Punti di intervento neĺ programma
3. Eventuali scelte implementative
4. Descrizione dell’esecuzione e verifica correttezza.

---
[^1]:  Per effettuare il test si utilizzi il port indicato nel file port.txt della vostra “home”