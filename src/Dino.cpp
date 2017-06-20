#include "stdafx.h"
#include "Dino.h"
#include "Utils.h"
#include <iostream>


Dino::Dino(const PointStruct& dinoPos):_dinoPos(dinoPos)
{
}


Dino::~Dino()
{
}


void Dino::Jump()
{
	std::cout << "JUMP" << std::endl;
	ReleaseDownKey();
	PressUpKey();
}

void Dino::Crawl()
{
	std::cout << "CRAWL" << std::endl;
	HoldDownKey();
}