// Glowny plik aplikacji,
// Dla wiekszosci programow w Qt ma on taka sama postac

// Dolaczamy plik naglowkowy klasy QApplication
#include <QApplication>
#include <QTimer>

// Dolaczamy plik naglowkowy klasy glownego widgetu (okna) aplikacji
#include "mywindow.h"
#include "Game/Genaration.h"
#include "Game/HMath.h"
#include <iostream>

HyperMaze game = HyperMaze();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // Tworzymy obiekt klasy MyWindow - glownego okna naszej aplikacji.
    // Jest to klasa zdefiniowana przez nas.
    // Jej definicja znajduje sie w plikach mainwindow.h i mainwindow.cpp
    MyWindow w;
    game.window = &w;
    game.app = &a;
    w.game = &game;
    game.Pause = true;


    // Pokazujemy glowne okno aplikacji na ekranie. Domyslnie jest ono niewidoczne.
    // Wszystkie widgety (elementy GUI) zawarte w glownym oknie beda rowniez widoczne.
    w.show();

    // Przekazujemy kontrole nad aplikacja do Qt. Program wchodzi w petle zdarzeñ
    // tzn. zaczyna oczekiwac na akcje uzytkownika - klikniecia przycisku myszy,
    // lub klawisza klawiatury itp.

    game.Init();
    return game.Run();


    return 0;
}
