# Esame di Reti di Calcolatori -  2 settembre 2016

Implementare un server HTTP che:

- reindirizza a una pagina predefinita se la risorsa di destinazione non è disponibile 
- Invii una risposta temporaneamente non disponibile se la risorsa è disponibile, e dopo una seconda richiesta dia l'output.

**Suggerimenti**:
L'header `Retry-After` è ignorata dalla maggior parte dei browser web, quindi il reindirizzamento non avverrà dopo 10 secondi, ma immediatamente.
