#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>

struct Circle {

	olc::vf2d pos;
	olc::vf2d a;
	olc::vf2d v;

	float m;
	int r;
};
// Override base class with your custom functionality
class PhysicsEngine : public olc::PixelGameEngine
{
private:
	float G = 10.0f;
	Circle circles[2];
	void displayCircles(Circle circArray[], int centerCircle = -1) {

		int size = sizeof(circles) / sizeof(Circle);
		for (int i = 0; i < size; i++) {

			//for relocation purposes			
			olc::vf2d renderPos = circArray[i].pos;

			//center on centerCircle unless -1;
			if (centerCircle != -1) {

				renderPos.y = renderPos.y - circArray[centerCircle].pos.y + (ScreenHeight() / 2);
				renderPos.x = renderPos.x - circArray[centerCircle].pos.x + (ScreenWidth() / 2);
			}
			
			//set +y to up
			renderPos.y = ScreenHeight() - renderPos.y;
			//draw
			FillCircle(renderPos, circArray[i].r, olc::WHITE);
		}
	}
	void updateCircles(Circle circArray[], float t) {

		system("CLS");
		int size = sizeof(circles) / sizeof(Circle);
		for (int i = 0; i < size; i++) {

			//update position
			circArray[i].pos.x = circArray[i].pos.x + (circArray[i].v.x * t) + (.5f * circArray[i].a.x * t * t);
			circArray[i].pos.y = circArray[i].pos.y + (circArray[i].v.y * t) + (.5f * circArray[i].a.y * t * t);

			//update velocity
			circArray[i].v.x = circArray[i].v.x + (circArray[i].a.x * t);
			circArray[i].v.y = circArray[i].v.y + (circArray[i].a.y * t);

			//do acceleration due to gravity
			circArray[i].a = { 0.0f, 0.0f };
			for (int j = 0; j < size; j++) {

				if (j != i) {

					float a = G * circArray[j].m;
					float deltaX = circArray[i].pos.x - circArray[j].pos.x;
					float deltaY = circArray[i].pos.y - circArray[j].pos.y;
					a /= (float)std::pow(deltaX, 2) + (float)std::pow(deltaY, 2);
					//a /= circArray[i].m;
					float angleTo = std::atanf(deltaY / deltaX);
					float aX = std::cosf(angleTo) * a;
					float aY = std::sinf(angleTo) * a;

					if (circArray[i].pos.x > circArray[j].pos.x) {

						aX *= -1.0f;
						aY *= -1.0f;
					}

					circArray[i].a.x += aX;
					circArray[i].a.y += aY;
				}
			}

			std::cout << "a_" << i << ": " << circArray[i].a.x << ", " << circArray[i].a.y << " >" << std::endl;
		}
	}
public:
	PhysicsEngine()
	{
		// Name you application
		sAppName = "Physics Circles";
	}

public:
	bool OnUserCreate() override
	{

		//sqrt(G * M / r) = v
		//G*M / v^2 = r
		// Called once at the start, so create things here'
		circles[0].r = 10;
		circles[0].pos = { ((float)ScreenWidth() / (float)2) - (float)200.0f, ((float)ScreenHeight() / (float)2) - (float)0.0 };
		circles[0].m = 100000.0f;
		circles[0].v = { 0.0f, -100.0f };
		circles[0].a = { 0.0f, 0.0f };

		circles[1].r = 60;
		circles[1].pos = { (float)ScreenWidth() / (float)2 , (float)ScreenHeight() / (float)2 };
		circles[1].m = 200000.0f;
		circles[1].v = { 0.0f, 0.0f };
		circles[1].a = { 0.0f, 0.0f };

		
		//circles[2].r = 1;
		//circles[2].pos = { ((float)ScreenWidth() / (float)2) - (float)220.0f, ((float)ScreenHeight() / (float)2) - (float)0.0 };
		//circles[2].m = 1.0f;
		//circles[2].v = { 0.0f, -110.47f };
		//circles[2].a = { 0.0f, 0.0f };
		
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//update circle physics.
		updateCircles(circles, fElapsedTime);
		//Clear screen and set background color
		Clear(olc::BLACK);
		//loop through all of the circles and display them
		displayCircles(circles, 1);
		//display Data
		//FillRect( 0, 0, 200, 100, olc::GREY);
		for (int i = 0; i < sizeof(circles) / sizeof(Circle); i++) {

			std::ostringstream out;
			out << "v_x" << i << ": ";
			out << int(circles[i].v.x * 100) / 100.0f << std::endl;

			out << "v_y" << i << ": ";
			out << int(circles[i].v.y * 100) / 100.0f << std::endl;

			out << "a_x" << i << ": ";
			out << int(circles[i].a.x * 100) / 100.0f << std::endl;

			out << "a_y" << i << ": ";
			out << int(circles[i].a.y * 100) / 100.0f;
			
			DrawString({ 10, 70 * i }, out.str(), olc::WHITE, 2);
		}
		return true;
	}
};

int main()
{
	PhysicsEngine demo;
	if (demo.Construct(1000, 1000, 1, 1))
		demo.Start();
	return 0;
}