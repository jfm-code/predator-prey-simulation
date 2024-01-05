<h1>Predator-Prey Simulation</h1>

This is a feature-rich 2D predator-prey simulation implemented in C++ using SFML (Simple and Fast Multimedia Library). The simulation takes place on a 20 x 20 grid, where ants (prey) and doodlebugs (predators) interact in a dynamic ecosystem.

<p> 
Explore the fascinating interplay between predators and prey in this simulation, where OOP principles such as inheritance and polymorphism contribute to a modular and extensible code structure.</p>


<h2>Features</h2>

- <b>Ant Behavior:</b>
  - Random movement in up, down, left, or right directions.
  - Breeding after surviving for three time steps.
  - Breeding results in a new ant in an adjacent empty cell.

- <b>Doodlebug Behavior:</b>
  - Moves to an adjacent cell containing an ant and eats it.
  - If no ants nearby, moves randomly like ants.
  - Breeds after surviving for eight time steps.
  - Starves and dies if it hasn't eaten an ant within three-time steps.

- <b>Simulation Dynamics:</b>
  - Initialization with 5 doodlebugs and 100 ants randomly placed.
  - Time steps simulate the movement, breeding, and starvation of critters.
  - Displayed using SFML, with green circles representing ants and red circles representing doodlebugs.
