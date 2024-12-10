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
  // 💀💀💀💀💀💀💀💀 DON'T TOUCH THIS BLOCK 💀💀💀💀💀💀💀💀💀 ///
  int keyEvent();
  void frameSleep(const int &ms);
  const char *cmd_clear = "clear";
  void backgroundClear();
  void printFrame(const int &nx, const int &ny, const std::vector<int> &bg);
  void createFood(std::vector<int> &bg, std::array<int, 2> &food, const int &nx, const int &ny);
  bool eatFood(std::array<int, 2> &food, std::vector<std::pair<int, int>> &snake);
  // 💀💀💀💀💀💀💀💀 DON'T TOUCH THIS BLOCK 💀💀💀💀💀💀💀💀💀 ///
}

// 😇 Je suis gentil je vous ai fait le tracé du terrain 😇
std::vector<int> backgroundSetup(const int &nx, const int &ny)
{
  std::vector<int> bg(nx * ny, 0);
  int i;
  for (i = 0; i < nx; i++)
  {
    bg[i] = 1;
  }
  for (i = 0; i < nx; i++)
  {
    bg[(ny - 1) * nx + i] = 1;
  }
  for (i = 0; i < ny; i++)
  {
    bg[i * nx] = 1;
  }
  for (i = 0; i < ny; i++)
  {
    bg[i * nx + nx - 1] = 1;
  }

  return bg;
}

///////////////////////////////////////////////////////////////////////////////
/////// 🧑‍💻 FROM HERE you must complete 🧑‍💻 ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void add_snake(const std::vector<std::pair<int, int>> &snake, std::vector<int> &bg, int nx, int ny)
{
  // 👉️ Your code here 👈️
for (int i = 0; i < snake.size(); i++) {
    bg[snake[i].second * nx + snake[i].first] = 3;
}
}

void remove_snake(const std::vector<std::pair<int, int>> &snake, std::vector<int> &bg, int nx, int ny)
{
  // 👉️ Your code here 👈️
  for (int i = 0; i < snake.size(); i++) {
    bg[snake[i].second * nx + snake[i].first] = 0;
}
}

std::array<int, 2> snake_movement(char key)
{
  // 👉️ Your code here 👈️
  std::array<int, 2> a = {0, 0};
  if (key == 'z') {a = {0, -1};}
  else if (key == 'd') {a = {1, 0};}
  else if (key == 's') {a = {0, 1};}
  else if (key == 'q') {a = {-1, 0};};
return a;
}

bool verifyBorder(const std::vector<std::pair<int, int>> &snake, int nx, int ny)
{
  // 👉️ Your code here 👈️
  bool is_border = true;
    if (snake[0].first == 0 or snake[0].first == nx-1 or snake[0].second == 0 or snake[0].second == ny-1) {is_border = false;}
return is_border;
}

bool verifySnakeSuperposition(const std::vector<std::pair<int, int>> &snake, int nx, int ny)
{
  // 👉️ Your code here 👈️
  bool not_superposed = true;
  for (int i = 1; i < snake.size(); i++) {
    if (snake[i] == snake[0]) {not_superposed = false;}
};
return not_superposed;
}

std::vector<std::pair<int, int>> setupSnake(int snake_len)
{
  std::vector<std::pair<int, int>> snake(snake_len);
  int i;
  for (i = 0; i < snake_len; i++)
  {
    snake[i].first = 10 + snake_len - i;
    snake[i].second = 10;
  }
  return snake;
}

void update_snake_coordinates(std::vector<std::pair<int, int>> &snake, bool eat, std::array<int, 2> dxdy)
{
  // 👉️ Your code here 👈️
  auto [dx,dy] = dxdy;
  auto [head_x,head_y] = snake[0];
  snake.insert(snake.begin(), {head_x + dx,head_y + dy});
  if (!eat) {snake.pop_back();};
}

void startGame(const int &lap, const int &nx, const int &ny, std::vector<std::pair<int, int>> &snake, std::vector<int> &bg, int gamemode)
{
  float modified_lap = lap;
  char key;
  std::array<int, 2> dxdy = {1, 0};
  std::array<int, 2> food = {0, 0};
  internal::createFood(bg, food, nx, ny);
  while (true)
  {
    internal::frameSleep(modified_lap);
    modified_lap = modified_lap*0.999;
    if (internal::keyEvent())
    {
      std::cin >> key;
      dxdy = snake_movement(key);
    }
    internal::backgroundClear();
    add_snake(snake, bg, nx, ny);
    internal::printFrame(nx, ny, bg);
    remove_snake(snake, bg, nx, ny);
    bool out = verifyBorder(snake, nx, ny);
    bool superposed = verifySnakeSuperposition(snake,nx,ny);
    if ( superposed == false | (out == false & gamemode == 0)) //mode avec border
    {
      std::cerr << "" << std::endl;
      exit(1);
    }
    else if (out == false & gamemode == 1){ //mode sans border
      auto [dx,dy] = dxdy;
      auto [head_x,head_y] = snake[0];
      bg[head_y*nx + head_x] = 1;
      if (dx == 0 & dy == 1) {snake[0].second = ny - head_y;}
      else if (dx == 0 & dy == -1) {snake[0].second = ny - head_y -2;}
      else if (dx == 1 & dy == 0) {snake[0].first = nx - head_x;}
      else if (dx == -1 & dy == 0) {snake[0].first = nx - head_x -2;}
    }
    bool eat = internal::eatFood(food, snake);
    if (eat)
    {
      internal::createFood(bg, food, nx, ny);
    }
    update_snake_coordinates(snake, eat, dxdy);
  }
}

int main()
{
  const int nx = 50;
  const int ny = 25;
  const int lap = 200;
  int snake_len = 3;
  int gamemode = 1;

  std::vector<int> background = backgroundSetup(nx, ny);
  internal::printFrame(nx, ny, background);

  std::vector<std::pair<int, int>> snake = setupSnake(snake_len);

  startGame(lap, nx, ny, snake, background, gamemode);
  return 0;
}

// 💀💀💀💀💀💀💀💀 DON'T TOUCH CODE AFTER THIS LINE 💀💀💀💀💀💀💀💀💀💀 ///

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

  void printFrame(const int &nx, const int &ny, const std::vector<int> &bg)
  {
    for (int j = 0; j < ny; j++)
    {
      for (int i = 0; i < nx; i++)
      {
        if (bg[i + j * nx] == 1)
        {
          std::cout << "#";
        }
        else if (bg[i + j * nx] == 2)
        {
          std::cout << "@";
        }
        else if (bg[i + j * nx] == 3)
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

  void createFood(std::vector<int> &bg, std::array<int, 2> &food, const int &nx, const int &ny)
  {
    if (food[0] == 0)
    {
      food[0] = rand() % (nx - 2) + 2;
      food[1] = rand() % (ny - 2) + 2;
      bg[food[1] * nx + food[0]] = 2;
    }
  }

  bool eatFood(std::array<int, 2> &food, std::vector<std::pair<int, int>> &snake)
  {
    if ((food[0] == std::get<0>(snake[0])) && (food[1] == std::get<1>(snake[0])))
    {
      food[0] = 0;
      food[1] = 0;
      return true;
    }
    return false;
  }

}
