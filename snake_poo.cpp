#include <iostream>
#include <vector>
#include <utility>
#include <array>
#include <thread>
#include "sys/ioctl.h"
#include "termios.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"

namespace internal
{
  int keyEvent();
  void frameSleep(const int &ms);
  const char *cmd_clear = "clear";
  void backgroundClear();
}

class Board;
class Snake;
class Food;

class Food {
public:
    std::pair<int, int> position_;

    Food(int x, int y) : position_{x, y} {}
};

class Snake {
public:
    std::vector<std::pair<int, int>> snake_;
    std::array<int, 2> direction_;

    Snake(int snake_len) : snake_(snake_len) {
        for (int i = 0; i < snake_len; i++) {
            snake_[i].first = 10 + snake_len - i;
            snake_[i].second = 10;
        }
    }

    int size() const {
        return snake_.size();
    }
    
    void change_direction(char key){
      if (key == 'z') {direction_ = {0, -1};}
      else if (key == 'd') {direction_ = {1, 0};}
      else if (key == 's') {direction_ = {0, 1};}
      else if (key == 'q') {direction_ = {-1, 0};};
    }

    bool eat_food(Food &food)
    {
      if ((food.position_ == snake_[0]))
      {
        food.position_.first = 0;
        food.position_.second = 0;
        return true;
      }
      return false;
    }

    void update_coordinates(bool eat)
    {
      auto [dx,dy] = direction_;
      auto [head_x,head_y] = snake_[0];
      snake_.insert(snake_.begin(), {head_x + dx,head_y + dy});
      if (!eat) {snake_.pop_back();};
    }
};

class Board{
    public:
        const int nx_;
        const int ny_;
        std::vector<int> bg_;

    Board(int nx, int ny) : nx_{nx},ny_{ny},bg_(nx_ * ny_,0) {};

    void add_walls(){
        for (int i = 0; i < nx_; i++) {
            bg_[i] = 1;
            bg_[(ny_ - 1) * nx_ + i] = 1;
        }
        for (int i = 0; i < ny_; i++) {
            bg_[i * nx_] = 1;
            bg_[i * nx_ + nx_ - 1] = 1;
        }
    }

    void add_snake(const Snake &snake)
    {     for (int i = 0; i < snake.size(); i++) 
      {bg_[snake.snake_[i].second * nx_ + snake.snake_[i].first] = 3;}
    }

    void remove_snake(const Snake &snake)
    {     for (int i = 0; i < snake.size(); i++) 
      {bg_[snake.snake_[i].second * nx_ + snake.snake_[i].first] = 0;}
    }

    bool is_out(const Snake &snake) 
    {    const auto head = snake.snake_[0];
    return (head.first == 0 | head.first == nx_ - 1 | head.second == 0 | head.second == ny_ - 1);
    }

    bool is_superposed(const Snake &snake)
    {  bool is_superposed = false;
    for (int i = 1; i < snake.size(); i++) {
      if (snake.snake_[i] == snake.snake_[0]) {is_superposed = true;}
      };
    return is_superposed;
    }

    void printFrame()  {
      for (int j = 0; j < ny_; j++)
      {
        for (int i = 0; i < nx_; i++)
        {
          if (bg_[i + j * nx_] == 1)
          {
            std::cout << "#";
          }
          else if (bg_[i + j * nx_] == 2)
          {
            std::cout << "@";
          }
          else if (bg_[i + j * nx_] == 3)
          {
            std::cout << "o";
          }
          else
          {
            std::cout << " ";
          }
        }
        std::cout << std::endl;
      }
    }

    Food createFood(Food food)
    {
      bool created = false;
      while(!created) {
          int x = rand() % (nx_ - 2) + 1;
          int y = rand() % (ny_ - 2) + 1;
          if (bg_[y * nx_ + x] == 0){
          food.position_ = {x,y};
          bg_[food.position_.second * nx_ + food.position_.first] = 2;
          created = true;}
      } 
      return food;
    }

};


void startGame(const int &lap, Board &board, Snake &snake, int gamemode){
    snake.direction_ = {1, 0};
    Food food (0,0);
    food = board.createFood(food);

  while (true)
  {
    char key;
    internal::frameSleep(lap);
    if (internal::keyEvent())
    {
      std::cin >> key;
      snake.change_direction(key);
    }
    internal::backgroundClear();
    board.add_snake(snake);
    board.printFrame();
    board.remove_snake(snake);
    bool is_out = board.is_out(snake);
    bool is_superposed = board.is_superposed(snake);
    if ( is_superposed | (is_out & gamemode == 0)) //mode avec border
    { // Game Over
      std::cerr << "" << std::endl;
      exit(1);
    }
    else if (is_out & gamemode == 1) 
    { 
      auto [dx,dy] = snake.direction_;
      auto [head_x,head_y] = snake.snake_[0];
      board.bg_[head_y*board.nx_ + head_x] = 1;
      if (dx == 0 & dy == 1) {snake.snake_[0].second = board.ny_ - head_y;}
      else if (dx == 0 & dy == -1) {snake.snake_[0].second = board.ny_ - head_y -2;}
      else if (dx == 1 & dy == 0) {snake.snake_[0].first = board.nx_ - head_x;}
      else if (dx == -1 & dy == 0) {snake.snake_[0].first = board.nx_ - head_x -2;}
    }
    bool eat = snake.eat_food(food);
    if (eat){food = board.createFood(food);}
    snake.update_coordinates(eat);
  }
}

int main()
{
  const int nx = 50;
  const int ny = 25;
  const int lap = 200;
  int snake_len = 3;
  int gamemode = 0; // 0 pour le mode habituel, et 1 pour un mode ou quand ça touche un bord, il se teleporte à l'autre coté

  Board board = Board(nx,ny);
  board.add_walls();
  //board.printFrame();

  Snake snake = Snake(snake_len);

  startGame(lap,board,snake, gamemode);
  return 0;
}

namespace internal
{

  static const int STDIN = 0;
  static bool initialized = false;

  // Comment ca va

  int keyEvent()
  {
    if (!initialized)
    {
      termios term;
      tcgetattr(STDIN, &term);
      term.c_lflag &= ~ICANON;
      tcsetattr(STDIN, TCSANOW, &term);
      setbuf(stdin, NULL);
      initialized = true;
    }
    int bytesWaiting;
    // int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
  }

  void frameSleep(const int &ms)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }

  /////////////

  void backgroundClear()
  {
    int out = system(cmd_clear);
    if (out != 0)
    {
      std::cerr << "clear command failed" << std::endl;
      exit(1);
    }
  }
}