#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>

using namespace std;

const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;
const int NUM_OF_ANT = 100;
const int NUM_OF_DOODLEBUG = 5;

enum OBJECT_TYPE { ANT, DOODLEBUG, EMPTY };

enum DIRECTIONS { UP, DOWN, LEFT, RIGHT, NUMBER_OF_DIRECTIONS };
struct Coord
{
    int x;
    int y;
};
typedef struct Coord coord;

class Grid; //forward declaration - because we use "Grid" in Cell class, so we need to declare it before Cell class
class Cell
{
public:
    Cell(int x_coord, int y_coord, Grid& grid);
    virtual void display(sf::RenderWindow& window) {};
    virtual void move() {};
    virtual OBJECT_TYPE who() { return EMPTY; }
    virtual void breed() {};
    virtual void starve() {};
protected:
    coord my_coord;
    Grid& current_grid;
};

class Grid
{
public:
    Grid();
    void display(sf::RenderWindow& window);
    vector<Cell*>& operator[](int row);
    void simulate_turn();
private:
    vector<vector<Cell*>>my_grid;
};

class Ant : public Cell
{
public:
    Ant(int x_coord, int y_coord, Grid& grid);
    OBJECT_TYPE who() { return ANT; }
    void move();
    void display(sf::RenderWindow& window);
    void breed();
private:
    int num_of_turn;
};

class Doodlebug : public Cell
{
public:
    Doodlebug(int x_coord, int y_coord, Grid& grid);
    OBJECT_TYPE who() { return DOODLEBUG; }
    void move();
    void display(sf::RenderWindow& window);
    void breed();
    void starve();
private:
    int num_of_turn;
    int count_hungry;
};
