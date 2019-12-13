// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//--- Day 11: Space Police ----
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"
#include "Program.h"

enum Tile
{
  empty, //0 is an empty tile.No game object appears in this tile.
  wall,  //1 is a wall tile.Walls are indestructible barriers.
  block, //2 is a block tile.Blocks can be broken by the ball.
  horizontal_paddle, //3 is a horizontal paddle tile.The paddle is indestructible.
  ball //4 is a ball tile.The ball moves diagonally and bounces off objects.
};

const char* chars = " #._*";
struct Solve {
  Program p = Program({}, {0});

  Solve(string inStr)
  {
    static const regex colsRxToken(",");
    forEachRxToken(inStr, colsRxToken, [&](string instr) {
      p.instructions.push_back(atoll(instr.c_str()));
      });
  };

  int Do2(const vector<int_t>& a)
  {
    int score;
    p.instructions[0] = 2;
    Point ballPoint, paddlePoint;
    for (p.Run(3); p.output.size() == 3; p.Run(3))
    {
      Tile id = (Tile)p.output[2];
      Point pt{ p.output[1], p.output[0], 0 };
      (pt == Point{ 0, -1, 0 })?(toConsole(Point{ 10, 70,0 }, "Score: " + to_string(id)), score = id):0, toConsole(pt, &chars[id], 1, id == ball ? -1 : -1);
      id == ball ? ballPoint = pt : (id == horizontal_paddle ? paddlePoint = pt : Point());
      p.output.clear(), p.regA[0] = Compare(ballPoint.x, paddlePoint.x, -1, 0, 1);
    }
    return score;
  }
};

TEST_CASE("New2", "[x.]") {
  //Solve(ReadFileToString(L"input.txt")).Do2({ 1 });
  toClipboard((int)Solve(ReadFileToString(L"input.txt")).Do2({ 0 }));

}

/* Input ---------------

1,380,379,385,1008,2389,754058,381,1005,381,12,99,109,2390,1102,1,0,383,1102,1,0,382,21002,382,1,1,21001,383,0,2,21101,37,0,0,1106,0,578,4,382,4,383,204,1,1001,382,1,382,1007,382,35,381,1005,381,22,1001,383,1,383,1007,383,25,381,1005,381,18,1006,385,69,99,104,-1,104,0,4,386,3,384,1007,384,0,381,1005,381,94,107,0,384,381,1005,381,108,1106,0,161,107,1,392,381,1006,381,161,1102,1,-1,384,1105,1,119,1007,392,33,381,1006,381,161,1102,1,1,384,20102,1,392,1,21102,1,23,2,21101,0,0,3,21101,0,138,0,1105,1,549,1,392,384,392,21001,392,0,1,21102,1,23,2,21102,1,3,3,21102,161,1,0,1106,0,549,1102,1,0,384,20001,388,390,1,20102,1,389,2,21101,0,180,0,1106,0,578,1206,1,213,1208,1,2,381,1006,381,205,20001,388,390,1,20101,0,389,2,21101,205,0,0,1106,0,393,1002,390,-1,390,1101,1,0,384,21002,388,1,1,20001,389,391,2,21102,228,1,0,1105,1,578,1206,1,261,1208,1,2,381,1006,381,253,21002,388,1,1,20001,389,391,2,21101,253,0,0,1105,1,393,1002,391,-1,391,1101,0,1,384,1005,384,161,20001,388,390,1,20001,389,391,2,21101,0,279,0,1105,1,578,1206,1,316,1208,1,2,381,1006,381,304,20001,388,390,1,20001,389,391,2,21101,0,304,0,1106,0,393,1002,390,-1,390,1002,391,-1,391,1102,1,1,384,1005,384,161,21002,388,1,1,20102,1,389,2,21101,0,0,3,21101,338,0,0,1106,0,549,1,388,390,388,1,389,391,389,20102,1,388,1,21001,389,0,2,21102,4,1,3,21101,365,0,0,1106,0,549,1007,389,24,381,1005,381,75,104,-1,104,0,104,0,99,0,1,0,0,0,0,0,0,309,15,20,1,1,17,109,3,22101,0,-2,1,21202,-1,1,2,21101,0,0,3,21102,1,414,0,1106,0,549,22101,0,-2,1,22102,1,-1,2,21101,429,0,0,1106,0,601,2102,1,1,435,1,386,0,386,104,-1,104,0,4,386,1001,387,-1,387,1005,387,451,99,109,-3,2105,1,0,109,8,22202,-7,-6,-3,22201,-3,-5,-3,21202,-4,64,-2,2207,-3,-2,381,1005,381,492,21202,-2,-1,-1,22201,-3,-1,-3,2207,-3,-2,381,1006,381,481,21202,-4,8,-2,2207,-3,-2,381,1005,381,518,21202,-2,-1,-1,22201,-3,-1,-3,2207,-3,-2,381,1006,381,507,2207,-3,-4,381,1005,381,540,21202,-4,-1,-1,22201,-3,-1,-3,2207,-3,-4,381,1006,381,529,22102,1,-3,-7,109,-8,2106,0,0,109,4,1202,-2,35,566,201,-3,566,566,101,639,566,566,2102,1,-1,0,204,-3,204,-2,204,-1,109,-4,2105,1,0,109,3,1202,-1,35,593,201,-2,593,593,101,639,593,593,21002,0,1,-2,109,-3,2105,1,0,109,3,22102,25,-2,1,22201,1,-1,1,21101,439,0,2,21102,399,1,3,21101,0,875,4,21101,630,0,0,1105,1,456,21201,1,1514,-2,109,-3,2106,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,2,2,0,2,0,0,2,2,2,0,2,2,2,0,2,2,0,2,2,2,2,0,2,0,0,2,2,0,2,2,0,1,1,0,0,0,0,2,2,2,0,0,2,0,2,0,2,0,2,2,0,0,0,0,0,2,2,2,0,2,2,2,0,0,2,0,1,1,0,0,0,2,0,0,2,2,2,0,2,0,2,2,0,0,2,2,0,0,0,2,2,2,0,2,2,2,2,2,2,2,0,1,1,0,0,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,0,2,2,2,0,2,2,2,2,2,0,2,2,2,0,0,1,1,0,0,0,2,0,2,0,0,0,2,0,2,2,2,0,0,2,2,2,2,2,2,0,2,2,0,0,2,2,0,0,2,0,1,1,0,2,0,2,0,0,2,0,2,2,2,2,2,0,2,2,2,2,2,2,2,0,2,2,0,2,0,0,0,2,2,0,0,1,1,0,0,2,2,0,2,2,2,0,2,2,0,0,2,2,2,2,0,0,2,0,2,2,2,0,2,0,0,2,0,2,2,0,1,1,0,2,2,2,0,2,0,2,2,2,0,2,2,2,2,2,0,0,0,2,0,2,2,2,0,2,2,0,0,2,0,2,0,1,1,0,2,2,2,2,2,0,0,2,2,2,0,2,0,2,2,2,0,0,0,0,0,2,0,0,0,0,2,2,2,2,2,0,1,1,0,0,0,2,2,2,0,0,0,0,2,2,0,2,2,0,2,2,2,2,0,2,2,2,0,2,2,0,0,2,2,2,0,1,1,0,2,2,0,2,0,2,2,2,0,2,2,0,2,0,2,2,2,2,2,2,2,2,0,0,2,0,2,0,0,0,0,0,1,1,0,0,0,2,2,2,0,2,0,0,2,2,2,0,0,2,0,2,2,0,2,0,0,0,2,0,0,2,2,0,0,2,0,1,1,0,0,2,2,0,2,0,2,2,0,2,2,2,2,2,0,2,2,0,2,2,0,2,0,0,2,0,2,2,2,0,2,0,1,1,0,0,0,2,2,0,2,2,0,2,2,2,0,0,2,2,2,2,0,0,2,0,2,0,2,2,0,2,2,0,0,0,0,1,1,0,0,2,2,2,0,2,0,2,2,2,2,0,0,0,0,2,0,2,0,0,2,0,2,2,2,2,2,0,0,2,0,0,1,1,0,2,2,2,0,2,2,0,0,0,0,0,2,2,2,2,0,2,2,0,2,2,0,2,0,0,2,2,0,0,0,0,0,1,1,0,2,0,2,2,0,0,0,2,0,2,0,0,2,0,0,0,2,0,0,2,2,0,0,0,0,2,2,2,2,0,2,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,57,24,40,42,15,20,34,98,57,80,65,54,51,27,26,52,50,56,60,96,87,6,28,66,17,18,57,43,65,35,26,40,60,37,42,55,15,6,84,50,59,56,5,64,25,49,28,34,91,39,8,65,98,74,51,56,74,40,84,83,69,52,58,92,82,14,28,61,68,25,7,62,69,85,82,38,60,82,31,93,26,62,39,93,23,56,84,30,37,64,93,76,10,44,14,97,63,55,4,80,72,93,41,1,73,17,5,13,35,79,82,23,69,16,48,2,92,46,11,42,78,66,90,90,17,95,85,76,77,85,72,84,67,47,71,61,74,93,97,52,40,21,65,31,96,18,84,59,4,24,62,65,38,71,98,20,35,25,31,36,88,38,36,17,20,25,56,15,39,57,54,1,31,72,8,54,42,72,1,33,70,87,37,95,19,80,93,79,38,56,29,58,16,13,26,11,55,18,34,81,84,97,66,48,92,21,34,76,91,48,38,79,2,66,82,37,73,91,21,91,20,42,77,95,23,76,44,29,49,95,38,5,38,92,52,96,47,9,76,95,9,72,5,16,25,40,11,95,94,5,16,30,28,25,78,81,42,19,79,71,77,63,64,7,73,13,53,74,28,32,97,42,18,63,77,95,9,85,18,33,19,70,2,12,15,93,9,20,47,70,15,6,98,9,10,47,25,85,11,60,75,60,11,63,16,4,76,37,65,16,40,18,3,32,18,40,84,97,4,52,34,50,74,1,6,38,91,37,38,43,91,61,90,82,57,12,1,34,43,18,57,19,41,60,38,42,89,62,19,65,12,6,32,8,61,19,80,23,79,20,21,73,71,89,33,62,14,97,60,10,70,76,50,72,39,33,4,62,97,78,2,81,92,46,76,43,74,83,96,14,81,95,27,85,32,93,73,68,20,57,43,8,88,13,61,86,67,31,38,94,96,27,90,19,70,40,38,23,89,36,26,5,54,37,73,28,23,2,21,68,34,46,11,63,66,74,4,37,23,14,51,20,10,50,18,65,28,66,58,70,45,96,24,58,25,59,76,79,26,76,8,50,85,39,90,59,90,79,3,4,13,54,30,14,42,82,31,64,49,82,75,54,48,88,34,23,98,93,66,36,83,15,67,43,22,72,25,32,63,86,12,43,40,85,79,29,23,25,42,10,58,51,55,21,73,85,69,29,88,98,41,97,51,52,77,49,84,45,98,5,87,77,65,80,79,76,71,4,85,96,66,80,97,27,41,62,16,61,92,70,52,44,62,10,59,61,26,9,58,69,60,46,94,24,91,80,90,24,80,94,33,71,4,37,58,55,66,56,87,87,38,11,57,55,40,93,37,22,26,12,25,95,34,93,67,14,36,98,34,67,49,7,34,4,63,77,93,39,23,5,60,22,76,19,54,65,32,77,47,43,94,10,94,48,29,48,48,72,70,8,14,88,74,98,65,4,73,64,74,72,56,40,37,74,37,67,26,33,97,33,92,63,17,32,60,17,82,41,21,69,40,95,37,76,39,14,92,14,96,3,26,1,78,53,51,73,69,8,93,64,52,96,52,75,91,52,11,91,62,73,7,67,29,89,74,70,76,41,95,6,13,5,8,16,52,33,39,78,79,65,87,97,64,88,42,29,37,61,26,28,83,28,26,93,13,19,38,85,80,53,73,7,82,54,32,12,94,94,53,25,44,28,25,70,25,88,82,10,49,87,86,82,11,90,65,68,2,97,13,3,74,45,19,90,22,16,20,22,42,61,21,26,26,89,93,11,48,90,83,59,21,41,82,93,48,62,35,35,55,20,6,58,62,20,77,10,18,46,16,70,8,65,87,19,47,72,8,82,40,5,60,55,48,2,18,60,4,39,67,65,25,70,6,4,91,93,79,52,25,83,23,56,52,12,89,65,64,82,79,62,88,72,9,57,38,30,19,33,19,8,9,39,89,72,97,17,35,5,19,79,78,20,29,63,11,30,49,78,55,3,68,64,51,83,70,754058

 ------------Input End*/
