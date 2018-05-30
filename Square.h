#pragma once
#include <GL/glut.h>

class Square{
public:
	Square(int x, int y, float r, float g, float b, float sl) {
		pos[0] = x;
		pos[1] = y;
		color[0] = r;
		color[1] = g;
		color[2] = b;
		side = sl;
	}

	void draw() const {
		glBegin(GL_POLYGON);
		glColor3f(color[0], color[1], color[2]);
		glVertex2f(pos[0] + 0.5 + side / 2, pos[1] + 0.5 + side / 2);
		glVertex2f(pos[0] + 0.5 - side / 2, pos[1] + 0.5 + side / 2);
		glVertex2f(pos[0] + 0.5 - side / 2, pos[1] + 0.5 - side / 2);
		glVertex2f(pos[0] + 0.5 + side / 2, pos[1] + 0.5 - side / 2);
		glEnd();
	}
	void setPos(float x, float y) {
		pos[0] = x;
		pos[1] = y;
	}
	void setColor(float r, float g, float b) {
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}
	int getX() {
		return pos[0];
	}
	int getY() {
		return pos[1];
	}
	int getside() {
		return side;
	}
private:
	float side;
	float pos[2];
	float color[3];
};