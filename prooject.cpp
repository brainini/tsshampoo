#include <time.h>

using namespace std;

#include <iostream>
#include <sstream>
#include "Square.h"		//간략화한 square.h

#define w 600		// window's width
#define h 600		// window's height
#define gw 200		// grid's width     
#define gh 200		// grid's height

#define ter 1	// territory
#define tra 2	// trace
#define ano 3	// another

int level = 0;
float arrowy = gh*2/3;
int lose = 0;
int win = 0;
int life = 3;
int sp_key;				// special key
float sl = 1;		//sidelength이자 스피드
int prop[gw][gh];		//grid 각 격자의 속성
clock_t start = clock();
clock_t endd;
float ratio = 0;	// ratio of area
int color[40000];	// color array for another
int curr_color = ano;	// current color
Square sq(gw/2, gh / 2, 1, 0, 0, sl*2); // grid 중앙에 side lenght 1짜리

void draw_string(void * font, const char* str, float x, float y) {
	glRasterPos2f(x, y);
	for (int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
}

void floodfill(int x, int y, int target, int replace) {
	if (target == replace)  return;
	if (prop[x][y] != target) return;
	prop[x][y] = replace;
	if (y != 0) floodfill(x, y - 1, target, replace);
	else color[replace] = -1;
	if (y != gh - 1) floodfill(x, y + 1, target, replace);
	else color[replace] = -1;
	if (x != 0) floodfill(x - 1, y, target, replace);
	else color[replace] = -1;
	if (x != gw - 1) floodfill(x + 1, y, target, replace);
	else color[replace] = -1;
	return;
}

void init() {
	for (int i = gw / 2 - 5; i < gw / 2 + 5; i++) for (int j = gh / 2 - 5; j < gh / 2 + 5; j++) prop[i][j] = ter;
}

void processNormalKeys(unsigned char key, int x, int y) {
	if (key == 13) {
		glutPostRedisplay();
		if (arrowy > gh*395/600) level = 1;
		else if (arrowy > gh*380/600) level = 2;
		else if (arrowy > gh*365/600) level = 3;
		else if (arrowy > gh*350/600) level = 4;
		if (lose == 1) level = 0;
		if (win == 1) level = 0;
	}

	if (key == 27) {
		glutPostRedisplay();
		level = 0;
	}
}

void processSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		if (level >= 1 && sp_key != 3) sp_key = 1;
		cout << "LEFT" << endl;
		break;
	case GLUT_KEY_DOWN:
		if (arrowy >= gh*360/600) arrowy -= gh*15/600;
		if(level >= 1 && sp_key != 4) sp_key = 2;
		cout << "DOWN" << endl;
		break;
	case GLUT_KEY_RIGHT:
		if(level >= 1 && sp_key != 1) sp_key = 3;
		cout << "RIGHT" << endl;
		break;
	case GLUT_KEY_UP:
		if (arrowy <= gh*395/600) arrowy += gh*15/600;
		if(level>=1 && sp_key != 2) sp_key = 4;
		cout << "UP" << endl;
		break;
	default:
		sp_key = 5;
		cout << "DIFFERENT" << endl;
	}
}

void idle() {
	if (level >= 1) {
		int x = sq.getX(), y = sq.getY(), speed = sl;
		int new_x = x, new_y = y;
		switch (sp_key) {
		case 1:
			new_x = x - speed;
			break;
		case 2:
			new_y = y - speed;
			break;
		case 3:
			new_x = x + speed;
			break;
		case 4:
			new_y = y + speed;
			break;
		}

		endd = clock();
		if (endd - start > 1000 / 30) {
			if (new_x  < gw && new_x > 0 && new_y < gh && new_y > 0) {
				sq.setPos(new_x, new_y);
				if (prop[new_x][new_y] >= tra) {
					life--;
					ratio = 0;
					for (int j = 0; j < gh; j++) {
						for (int i = 0; i < gw; i++) {
							prop[i][j] = 0;
						}
					}
					init();
					sq.setPos(gw / 2 + sl / 2, gh / 2 + sl / 2);
					new_x = gw / 2; new_y = gh / 2;
					sp_key = 5;
				}
				if (prop[new_x][new_y] == 0) {
					prop[new_x][new_y] = tra;
				}
				if (prop[x][y] >= tra && prop[new_x][new_y] == ter) {
					for (int j = 0; j < gh; j++) {
						for (int i = 0; i < gw; i++) {
							if (prop[i][j] == 0) {
								floodfill(i, j, 0, curr_color);
								curr_color++;
							}
						}
					}
					curr_color = ano;
					for (int j = 0; j < gh; j++) {
						for (int i = 0; i < gw; i++) {
							if (prop[i][j] >= ano) {
								if (color[prop[i][j]] == -1) {
									prop[i][j] = 0;
								}
							}
							if (prop[i][j] >= ter) {
								prop[i][j] = ter;
								ratio++;
							}
						}
					}
					ratio /= gw*gh;
				}

			}
			start = endd;
		}
	}
	
	glutPostRedisplay();
}

void renderScene() {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, gw, 0, gh);
	

	if (level == 0) {
		glColor3f(1, 1, 1);
		draw_string(GLUT_BITMAP_HELVETICA_18, "SELECT LEVEL", gw/2, gh*3/4);
		glColor3f(0.4, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "EASY", gw/2, gh*2/3);
		glColor3f(0.6, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "NORMAL", gw/2, gh*385/600);
		glColor3f(0.8, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "HARD", gw/2, gh*370/600);
		glColor3f(1, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "HELL", gw/2, gh*355/600);


		glColor3f(1, 1, 1);
		glLineWidth(2.5/3);
		glBegin(GL_LINES);
		glVertex2f(gw * 240/600, arrowy);
		glVertex2f(gw * 260/600, arrowy);
		glVertex2f(gw * 260/600, arrowy);
		glVertex2f(gw * 255/600, arrowy + gh*5/600);
		glVertex2f(gw * 260/600, arrowy);
		glVertex2f(gw * 255/600, arrowy - gh*5/600);
		glEnd();
		for (int j = 0; j < gh; j++) {
			for (int i = 0; i < gw; i++) {
				prop[i][j] = 0;
			}
		}
		sq.setPos(gw / 2, gh / 2);
		init();
		sp_key = 0;
		win = 0;
		lose = 0;
		life = 3;
		ratio = 0;

	}
	if (level >= 1) {
		glColor3f(1, 1, 1);
		glLineWidth(4);
		glBegin(GL_LINES);
		glVertex2f(0, gh*9/10);
		glVertex2f(gw, gh*9/10);
		glVertex2f(gw/3, gh);
		glVertex2f(gw/3, gh*9/10);
		glVertex2f(gw*7/12, gh);
		glVertex2f(gw*7/12, gh*9/10);
		glEnd();

		glColor3f(1, 1, 1);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "LIFE", gw/200, gh*59/60);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "LEVEL", gw/3+gw/200, gh * 59 / 60);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_10, "RATIO : ", gw*7/12 + gw / 200, gh * 59 / 60);

		if (level == 1) draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "EASY", gw*2/5, gh*14/15);
		else if (level == 2) draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "NORMAL", gw*23/60, gh * 14 / 15);
		else if (level == 3) draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "HARD", gw * 2 / 5, gh * 14 / 15);
		else if (level == 4) draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "HELL", gw * 2 / 5, gh * 14 / 15);
		
		cout << ratio << endl;
		char buf[50];
		sprintf(buf, "%0.2f", (float)((int)ratio*10000)/100);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, buf, gw * 3 / 5, gh * 14 / 15);

		if (life >= 1) {
			glColor3f(1, 0, 0);
			glBegin(GL_POLYGON);
			glVertex2f(gw/12, gh*11/12);
			glVertex2f(gw/12, gh*59/60);
			glVertex2f(gw*3/20, gh*59/60);
			glVertex2f(gw*3/20, gh*11/12);
			glEnd();
			if (life >= 2) {
				glColor3f(1, 0, 0);
				glBegin(GL_POLYGON);
				glVertex2f(gw/6, gh * 11 / 12);
				glVertex2f(gw / 6, gh * 59 / 60);
				glVertex2f(gw*14/60, gh * 59 / 60);
				glVertex2f(gw * 14 / 60, gh * 11 / 12);
				glEnd();
				if (life >= 3) {
					glColor3f(1, 0, 0);
					glBegin(GL_POLYGON);
					glVertex2f(gw/4, gh * 11 / 12);
					glVertex2f(gw/4, gh * 59 / 60);
					glVertex2f(gw*19/60, gh * 59 / 60);
					glVertex2f(gw*19/60, gh * 11 / 12);
					glEnd();
				}
			}
		}
		for (int i = 0; i < gw; i++) for (int j = 0; j < gh; j++) {
			if (prop[i][j] == ter) Square(i, j, 0, 1, 0, sl).draw();
			if (prop[i][j] == tra) Square(i, j, 0, 0, 1, sl).draw();
		}
		sq.draw();


		//for (auto i : z) i.draw();
		//z.draw();
		//z1.draw();

		if (life == 0) {
			glColor3f(1, 1, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "You Lose! LOLOLOLOLOLO :(", gw*160/600, gh*300/600);
			lose = 1;
		}

		if (ratio >= 0.75) {
			glColor3f(1, 1, 1);
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "You Win!! LOLOLOLOLOLO :)", gw*160/600, gh/2);
			win = 1;
		}

	}
	glutSwapBuffers();
}

void main(int argc, char **argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(650, 100);
	glutInitWindowSize(w, h);
	glutCreateWindow("The Binding of Issac : territory expansion mode");
	init();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	glutIdleFunc(idle);

	// enter GLUT event processing cycle
	glutMainLoop();
}


