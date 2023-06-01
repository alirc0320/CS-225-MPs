/* Your code here! */
#include "maze.h"
#include <iostream>
#include "dsets.h"
#include <vector>
#include <random>
#include <queue>
#include <algorithm>

using namespace std;
using namespace cs225; 
 
SquareMaze::SquareMaze() {
  width_ = 0;
  height_ = 0;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if (dir == 0) { // right wall
        int index = y * width_ + x;
        maze_[index].right = exists;
    } else if (dir == 1) { // down wall
        int index = y * width_ + x;
        maze_[index].down = exists;
    }
}
bool SquareMaze::canTravel(int x, int y, int dir) const {
  // check if input coordinates are within bounds
  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
    return false;
  }

  // determine if there is a wall in the direction of interest
  if (dir == 0) { // check rightward step
    if (x + 1 >= width_ || maze_[y * width_ + x].right) {
      return false;
    }
  } if (dir == 1) { // check downward step
    if (y + 1 >= height_ || maze_[y * width_ + x].down) {
      return false;
    }
  } if (dir == 2) { // check leftward step
    if (x < 1 || maze_[y * width_ + x - 1].right) {
      return false;
    }
  } if (dir == 3) { // check upward step
    if (y < 1 || maze_[(y - 1) * width_ + x].down) {
      return false;
    }
  }

  // no wall in direction of interest, can travel in that direction
  return true;
}




void SquareMaze::makeMaze(int width, int height) {
    // Initialize the maze
    width_ = width;
    height_ = height; 


    maze_.resize(width * height);
    // down_walls_.resize(width * height, true);
    // right_walls_.resize(width * height, true);
    walls_.clear();
    for (int i = 0; i < width * height; i++) {
        maze_[i].down = true;
        maze_[i].right = true;
    }

    // Create disjoint sets for each cell in the maze
    DisjointSets sets;
    sets.elmnts.clear(); 
    sets.addelements(width * height);

    // Seed the random number generator
    // srand(time(nullptr));

    // Randomly remove walls until there is a single connected component
    while (sets.size(0) < width * height) {
      // std::cout<<"reached: "<<__LINE__<<std::endl;
      // std::cout<<"set size: "<<sets.size(0)<<std::endl;
        // Generate a random index and x/y coordinates
        // int index = rand() % (width * height);
        int x = rand() % width;  // Generate x coordinate
        int y = rand() % height;  // Generate y coordinate

        // Check if the cell is on the perimeter
//         if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
//     continue;  /
// }
        //}

        // Randomly choose to remove either the right or down wall
        bool removeRight = rand() % 2;
        int a = y * width + x;  // Index of current cell
        
        // int b; // Index of adjacent cell depending on which wall we want to remove  // Index of adjacent cell
        //   if (removeRight) {
        //       b = a + 1; // Index of cell to the right
        //   } else {
        //       b = a + width; // Index of cell below
        //   } 
      
      int bx = x + (removeRight == 1);
      int by = y + (removeRight == 0);
      int b = by * width + bx;

      if(bx < width && by < height){
      
      // bool wallExists; // Check if the wall exists
        // if (removeRight) {
        //     wallExists = maze_[a].right;
        // } else {
        //     wallExists = maze_[a].down;
        // } 

        if (sets.find(a) != sets.find(b)) {  // Check if there is a loop
            // Remove the wall and join the sets
            if (removeRight) {
                maze_[a].right = false;
               // right_walls_[a] = false;
            } else {
                maze_[a].down = false;
               // down_walls_[a] = false;
            }
            sets.setunion(a, b);
        }
      }
    }
}

vector<int> SquareMaze::solveMaze() {
    // Initialize variables
    vector<int> path;
    vector<int> distances(height_ * width_, -1);
    vector<int> prev(height_ * width_, -1);
    queue<int> q;
    
    // Add the start cell to the queue
    int startCell = 0;
    q.push(startCell);
    distances[startCell] = 0;
    
    // Perform BFS
    while (!q.empty()) {
        int currCell = q.front();
        q.pop();

        // Check all neighbors of the current cell
        int x = currCell % width_;
        int y = currCell / width_;
        if (canTravel(x, y, 0) && distances[currCell+1] == -1) { // Right
            int right = currCell + 1;
            q.push(right);
            distances[right] = distances[currCell] + 1;
            prev[right] = currCell;
        }
        if (canTravel(x, y, 1) && distances[currCell+width_] == -1) { // Down
            int down = currCell + width_;
            q.push(down);
            distances[down] = distances[currCell] + 1;
            prev[down] = currCell;
        }
        if (canTravel(x, y, 2) && distances[currCell-1] == -1) { // Left
            int left = currCell - 1;
            q.push(left);
            distances[left] = distances[currCell] + 1;
            prev[left] = currCell;
        }
        if (canTravel(x, y, 3) && distances[currCell-width_] == -1) { // Up
            int up = currCell - width_;
            q.push(up);
            distances[up] = distances[currCell] + 1;
            prev[up] = currCell;
        }
    }
    std::cout << "We made it through the first while loop" << std::endl;
    // Traverse the path from the destination to the origin
    int currentlong = 0;
    int squarewithgreatestdist = 0;
    int destCell = (height_ - 1) * width_;
    int currCell;
    int x;
    for (int i = 0; i < width_; i++) {
        int curr = destCell + i;
        if (currentlong < distances[curr]) {
            currentlong = distances[curr];
            squarewithgreatestdist = curr;
            x = i;
        } else if (currentlong == distances[curr]) {
            squarewithgreatestdist = min(squarewithgreatestdist, curr);
        }
    }

    std::cout << "We made it through the first for loop" << std::endl;


    currCell = squarewithgreatestdist;

    //int xref = currCell;
    int xref = x;
    int yref = height_ - 1;

    std::cout << "The xref is " <<xref << std::endl;
    std::cout << "The yref is " <<yref << std::endl;
    std::cout <<  squarewithgreatestdist << std::endl;


    while (/*xref != 0 && yref != 0*/ squarewithgreatestdist != 0) {
        if (canTravel(xref, yref, 0)) {
            if (distances[currCell] - distances[currCell + 1] == 1) {
                int right = currCell + 1;
                xref = xref + 1;
                currCell = right;
                path.push_back(2);
                std::cout <<  "right" << std::endl;

            }
        }
        if (canTravel(xref, yref, 1)) {
        
            if (distances[currCell] - distances[currCell + width_] == 1) {
               int down = currCell + width_;
                yref = yref + 1;
                currCell = down;
                path.push_back(3);
                 std::cout <<  "down" << std::endl;
            }
        }

        if(canTravel(xref, yref, 2)){
          if(distances[currCell] - distances[currCell - 1] == 1){
               int left = currCell - 1 ;
            xref = xref - 1;
            currCell = left;
            path.push_back(0);
             std::cout <<  "left" << std::endl;
          }
        }

        if(canTravel(xref, yref, 3)){
         
          if(distances[currCell] - distances[currCell - width_] == 1){
             int up = currCell - width_;
             yref = yref - 1;
             currCell = up;
            path.push_back(1);
             std::cout <<  "up" << std::endl;
          }
        }
        squarewithgreatestdist--;
         std::cout << squarewithgreatestdist << std::endl;

      }
       
    // Reverse the path to get the correct order
    reverse(path.begin(), path.end()); // The path is currently in reverse order, so reverse it to get the correct order
    
    // Return the path
    return path; // Return the path as a vector of integers
}



PNG* SquareMaze::drawMaze() const {
    // make the newImage
    int w = width_;
    int h = height_;
    PNG* newImage = new PNG(w*10+1, h*10+1);
    HSLAPixel blackPixel(0, 0, 0);

    // blacken top row and left column
    for (int i = 0; i < w*10+1; i++) {
        newImage->getPixel(0, i) = blackPixel;
    }

    for (int i = 0; i < h*10+1; i++) {
        if (i < 1 || i > 9) {
            newImage->getPixel(i, 0) = blackPixel;
        }
    }

    // Draw the maze
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int i = y * w + x;

            // Draw right wall
            if (maze_[i].right) {
                for (int k = 0; k < 11; k++) {
                    newImage->getPixel((x+1)*10, y*10+k) = blackPixel;
                }
            }

            // Draw down wall
            if (maze_[i].down) {
                for (int k = 0; k < 11; k++) {
                    newImage->getPixel(x*10+k, (y+1)*10) = blackPixel;
                }
            }
        }
    }

    return newImage;
}    



PNG* SquareMaze::drawMazeWithSolution() {
    PNG* picture = drawMaze(); // Creates a new PNG object from the maze
    vector<int> solution = solveMaze(); // Solves the maze and stores the solution in a vector
    pair<int, int> curr(5, 5); // Stores the current position to (5, 5)

    // Draw red where solution vector points to
    size_t i = 0; // Initialize loop counter
    while (i < solution.size()) {
        int direction = solution[i]; // Gets the direction to move in
        int x_offset = (direction == 0) ? 1 : (direction == 2) ? -1 : 0; // Computes the x offset based on the direction
        int y_offset = (direction == 1) ? 1 : (direction == 3) ? -1 : 0; // Computes the y offset based on the direction

        int x = 0; // Initialize loop counter
        while (x < 10) {
            HSLAPixel & pix = picture->getPixel(curr.first, curr.second); // Gets the current pixel
            pix.h = 0; // Sets the hue to 0, which is red
            pix.s = 1.0; // Sets the saturation to 1.0, which is fully saturated
            pix.l = 0.5; // Sets the lightness to 0.5, which is halfway between black and white

            curr.first += x_offset; // Updates the x coordinate of the current position
            curr.second += y_offset; // Updates the y coordinate of the current position

            x++; // Increment loop counter
        }

        i++; // Increment loop counter
    }

    // Draw exit marker
    HSLAPixel & pix = picture->getPixel(curr.first, curr.second); // Gets the current pixel
    pix.h = 0; // Sets the hue to 0, which is red
    pix.s = 1.0; // Sets the saturation to 1.0, which is fully saturated
    pix.l = 0.5; // Sets the lightness to 0.5, which is halfway between black and white
    curr.first -= 4; // Moves the x coordinate back by 4
    curr.second += 5; // Moves the y coordinate forward by 5

    int x = curr.first; // Initialize loop counter
    while (x < curr.first + 9) {
        HSLAPixel & exit_pix = picture->getPixel(x, height_ * 10); // Gets the current pixel
        exit_pix.l = 1.0; // Sets the lightness to 1.0, which is fully bright

        x++; // Increment loop counter
    }

    // Simulate longer runtime

    return picture; // Returns the modified PNG object
}


//Ali 