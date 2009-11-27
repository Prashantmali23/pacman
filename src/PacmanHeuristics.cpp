#include "PacmanHeuristics.h"

#include "Pacman.h"

#include "iostream"
#include "cmath"

using namespace std;


int distancia_manhattan(int x1, int x2, int y1, int y2)
{
    return abs(x1-x2) + abs(y1-y2);
}

int distancia_euclidiana(int x1, int x2, int y1, int y2)
{
    return (int)sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
}

int distancia_punto_mas_cercano(const LogicObject * pacman, const GameSpace & gs)
{
    int x = pacman->getShape()->getX();
    int y = pacman->getShape()->getY();
    int w = gs.getWidth();
    int h = gs.getHeight();

    list<const LogicObject *> puntos;
    int min_dist = 1000;
    int frontera = 2;
    bool cubrio_espacio_total = false;
    bool hay_puntos = false;
    do {
        int wi = max(0, x - frontera);
        int hi = max(0, y - frontera);
        int wf = min(w, x + frontera);
        int hf = min(h, y + frontera);

        gs.getObjects(Rectangle(wi, hi, wf - wi, hf - hi), DOT, puntos);
        for (list<const LogicObject *>::const_iterator it = puntos.begin(); it != puntos.end(); it++) {
            const Shape * s = (*it)->getShape();
            min_dist = min(min_dist, distancia_manhattan(x, s->getX(), y, s->getY()));
        }

        hay_puntos = puntos.size() > 0;
        cubrio_espacio_total = (x - frontera) <= 0 && (y - frontera) <= 0 &&
                               (x + frontera) >= w && (y + frontera) >= h;
        frontera *= 2;
    } while (!hay_puntos && !cubrio_espacio_total);

    return min_dist;
}


unsigned int h1(const Game * game)
{
    int frontera = 4;
    const GameSpace & gs = game->getGameSpace();
    int w = gs.getWidth();
    int h = gs.getHeight();
    const LogicObject * pacman = game->getObject(PACMAN);
    const Shape * shape = pacman->getShape();
    int x = shape->getX();
    int y = shape->getY();
    int hi, hf, wi, wf; //Extremos del rectangulo.
    hi = max(0, y - frontera);
    hf = min(h - 1, y + frontera);
    wi = max(0, x - frontera);
    wf = min(w - 1, x + frontera);

    set<string> ghostSubtype;
    ghostSubtype.insert(GHOST);
    list<const LogicObject *> ghosts;
    gs.getObjects(Rectangle(wi,hi,wf-wi,hf-hi), LogicObject::Type(ghostSubtype), ghosts);
    int cant_ghosts = ghosts.size();
    int cant_puntos = game->getVariables().getInteger(DOT_COUNT);

    return cant_puntos + frontera*cant_ghosts;
}

unsigned int h2(const Game * game)
{
    const LogicObject * pacman = game->getObject(PACMAN);
    const GameSpace & gs = game->getGameSpace();

	int cant_puntos = game->getVariables().getInteger(DOT_COUNT);
	int dist_cercano = distancia_punto_mas_cercano(pacman, gs);

    if (cant_puntos <= dist_cercano)
        return cant_puntos;
    return cant_puntos + dist_cercano;
}

unsigned int h3(const Game * game)
{
    int frontera = 4;
    const GameSpace & gs = game->getGameSpace();
    int w = gs.getWidth();
    int h = gs.getHeight();
    const LogicObject * pacman = game->getObject(PACMAN);
    const Shape * shape = pacman->getShape();
    int x = shape->getX();
    int y = shape->getY();
    int hi, hf, wi, wf; //Extremos del rectangulo.
    hi = max(0, y - frontera);
    hf = min(h - 1, y + frontera);
    wi = max(0, x - frontera);
    wf = min(w - 1, x + frontera);
    set<string> ghostSubtype;
    ghostSubtype.insert(GHOST);
    list<const LogicObject *> ghosts;
    gs.getObjects(Rectangle(wi,hi,wf-wi,hf-hi), LogicObject::Type(ghostSubtype), ghosts);
    int cant_ghosts = ghosts.size();
    int cant_puntos = game->getVariables().getInteger(DOT_COUNT);
	int dist_cercano = distancia_punto_mas_cercano(pacman, gs);

    return 5*cant_puntos + cant_ghosts * frontera + dist_cercano/10;
}

unsigned int h4(const Game * game)
{
    const LogicObject * pacman = game->getObject(PACMAN);
    int pac_x = pacman->getShape()->getX();
    int pac_y = pacman->getShape()->getY();
    int cant_puntos = game->getVariables().getInteger(DOT_COUNT);

    int costo_fantasmas = 0;
    list<const LogicObject *> ghosts;
    ghosts.push_back(game->getObject(CLYDE));
    ghosts.push_back(game->getObject(INKY));
    ghosts.push_back(game->getObject(PINKY));
    ghosts.push_back(game->getObject(BLINKY));

    for (list<const LogicObject *>::iterator it = ghosts.begin(); it != ghosts.end(); it++)
    {
        const Shape * s = (*it)->getShape();
        costo_fantasmas += 10/distancia_euclidiana(s->getX(), pac_x, s->getY(), pac_y);
    }

    return  2*cant_puntos + costo_fantasmas;
}

unsigned int h5(const Game * game)
{
    // Reemplazar
	return game->getVariables().getInteger(DOT_COUNT);
}

unsigned int h6(const Game * game)
{

}

