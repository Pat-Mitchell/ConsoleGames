# ConsoleGames
Just having fun with the console making simple games.

Learning how to use Windows.h for fun.

* UpdatedSource
  - A game someone made using Windows.h. It didn't work on my computer, so I updated the code that dealt with the buffer. 
  - Learned about the difference between WriteConsoleOutput and WriteConsoleOutputA.
  - Unicode characters
  - Being able to move the cursor to different parts of the console for the UI
 
* ConwaysGoL
  - Conway's Game of Life.
  - Using the learned knowledge of the Console from UpdatedSource for an "original" project

* PlayerMoving
  - The "map" that's displayed in the previous two files was static and hard coded.
  - PlayerMoving changes the backend to store the map dynamically and it's easy to remove hard coded values.
  - At one point, the user would enter the values of the height + width, but I got tired of entering numbers while testing.
  - Takes user input from the keyboard to move a player around the map within the bounds of the map
  
* newMap
  - Updates the map from primative characters to objects (tiles)
  - The plan was for each tile to possibly contain multiple objects like a player, an enemy, and items at the same time.
  - Behaves exactly like PlayerMoving.
  
* snek
  - A console based snake game
  - Accidentally forgot about the intentions of the last experiment and explored inheritence.
