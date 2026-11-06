# Async Logger

Un micro-moteur de journalisation asynchrone zero-allocation vers fichier avec buffer tournant.

## Fonctionnalités
- Journalisation asynchrone sans allocation dynamique (zero-allocation)
- Buffer tournant pour éviter les blocages
- Support multi-threading
- Écriture directe dans un fichier

## Compilation et Exécution

```bash
cmake -B build
cd build && cmake --build .
./logger_app # Pour l'exemple principal
./unit_tests  # Pour exécuter les tests unitaires
```

## Structure du Projet
- `include/logger.h`: Interface de la classe Logger
- `src/logger.cpp`: Implémentation du logger asynchrone avec buffer tournant
- `src/main.cpp`: Exemple d'utilisation multi-threading
- `tests/test.cpp`: Tests unitaires simples
