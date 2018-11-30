// LedCubeTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <cmath>
class Point
{
public:
	//координаты x height y width z depth, верхний левый передний угол будет 0 0 0
	int8_t x;
	int8_t y;
	int8_t z;
	bool Equals(Point otherPoint);
};
bool Point::Equals(Point otherPoint)//сравнение одной точки с другой
{
	if ((x == otherPoint.x) && (y == otherPoint.y) && (z == otherPoint.z))
	{
		return true;
	}
	else
	{
		return false;
	}
}



class Cube
{
public:
	Point points[4]; //точки - 4 из восьми - передний левый верхний угол, передний верхний правый, задний нижний левый, задний нижний правый
	Point fullPoints[2][4]; //верхняя грань по часовой начиная с переднего левого, нижняя тот же принцип
	void getFullPoints();
	Cube(Point newPoints[2]); //передаешь верхний передний левый и задний нижний правый
	Cube(Point newPoints[4]); //передаешь все точки
	bool Equals(Cube cube);
	int8_t getSide();
private:
	int8_t side;
};
Cube::Cube(Point newPoints[2]) //передаешь верхний передний левый и задний нижний правый
{
	if (newPoints[1].x >= newPoints[0].x)
	{
		side = newPoints[1].x - newPoints[0].x;
	}
	else
	{
		side = newPoints[0].x - newPoints[1].x;
	}
	points[0] = newPoints[0];
	points[3] = newPoints[1];

	points[1].x = newPoints[0].x;
	points[1].y = newPoints[1].y;
	points[1].z = newPoints[0].z;

	points[2].x = newPoints[1].x;
	points[2].y = newPoints[0].y;
	points[2].z = newPoints[1].z;

	getFullPoints();
}
Cube::Cube(Point newPoints[4]) //передаешь все точки
{
	if (newPoints[3].x >= newPoints[1].x)
	{
		side = newPoints[3].x - newPoints[1].x;
	}
	else
	{
		side = newPoints[1].x - newPoints[3].x;
	}
	for (int i = 0; i < 4; i++)
	{
		points[i] = newPoints[i];
	}
	getFullPoints();
}
void Cube::getFullPoints()
{
	//верхние 4 точки
	fullPoints[0][0] = points[0];

	fullPoints[0][1].x = points[0].x;
	fullPoints[0][1].y = points[0].y;
	fullPoints[0][1].z = points[0].z+side;

	fullPoints[0][2].x = points[0].x;
	fullPoints[0][2].y = points[0].y + side;
	fullPoints[0][2].z = points[0].z + side;

	fullPoints[0][3] = points[1];

	//нижние 4 точки
	fullPoints[1][1] = points[2];

	fullPoints[1][0].x = points[2].x;
	fullPoints[1][0].y = points[2].y;
	fullPoints[1][0].z = points[2].z - side;

	fullPoints[1][3].x = points[3].x;
	fullPoints[1][3].y = points[3].y;
	fullPoints[1][3].z = points[3].z - side;

	fullPoints[0][2] = points[3];
}
bool Cube::Equals(Cube cube) //Сравнение кубов
{
	if (points[0].Equals(cube.points[0]) && side == cube.getSide())
	{
		return true;
	}
	else
	{
		return false;
	}
}
int8_t Cube::getSide()
{
	return side;
}



class LedCube
{
public:
	int8_t leds[4][4][4];
	LedCube();
	void Woop(Cube cubes[2]);
private:
	void init();
	Point litLeds[64];
	int8_t litLedsCount;
	int8_t findCorner(Cube cubes[2]);
	void lightCube(Cube cube);
	void lightLed(int8_t x, int8_t y, int8_t z);
	void addLitLed(int8_t x, int8_t y, int8_t z);
	void clearLitLeds();
};
void LedCube::clearLitLeds() //иду по списку горящих ледов и выключаю их
{
	for (int i = 0; i < litLedsCount; i++)
	{
		leds[litLeds[i].x][litLeds[i].y][litLeds[i].z] = 0;
		litLedsCount = 0;
	}
}
void LedCube::addLitLed(int8_t x, int8_t y, int8_t z)//добавляю точку в список горящих ледов
{
	litLeds[litLedsCount].x = x;
	litLeds[litLedsCount].y = y;
	litLeds[litLedsCount++].z = z;
}
void LedCube::lightLed(int8_t x, int8_t y, int8_t z)
{
	leds[x][y][z] = 1;
	addLitLed(x,y,z);
}
void LedCube::lightCube(Cube cube) //зажигаю куб
{
	int8_t side = cube.getSide();
	if (side = 0)
	{
		lightLed(cube.points[0].x, cube.points[0].y, cube.points[0].z);
	}
	else
	{
		Point FTL = cube.points[0];
		//верхние грани
		for (int i = 0; i <= side;i++)
		{
			lightLed(FTL.x, FTL.y + i, FTL.z);
			lightLed(FTL.x, FTL.y + i, FTL.z + side);
		}
		for (int i = 1;i < side; i++)
		{
			lightLed(FTL.x, FTL.y, FTL.z + i);
			lightLed(FTL.x, FTL.y+side, FTL.z + i);
		}
		//Вертикальные грани
		for (int i = 1;i <= side; i++)
		{
			lightLed(FTL.x + i, FTL.y, FTL.z);
			lightLed(FTL.x + i, FTL.y + side, FTL.z);
			lightLed(FTL.x + i, FTL.y, FTL.z + side);
			lightLed(FTL.x + i, FTL.y + side, FTL.z + side);
		}
		//нижние грани
		for (int i = 1;i < side; i++)
		{
			lightLed(FTL.x + side, FTL.y+ i, FTL.z);
			lightLed(FTL.x + side, FTL.y + i, FTL.z+ side);
			lightLed(FTL.x + side, FTL.y, FTL.z+ i);
			lightLed(FTL.x + side, FTL.y+ side, FTL.z + i);
		}
	}
}
int8_t LedCube::findCorner(Cube cubes[2])//нахожу общий угл, точку которая будет гореть в любом случае, возвращаю ее позицию в массиве
{
	int8_t corN = 9;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (cubes[0].fullPoints[x][y].Equals(cubes[1].fullPoints[x][y]))
			{
				corN = 4 * x + y;
			}
		}
	}
	return corN;
}
void LedCube::init()//
{
	for (int x = 0; x < 4;x++)
	{
		for (int y = 0; y < 4;y++)
		{
			for (int z = 0; z < 4;z++)
			{
				leds[x][y][z] = 0;
			}
		}
	}
	litLedsCount = 0;
}
LedCube::LedCube()
{
	init();
}
void LedCube::Woop(Cube cubes[2])
{
	int8_t cc = findCorner(cubes);
	if (cubes[0].getSide() > cubes[1].getSide())
	{
		int8_t sideDiff = cubes[0].getSide() - cubes[1].getSide();
	}
	else
	{
		int8_t sideDiff = cubes[1].getSide() - cubes[0].getSide();
	}
	switch (cc)
	{
	case 0:
		lightCube(cubes[0]);
		lightCube(cubes[1]);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	default:
		break;
	}
}



int main()
{
    
}
