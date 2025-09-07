# Dokumentáció

***

## Event Handler - eventh

### Namespace: eventh, eventh::internal, eventh::contextm

- eventh -> Publikus API, használd nyugodtan. + contextm publikus részei
- internal -> internális, belső működés.
- contextm -> Kontextus kezelése

***

### Mappa: code/engine/core/eventh

***

### File-ok:

- cancel.hpp -> Event megállítás.  
- context.hpp -> Event Handler contextusa.  
- contextm.hpp -> Event Handler contextusának kezelése.
- eventh.hpp -> Publikus API, egyebek.  

***

### Publikus API:

#### !FONTOS! ha a contextus változik, akkor vele lévő minden (kivéve poller) is!

- ```eventh::emit<Event>(%Event konstruktor args...%)``` Akármelyik Listener ami fel van iratkozva a poll() után meg fogja kapni  

> > Internálisan, pollereket csinálunk minden eventhez.  

- ```eventh::subscribe<Event>(std::function)``` Egy Listener feliratkoztatása egy eventre
- ```eventh::poll()``` Evenetek továbbítása minden listenernek.
- ```eventh::emit_now()``` Az eventet elküldi várás nélkül  

- ```eventh::use(EventhContext)``` Kontextus beállítása.  

> > Internálisan, töri az összes carry-t. main-nek állítsa be

> > emit és subscribe a main-hez ad.  

- ```eventh::carry(EventhContext, int(mask))``` A context egyik valamilyét hazsnálja  

> > Használat: eventh::carry(ctx2, LISTENER_BIT | QUEUE_BIT)

- ```eventh::init() -> EventhContext&``` initizálja egy contextel, nem szükséges.  

***
