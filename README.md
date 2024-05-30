La chat può avere un solo canale: chi si collega vede tutti i messaggi passati e può scrivere i suoi
La chat è real time: definisci tu il protocollo. Ogni client è connesso e vede i messaggi che arrivano dagli altri
Ogni client può essere identificato da un semplice nome passato al client a linea di comando.
L'accesso alla struttura condivisa della lista dei messaggi deve supportare la concorrenza (semafori semplici o letttori/scrittori vanno bene)
Aggiungi solo un piccolo storage dove tutti i messaggi passati vengono salvati e all'avvio del server vengono ricaricati in memoria