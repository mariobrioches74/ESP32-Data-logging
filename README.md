# Rilevatore di dati ambientali con ESP32 

Progetto IoT basato su **ESP32** e sensore **DHT11** per il monitoraggio ambientale in tempo reale.  
Il sistema acquisisce dati di **temperatura** e **umidità**, li invia tramite protocollo **HTTPS** a un **Realtime Database di Firebase** e li visualizza attraverso una dashboard web ospitata con **Firebase Hosting**.

---

## Funzionalità

- Lettura della temperatura e umidità tramite sensore DHT11
- Connessione Wi-Fi tramite ESP32
- Invio dati in tempo reale a Firebase Realtime Database tramite HTTPS
- Dashboard web per la visualizzazione dei dati live
- Hosting della pagina web tramite Firebase Hosting

---

## Tecnologie Utilizzate

### Hardware
- ESP32
- Sensore DHT11

### Software & Cloud
- VScode + PlatformIO extension
- Firebase Realtime Database
- Firebase Hosting
- HTML / CSS / JavaScript / Python

---

## Architettura del Sistema

```text
DHT11 --> ESP32 --> HTTPS Request --> Firebase Realtime Database
                                                |
                                                v
                                   Firebase Hosting Web App
```

---

## Dashboard Web

La pagina web mostra in tempo reale:

- Temperatura ambientale
- Umidità relativa
- Aggiornamento automatico dei dati

---
