#include<iostream>
using namespace std;

class Vehicle
{
	int speed;
public:
	virtual void move() = 0;
};

class GroundVehicle : public Vehicle {};

class Car :public GroundVehicle
{
public:
	void move()
	{
		cout << "Машина едит на 4-х колесах" << endl;
	}
};

class Bike :public GroundVehicle
{
public:
	void move()
	{
		cout << "Мотоцикл едит на 2-х колесах" << endl;
	}
};

class AirVehicle : public Vehicle {};

class Plane : public AirVehicle {
	public:
	void move()
	{
		cout << "Взлетаем" << endl;
	}
};

class Helicopter : public AirVehicle {
	public:
	void move()
	{
		cout << "вуп-вуп-вуп" << endl;
	}
};

class WaterVehicle : public Vehicle {};

class Boat : public WaterVehicle {
	public:
	void move()
	{
		cout << "Плывем" << endl;
	}
};

class Yacht : public WaterVehicle {
	public:
	void move()
	{
		cout << "отдыхаем" << endl;
	}
};