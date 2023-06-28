# Come installare Tender
1. clonare il repository da github con il comando:  
    `git clone https://github.com/AleSbri/Tender.git`
## database:
2. installazione database  
    `$sudo apt-get install postgresql`

3. libreria c comandi postgresql:  
    `$sudo apt-get install libpq-dev`

4. attivazione servizio postgresql:  
    `$sudo service postgresql start`

I dati presenti nel database sono nei file di dump allegato:   
    `LSO_Progetto/database/dump_file.sql`

# guida inizializzazione database dal file dump:  

5. creazione di un database:  
    `createdb -U alesilv tenderdb`

6. comando per importare il file dump nel nuovo database:  
    `psql -U alesilv -d tenderdb -f dump_file.sql`

# guida alla compilazione di Tender 

7. localizzazione path libreria libpq:  
    `$pg_config --libdir`

8. esempio di output:  
    `/usr/include/postgresql`

9. comando per la compilazione con tutte le varie dipendenze:  
    `$gcc -o Socket_server Socket_server.c database/database.c impl/User.c impl/Drink.c -I/usr/include/postgresql -lpq -lpthread`

10. eseguire il programma:  
    `$./Socket_server`
