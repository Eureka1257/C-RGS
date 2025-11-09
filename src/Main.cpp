#include<iostream>
#include "Application.h"



int main()
{

	RGS::Application app("RGS" , 400 , 300);

	app.Run();

	std::cout<<"Hello RGS"<<std::endl;
	return 0;
}