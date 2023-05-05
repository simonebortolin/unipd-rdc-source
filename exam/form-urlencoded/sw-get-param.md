# Esercizio di Reti di Calcolatori

Modificare sw.c affiche possa ricevere passare parametri attraverso la codifica detta www-form-urlencoded nelle query string degli URL (v. RFC 1866 Cap. 8.2).

Il client dovrà accedere tramite il programma web server alla risorsa http://88.80.187.84:<port>/get.html contenente il seguente sorgente html:

```
<html>
<form action="/cgi-bin/command" method="get" enctype="x-www-form-urlencoded">
  <p>Comando <input type="text" id="mycom" name="commando" >
  <p>parametro 1 <input type="text" id="mypar1" name="param1" >
  <p>parametro 2 <input type="text" id="mypar2" name="param2" >
  <p> <input type="submit">
</form>
</html>
```

Il Web Server ricevendo la richiesta alla risorsa /cgi-bin/command dovrà eseguire il comando specificato con i parametri e riportare l’output di quel comando come Entity Body della HTTP-response.

Utilizzare questi parametri per lanciare comandi nel terminale bash come `ls -l -t` (codificato come `commando=ls&param1=-l&param2=-t`), ed usare l'output del terminale come output html.

N.B. passare l'header corretto per indicare il mime type corretto di output, in quanto non è html, prestare attenzione all'escape di eventuali caratteri nella codifica www-form-urlencoded.

Prestare attenzione ad eventuali codifiche ed escape di caratteri speciali ed ad aventuali header obbligatori, come il `Content-Type` che deve contenere `application/x-www-form-urlencoded`.
