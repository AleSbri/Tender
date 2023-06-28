# Dipedenze:

## database:
`$sudo apt-get install postgresql`

- libreria c comandi postgresql:
    `$sudo apt-get install libpq-dev`

- attivazione servizio postgresql:
    `$sudo service postgresql start`

I dati presenti nel database sono nei file di dump allegato: 
    `LSO_Progetto/database/dump_file.sql`

# guida inizializzazione database dal file dump:

- creazione di un database:
    `createdb -U alesilv tenderdb`

- comando per importare il file dump nel nuovo database:
    `psql -U alesilv -d tenderdb -f dump_file.sql`


# guida alla compilazione di Tender 

- localizzazione path libreria libpq:
    `$pg_config --libdir`

- esempio di output:
    `/usr/include/postgresql`

- comando per la compilazione con tutte le varie dipendenze:
    `$gcc -o Socket_server Socket_server.c database/database.c impl/User.c impl/Drink.c -I/usr/include/postgresql -lpq -lpthread`

- eseguire il programma:
    `$./Socket_server`