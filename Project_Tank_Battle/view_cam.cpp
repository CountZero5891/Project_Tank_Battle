#include "view_cam.h"



void setPlayerCoordinateForView(float x, float y)
{
	
	float tempX = x; float tempY = y;
	//if (x < 320) tempX = 320;
	//if (y < 240) tempY = 240;//������� �������
	if (y > 624) tempY = 624;//������ �������.��� ����� �����

	view.setCenter(tempX, tempY);
}