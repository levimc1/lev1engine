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

***!FONTOS!*** *ha a contextus változik, akkor vele lévő minden (kivéve poller) is!*

- ```eventh::emit<Event>(%Event konstruktor args...%)``` Akármelyik Listener ami fel van iratkozva a poll() után meg fogja kapni  

  > Internálisan, pollereket csinálunk minden eventhez.  

- ```eventh::subscribe<Event>(std::function)``` Egy Listener feliratkoztatása egy eventre
- ```eventh::poll()``` Evenetek továbbítása minden listenernek.
- ```eventh::emit_now()``` Az eventet elküldi várás nélkül  
- ```eventh::use(EventhContext)``` Kontextus beállítása.  

  > Internálisan, töri az összes carry-t. main-nek állítsa be

  > emit és subscribe a main-hez ad.  

- ```eventh::carry(EventhContext, int(mask))``` A context egyik valamilyét hazsnálja  

  > Használat: eventh::carry(ctx2, LISTENER_BIT | QUEUE_BIT)

- ```eventh::init() -> EventhContext&``` initizálja egy contextel, nem szükséges.  

***

## Task Manager - taskm

### Namespace: taskm, internal

- taskm -> Publikus API
- internal -> Internális

***

### Mappa: code/engine/core/taskm

***

### File-ok:

- flag.hpp -> Flagok kezelése
- flow.hpp -> A rendszer ami eldönti mi következik mi után
- task.hpp -> A feladat objektuma, és ami kapcsolódik
- taskm.hpp -> A feladatok kezelése
- def_flow/ -> Jövőben áthelyezve a Task Scheduler modulhoz  

***

### Publikus API:

- ```taskm::init()``` Feladatkezelő initizálása. Nem kötelező.0
- ```taskm::cleanup()``` Üres
- ```taskm::tick()``` Egy tick futtatása, futtassa az összes feladatot
- ```taskm::make_task(Kondíció/Funkció)``` Feladat OBJEKTUM létrehozása  
- ```taskm::condition(...)``` make_task-hez segítő
- ```taskm::process(...)``` make_task-hez segítő  
- ```taskm::add()``` Feladat hozzáadása, **SZÜKSÉGES A RENDSZER MŰKÖDÉSÉHEZ**
- ```taskm::run()``` Egy task futtatása
- ```taskm::controller()``` Egy void() regisztrálása hogy minden frame-ben meghatározza a next-et.  

 > Internális okokból, elég szimpla. Ne igazán használd

- ```taskm::flag<Típus, Address típusa>(Address értéke)``` Egy flag megszerzése(Reference)

***
