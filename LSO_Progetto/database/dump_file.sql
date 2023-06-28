--
-- PostgreSQL database dump
--

-- Dumped from database version 12.15 (Ubuntu 12.15-0ubuntu0.20.04.1)
-- Dumped by pg_dump version 12.15 (Ubuntu 12.15-0ubuntu0.20.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: drink; Type: TABLE; Schema: public; Owner: alesilv
--

CREATE TABLE public.drink (
    nome character varying(256) NOT NULL,
    descrizione character varying(256) NOT NULL,
    vendite integer DEFAULT 0,
    costo double precision NOT NULL,
    frullato integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.drink OWNER TO alesilv;

--
-- Name: ingredient; Type: TABLE; Schema: public; Owner: alesilv
--

CREATE TABLE public.ingredient (
    nome_ingrediente character varying(64) NOT NULL,
    nome_drink character varying(256)
);


ALTER TABLE public.ingredient OWNER TO alesilv;

--
-- Name: utente; Type: TABLE; Schema: public; Owner: alesilv
--

CREATE TABLE public.utente (
    username character varying(64) NOT NULL,
    password character varying(36) NOT NULL,
    portafoglio double precision DEFAULT 0
);


ALTER TABLE public.utente OWNER TO alesilv;

--
-- Data for Name: drink; Type: TABLE DATA; Schema: public; Owner: alesilv
--

COPY public.drink (nome, descrizione, vendite, costo, frullato) FROM stdin;
Daiquiri	classico e popolare , una specialità cubana	18	4.5	0
Sex on the Beach 	cocktail fruttato	10	6	0
Bloody Mary	gusto avvolgente e leggermente piccante	6	5.5	0
Mojito	bevanda fresca con note amarognole e acide	12	6.5	0
Long Island	Super alcolico e ricco di ingredienti	3	8.5	0
Frullato di banane	estremamente nutrienete	1	3	1
Frullato di fragole	cremosa e dissetante	5	3	1
Limonata	rinfrescante	22	2	1
Pina Colada	Cocktail dolce e rinfrescante	39	5.5	0
Cosmopolitan	Cocktail rinfrescante	34	8	0
Mimosa	Drink fresco e leggero a base d'arancia	26	5	0
Frullato di frutta	salutare drink a base di frutta mista	10	4	1
manhattan	un classico 	2	7	0
Margarita	Esotico,con una nota dolce e un carattere decisamente rinfrescante	47	6.5	0
Frappe all amarena	merenda buonissima e rinfrescante	59	3	1
Frappe al cioccolato	merenda buonissima e rinfrescante	61	3	1
Frappe	merenda rinfrescante	71	3	1
\.


--
-- Data for Name: ingredient; Type: TABLE DATA; Schema: public; Owner: alesilv
--

COPY public.ingredient (nome_ingrediente, nome_drink) FROM stdin;
whisky	manhattan
vermouth	manhattan
ciliegie	manhattan
tequila	Margarita
lime	Margarita
triple sec	Margarita
vodka	Cosmopolitan
cointreau	Cosmopolitan
succo di mirtillo	Cosmopolitan
rum bianco	Daiquiri
lime	Daiquiri
sciroppo di zucchero	Daiquiri
vodka	Sex on the Beach 
liquore alla pesca	Sex on the Beach 
spumante	Mimosa
succo d'arancia	Mimosa
vodka	Bloody Mary
succo di pomodoro	Bloody Mary
rum bianco	Mojito
menta	Mojito
rum	Pina Colada
latte di cocco	Pina Colada
vodka	Long Island
coca cola	Long Island
banana	Frullato di banane
acqua	Frullato di banane
pesca	Frullato di frutta
fragola	Frullato di frutta
kiwi	Frullato di frutta
fragola	Frullato di fragole
zucchero	Frullato di fragole
limone	Limonata
zucchero	Limonata
gelato al cioccolato	Frappe al cioccolato
latte	Frappe al cioccolato
\.


--
-- Data for Name: utente; Type: TABLE DATA; Schema: public; Owner: alesilv
--

COPY public.utente (username, password, portafoglio) FROM stdin;
Franco12	FrancoPass12	45
Alessia00	AlessiaPass00	12
Matteo98	MatteoPass98	0
Tender10	TenderPass10	100
Pluto1	PlutoPass1	0
\.


--
-- Name: drink drink_pk; Type: CONSTRAINT; Schema: public; Owner: alesilv
--

ALTER TABLE ONLY public.drink
    ADD CONSTRAINT drink_pk PRIMARY KEY (nome);


--
-- Name: utente user_pk; Type: CONSTRAINT; Schema: public; Owner: alesilv
--

ALTER TABLE ONLY public.utente
    ADD CONSTRAINT user_pk PRIMARY KEY (username);


--
-- Name: ingredient ingredient_drink_nome_fk; Type: FK CONSTRAINT; Schema: public; Owner: alesilv
--

ALTER TABLE ONLY public.ingredient
    ADD CONSTRAINT ingredient_drink_nome_fk FOREIGN KEY (nome_drink) REFERENCES public.drink(nome) ON DELETE SET NULL;


--
-- PostgreSQL database dump complete
--

