#include "ant_doodlebug.hpp"

//constructor and overload operator for Grid class
Grid::Grid()
{
    //give the grid (vectors) the size 
    my_grid.resize(GRID_HEIGHT); //resize the number of row in the vector
    for (int i = 0; i < GRID_HEIGHT; i++) //resize the number of column in each row
    {
        my_grid[i].resize(GRID_WIDTH); //my_grid[i] is a row
    }

    //initialize the grid with Cells
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            my_grid[y][x] = new Cell(y, x, *this);
        }
    }

    //create 100 ants
    for (int number_of_ants = 0; number_of_ants < NUM_OF_ANT; number_of_ants++)
    {
        int randX = rand() % GRID_WIDTH; //x is the number of column
        int randY = rand() % GRID_HEIGHT; //y is the number of row
        if (my_grid[randY][randX]->who() == EMPTY)
        {
            delete my_grid[randY][randX]; //delete the Cell at my_grid[randY][randX]
            my_grid[randY][randX] = new Ant(randX, randY, *this); //create an Ant at my_grid[randY][randX]
        }
    }

    //create 5 doodlebugs
    for (int number_of_bugs = 0; number_of_bugs < NUM_OF_DOODLEBUG; number_of_bugs++)
    {
        int randX = rand() % GRID_WIDTH; //x is the number of column
        int randY = rand() % GRID_HEIGHT; //y is the number of row
        if (my_grid[randY][randX]->who() == EMPTY)
        {
            delete my_grid[randY][randX]; //delete the Cell at my_grid[randY][randX]
            my_grid[randY][randX] = new Doodlebug(randX, randY, *this); //create an Ant at my_grid[randY][randX]
        }
    }
}
vector<Cell*>& Grid::operator[](int row)
{
    if (row >= GRID_HEIGHT || row < 0)
    {
        cout << "Out of bounds access of the Grid height" << endl;
        exit(1);
    }
    return my_grid[row];
}

//constructor with initialization section
Cell::Cell(int x_coord, int y_coord, Grid& grid) : current_grid(grid), my_coord({ x_coord,y_coord }) {}
Ant::Ant(int x_coord, int y_coord, Grid& grid) : Cell(x_coord, y_coord, grid) { num_of_turn = 0; }
Doodlebug::Doodlebug(int x_coord, int y_coord, Grid& grid) : Cell(x_coord, y_coord, grid) { num_of_turn = 0; count_hungry = 0; }

//display functions
void Grid::display(sf::RenderWindow& window)
{
    //display the ants 
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            my_grid[y][x]->display(window);
        }
    }
}
void Ant::display(sf::RenderWindow& window)
{
    sf::CircleShape shape(10.f);            //create a shape type CircleShape with radius = 10
    shape.setFillColor(sf::Color::Yellow);  //fill the color in the shape

    //adjust the position so that it wont be default at (0,0)
    sf::Vector2f display_position;
    display_position.x = my_coord.x * 20 + 10;
    display_position.y = my_coord.y * 20 + 10;
    shape.setPosition(display_position);

    window.draw(shape);                    //display the shape in the window
}
void Doodlebug::display(sf::RenderWindow& window)
{
    sf::CircleShape shape(10.f);            //create a shape type CircleShape with radius = 10
    shape.setFillColor(sf::Color::Red);  //fill the color in the shape

    //adjust the position so that it wont be default at (0,0)
    sf::Vector2f display_position;
    display_position.x = my_coord.x * 20 + 10;
    display_position.y = my_coord.y * 20 + 10;
    shape.setPosition(display_position);

    window.draw(shape);                    //display the shape in the window
}

//initiate turn and move functions
void Grid::simulate_turn()
{
    vector<Cell*>doodlebugs;
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            if (my_grid[y][x]->who() == DOODLEBUG)
            {
                doodlebugs.push_back(my_grid[y][x]);
            }
        }
    }
    if (doodlebugs.size() > 0)
    {
        for (int i = 0; i < 100000; i++)
        {
            int index3 = rand() % doodlebugs.size();
            int index4 = rand() % doodlebugs.size();
            swap(doodlebugs[index3], doodlebugs[index4]);
        }
    }
    //Doodlebugs move first
    for (int i = 0; i < doodlebugs.size(); i++)
    {
        doodlebugs[i]->move();
    }

    vector<Cell*> ants;
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            if (my_grid[y][x]->who() == ANT)
            {
                ants.push_back(my_grid[y][x]);
            }
        }
    }
    if (ants.size() > 0)
    {
        for (int i = 0; i < 100000; i++)
        {
            int index1 = rand() % ants.size();
            int index2 = rand() % ants.size();
            swap(ants[index1], ants[index2]);
        }
    }
    //Ants move after
    for (int i = 0; i < ants.size(); i++)
    {
        ants[i]->move();
    }
}
void Ant::move()
{
    //if survive 3 turns then breed, will move later after breeding
    if (num_of_turn == 3)
        current_grid[my_coord.y][my_coord.x]->breed();

    //now move
    switch (rand() % NUMBER_OF_DIRECTIONS)  //choose a direction at random first 
    {
    case UP:
        if (my_coord.y > 0) //check boundary, if y=0 then cant move up
        {
            if (current_grid[my_coord.y - 1][my_coord.x]->who() == EMPTY) //check availability of upper cell
            {
                swap(current_grid[my_coord.y - 1][my_coord.x], current_grid[my_coord.y][my_coord.x]);
                my_coord.y--; //the current coordinate is updated (cause moved up 1 time)
            }
        }
        break; //do not forget to break because we're using switch statements

    case DOWN:
        if (my_coord.y < GRID_HEIGHT - 1)
        {
            if (current_grid[my_coord.y + 1][my_coord.x]->who() == EMPTY)
            {
                swap(current_grid[my_coord.y + 1][my_coord.x], current_grid[my_coord.y][my_coord.x]);
                my_coord.y++;
            }
        }
        break;
    case LEFT:
        if (my_coord.x > 0)
        {
            if (current_grid[my_coord.y][my_coord.x - 1]->who() == EMPTY)
            {
                swap(current_grid[my_coord.y][my_coord.x - 1], current_grid[my_coord.y][my_coord.x]);
                my_coord.x--;
            }
        }
        break;
    case RIGHT:
        if (my_coord.x < GRID_WIDTH - 1)
        {
            if (current_grid[my_coord.y][my_coord.x + 1]->who() == EMPTY)
            {
                swap(current_grid[my_coord.y][my_coord.x + 1], current_grid[my_coord.y][my_coord.x]);
                my_coord.x++;
            }
        }
        break;
    }
    num_of_turn++;
}
void Doodlebug::move()
{
    bool did_you_eat = false;
    //check for every adjacent cells to see if there is an Ant
    if (my_coord.y > 0 || my_coord.y < GRID_HEIGHT - 1 || my_coord.x > 0 || my_coord.x < GRID_WIDTH - 1)
    {
        if (my_coord.y > 0 && did_you_eat == false)
        {
            if (current_grid[my_coord.y - 1][my_coord.x]->who() == ANT)
            {
                delete current_grid[my_coord.y - 1][my_coord.x];
                current_grid[my_coord.y - 1][my_coord.x] = new Cell(my_coord.x, my_coord.y - 1, current_grid);
                swap(current_grid[my_coord.y - 1][my_coord.x], current_grid[my_coord.y][my_coord.x]);
                my_coord.y--;
                did_you_eat = true;
                count_hungry = 0;
            }
        }
        if (my_coord.y < GRID_HEIGHT - 1 && did_you_eat == false)
        {
            if (current_grid[my_coord.y + 1][my_coord.x]->who() == ANT)
            {
                delete current_grid[my_coord.y + 1][my_coord.x];
                current_grid[my_coord.y + 1][my_coord.x] = new Cell(my_coord.x, my_coord.y + 1, current_grid);
                swap(current_grid[my_coord.y + 1][my_coord.x], current_grid[my_coord.y][my_coord.x]);
                my_coord.y++;
                did_you_eat = true;
                count_hungry = 0;
            }
        }
        if (my_coord.x > 0 && did_you_eat == false)
        {
            if (current_grid[my_coord.y][my_coord.x - 1]->who() == ANT)
            {
                delete current_grid[my_coord.y][my_coord.x - 1];
                current_grid[my_coord.y][my_coord.x - 1] = new Cell(my_coord.x - 1, my_coord.y, current_grid);
                swap(current_grid[my_coord.y][my_coord.x - 1], current_grid[my_coord.y][my_coord.x]);
                my_coord.x--;
                did_you_eat = true;
                count_hungry = 0;
            }
        }
        if (my_coord.x < GRID_WIDTH - 1 && did_you_eat == false)
        {
            if (current_grid[my_coord.y][my_coord.x + 1]->who() == ANT)
            {
                delete current_grid[my_coord.y][my_coord.x + 1];
                current_grid[my_coord.y][my_coord.x + 1] = new Cell(my_coord.x + 1, my_coord.y, current_grid);
                swap(current_grid[my_coord.y][my_coord.x + 1], current_grid[my_coord.y][my_coord.x]);
                my_coord.x++;
                did_you_eat = true;
                count_hungry = 0;
            }
        }
    }

    if (did_you_eat == false) //case when doodlebug cant eat any ant for its turn
    {
        //now move to a random direction where there isn't an Ant
        switch (rand() % NUMBER_OF_DIRECTIONS)
        {
        case UP:
            if (my_coord.y > 0)
            {
                if (current_grid[my_coord.y - 1][my_coord.x]->who() == EMPTY)
                {
                    swap(current_grid[my_coord.y - 1][my_coord.x], current_grid[my_coord.y][my_coord.x]);
                    my_coord.y--;
                }
            }
            break;

        case DOWN:
            if (my_coord.y < GRID_HEIGHT - 1)
            {
                if (current_grid[my_coord.y + 1][my_coord.x]->who() == EMPTY)
                {
                    swap(current_grid[my_coord.y + 1][my_coord.x], current_grid[my_coord.y][my_coord.x]);
                    my_coord.y++;
                }
            }
            break;
        case LEFT:
            if (my_coord.x > 0)
            {
                if (current_grid[my_coord.y][my_coord.x - 1]->who() == EMPTY)
                {
                    swap(current_grid[my_coord.y][my_coord.x - 1], current_grid[my_coord.y][my_coord.x]);
                    my_coord.x--;
                }
            }
            break;
        case RIGHT:
            if (my_coord.x < GRID_WIDTH - 1)
            {
                if (current_grid[my_coord.y][my_coord.x + 1]->who() == EMPTY)
                {
                    swap(current_grid[my_coord.y][my_coord.x + 1], current_grid[my_coord.y][my_coord.x]);
                    my_coord.x++;
                }
            }
            break;
        }
        count_hungry++;
    }

    //whether eat or move to empty cell, number of turns will still increment
    num_of_turn++;

    //debug code
    /*if (current_grid[my_coord.y][my_coord.x]->who() == DOODLEBUG)
        cout << "its a doodlebug" << endl;
    else if (current_grid[my_coord.y][my_coord.x]->who() == ANT)
        cout << "its an ant" << endl;
    else if (current_grid[my_coord.y][my_coord.x]->who() == EMPTY)
        cout << "its a cell" << endl;
    cout << "number of turns is " << num_of_turn << endl;
    cout << "count hungry is " << count_hungry << endl;*/

    //if hungry after 3 steps then die, sorry :(
    if (count_hungry == 3)
        current_grid[my_coord.y][my_coord.x]->starve();

    //if survive 8 turns then breed
    if (num_of_turn == 8)
        current_grid[my_coord.y][my_coord.x]->breed();
}

//breed functions
void Ant::breed()
{
    //if survive 3 steps then create new Ant in an adjacent cell (choose random adjacent cell)
    switch (rand() % NUMBER_OF_DIRECTIONS) //pick an adjacent cell randomly
    {
    case UP:
        if (my_coord.y > 0)
        {
            if (current_grid[my_coord.y - 1][my_coord.x]->who() == EMPTY)
            {
                delete current_grid[my_coord.y - 1][my_coord.x];
                current_grid[my_coord.y - 1][my_coord.x] = new Ant(my_coord.x, my_coord.y - 1, current_grid);
            }
        }
        break;
    case DOWN:
        if (my_coord.y < GRID_HEIGHT - 1)
        {
            if (current_grid[my_coord.y + 1][my_coord.x]->who() == EMPTY)
            {
                delete current_grid[my_coord.y + 1][my_coord.x];
                current_grid[my_coord.y + 1][my_coord.x] = new Ant(my_coord.x, my_coord.y + 1, current_grid);
            }
        }
        break;
    case LEFT:
        if (my_coord.x > 0)
        {
            if (current_grid[my_coord.y][my_coord.x - 1]->who() == EMPTY)
            {
                delete current_grid[my_coord.y][my_coord.x - 1];
                current_grid[my_coord.y][my_coord.x - 1] = new Ant(my_coord.x - 1, my_coord.y, current_grid);
            }
        }
        break;
    case RIGHT:
        if (my_coord.x < GRID_WIDTH - 1)
        {
            if (current_grid[my_coord.y][my_coord.x + 1]->who() == EMPTY)
            {
                delete current_grid[my_coord.y][my_coord.x + 1];
                current_grid[my_coord.y][my_coord.x + 1] = new Ant(my_coord.x + 1, my_coord.y, current_grid);
            }
        }
        break;
    }
    num_of_turn = 0;
}
void Doodlebug::breed()
{
    //if survive 8 steps then create new Doodlebug in an adjacent cell (choose random adjacent cell)
    //cout << "breed of doodlebug called" << endl;
    bool breed_already = false;
    while (!breed_already)
    {
        switch (rand() % NUMBER_OF_DIRECTIONS) //pick an adjacent cell randomly
        {
        case UP:
            if (my_coord.y > 0)
            {
                if (current_grid[my_coord.y - 1][my_coord.x]->who() == EMPTY)
                {
                    delete current_grid[my_coord.y - 1][my_coord.x];
                    current_grid[my_coord.y - 1][my_coord.x] = new Doodlebug(my_coord.x, my_coord.y - 1, current_grid);
                    breed_already = true;
                }
            }
            break;
        case DOWN:
            if (my_coord.y < GRID_HEIGHT - 1)
            {
                if (current_grid[my_coord.y + 1][my_coord.x]->who() == EMPTY)
                {
                    delete current_grid[my_coord.y + 1][my_coord.x];
                    current_grid[my_coord.y + 1][my_coord.x] = new Doodlebug(my_coord.x, my_coord.y + 1, current_grid);
                    breed_already = true;
                }
            }
            break;
        case LEFT:
            if (my_coord.x > 0)
            {
                if (current_grid[my_coord.y][my_coord.x - 1]->who() == EMPTY)
                {
                    delete current_grid[my_coord.y][my_coord.x - 1];
                    current_grid[my_coord.y][my_coord.x - 1] = new Doodlebug(my_coord.x - 1, my_coord.y, current_grid);
                    breed_already = true;
                }
            }
            break;
        case RIGHT:
            if (my_coord.x < GRID_WIDTH - 1)
            {
                if (current_grid[my_coord.y][my_coord.x + 1]->who() == EMPTY)
                {
                    delete current_grid[my_coord.y][my_coord.x + 1];
                    current_grid[my_coord.y][my_coord.x + 1] = new Doodlebug(my_coord.x + 1, my_coord.y, current_grid);
                    breed_already = true;
                }
            }
            break;
        }
    }
    num_of_turn = 0;
}

//starve function
void Doodlebug::starve()
{
    int temp_x = my_coord.x;
    int temp_y = my_coord.y;
    delete current_grid[my_coord.y][my_coord.x];
    current_grid[temp_y][temp_x] = new Cell(temp_x, temp_y, current_grid);
}