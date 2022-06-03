#include "pch.h"
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <GL/freeglut.h>
#include <vector>
#include <string.h>
#include <assert.h>
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include <locale>
#define WIDTH 600
#define HEIGHT 600
#define BEIGE 0
#define RED 1
#define BLUE 2
#define ROCK 3
#define PAPPER 4
#define SCISSORS 5
////prototypes
void mouse(int button,int state,int x,int y);
void init();
void createmenu();
void mouse(int button,int state,int x,int y);
void display();
void checkForTriples();
////

////variables

bool gameHasStarted = false;
bool gameStopped = false;
int score = 0;
int remainingMoves = 1;
int currentMousePresses = 0;

int cubeX1;
int cubeY1;
int cubeX2;
int cubeY2;

int windowID = 0;

int gameMatrix[15][15];
bool isShown[15][15];
GLfloat positions[15][15][3];
GLfloat gameColors[][3] = {{1, 0.91f, 0.79f}, {1, 0, 0}, {0, 0, 1}};
const char *filename = "paper.png";
////

void loadTextureFromFile(const char *filename)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

void processSpecialKeys(int key, int xx, int yy) {

}

bool changeCubes()
{
	//check if cubes are neighbours
	if (abs(cubeX1 - cubeX2) <= 1 && abs(cubeY1 - cubeY2) <= 1){
		//check if cubes are not diagonal
		if (abs(cubeX1 - cubeX2) == 0 or abs(cubeY1 - cubeY2) == 0){
			//check if cube is not the same
			if (cubeX1 != cubeX2 || cubeY1 != cubeY2){
				if (cubeX1 >= 0 && cubeX2 >= 0 && cubeY1 >= 0 && cubeY2 >= 0){
					if (cubeX1 < 15 && cubeX2 < 15 && cubeY1 < 15 && cubeY2 < 15){
						if ((gameMatrix[cubeX1][cubeY1] == ROCK and gameMatrix[cubeX2][cubeY2] == SCISSORS) or (gameMatrix[cubeX1][cubeY1] == PAPPER and gameMatrix[cubeX2][cubeY2] == ROCK) or (gameMatrix[cubeX1][cubeY1] == SCISSORS and gameMatrix[cubeX2][cubeY2] == PAPPER)){
							if (isShown[cubeX1][cubeY1] and isShown[cubeX2][cubeY2]){
								isShown[cubeX2][cubeY2] = false;
								int temp = gameMatrix[cubeX1][cubeY1];
								bool tempbool = isShown[cubeX1][cubeY1];
								gameMatrix[cubeX2][cubeY2] = temp;
								isShown[cubeX1][cubeY1] = isShown[cubeX2][cubeY2];
								isShown[cubeX2][cubeY2] = tempbool;
								remainingMoves --;
								display();
								cubeX1 = cubeX2 = cubeY1 = cubeY2 = -1;
								return true;
							}
						}
						else{
							int temp = gameMatrix[cubeX1][cubeY1];
							bool tempbool = isShown[cubeX1][cubeY1];
							gameMatrix[cubeX1][cubeY1] = gameMatrix[cubeX2][cubeY2];
							gameMatrix[cubeX2][cubeY2] = temp;
							isShown[cubeX1][cubeY1] = isShown[cubeX2][cubeY2];
							isShown[cubeX2][cubeY2] = tempbool;
							remainingMoves --;
							display();
							cubeX1 = cubeX2 = cubeY1 = cubeY2 = -1;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void mouse(int button,int state,int x,int y)
{
	if (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN and gameHasStarted and remainingMoves > 0){
		GLint viewport[4];
	    GLdouble modelview[16];
	    GLdouble projection[16];
	    GLfloat winX, winY, winZ;
	    GLdouble posX, posY, posZ;
	 
	    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	    glGetDoublev( GL_PROJECTION_MATRIX, projection );
	    glGetIntegerv( GL_VIEWPORT, viewport );
	 
	    winX = (float)x;
	    winY = (float)viewport[3] - (float)y;
	    
	    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	 
	    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	 	
	 	posX = (posX * 1000);
	 	posY = (posY * 1000);
	 	
	 	if (currentMousePresses == 0){
			cubeX1 = (int)((posX + 32)/4.29);
			cubeY1 = (int)((posY + 32)/4.29);
	 		//printf("x1: %d y1: %d\n", cubeX1, cubeY1);
	 	}
	 	else if (currentMousePresses == 1){
	 		
			cubeX2 = (int)((posX + 32)/4.29);
			cubeY2 = (int)((posY + 32)/4.29);
			//printf("x2: %d y2: %d\n", cubeX2, cubeY2);

			if (changeCubes()){
				checkForTriples();
			}
	 	}
	 	currentMousePresses += 1;
	}
	else if (remainingMoves == 0){
		gameStopped = true;
		gameHasStarted = false;
 		display();
	}
}

void startFunc()
{
	int c;
	for (int i = 0; i < 15; i++){
		for (int j = 0; j < 15; j++){
			gameMatrix[i][j] = (rand()%5) + 1;
			isShown[i][j] = true;
		}
	}
	gameHasStarted = true;
	gameStopped = false;
	remainingMoves = 30;
	score = 0;
	checkForTriples();
	display();
}

void menu(int value)
{
	switch(value){
		case 1: //EXIT
			glutDestroyWindow(windowID);
			exit(0);
			break;
		case 2:// START
			startFunc();
			break;
	}
}

void createmenu()
{
	int gameMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Exit",1);
	glutAddMenuEntry("Start",2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glClear(GL_COLOR_BUFFER_BIT);
}

void drawCube(){
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glBegin(GL_QUADS);
		// Front Face
		// z = 1
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        // Top Face
        // y = 1
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        // Left Face
        // x = -1
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        // Back Face
        // z = -1
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        // Bottom Face
        // y = -1
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        // Right face
        // x = 1
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glEnd();  // End of drawing color-cube
}


void reshape(int x, int y)
{
    if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLdouble)x / (GLdouble)y, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, x, y);  //Use the whole window for rendering
}

void checkForRadius1Horizontal(int x, int y)
{
	for (int i = x - 1; i < x + 4; i ++){
		for (int j = y - 1; j < y + 2; j ++){
			if (j != y or (i < x and i > x + 2)){
				if (i >= 0 and i < 15){
					if ( j >= 0 and j < 15){
						if (isShown[x][y] and isShown[i][j]){
							if (gameMatrix[x][y] == ROCK){
								if (gameMatrix[i][j] == SCISSORS or gameMatrix[i][j] == RED or gameMatrix[i][j] == BLUE){
									score += 2;
									isShown[i][j] = false;
								}
								else if (gameMatrix[i][j] == PAPPER or gameMatrix[i][j] == ROCK){
									score -= 1;
								}
							}
							else if (gameMatrix[x][y] == PAPPER){
								if (gameMatrix[i][j] == ROCK or gameMatrix[i][j] == RED or gameMatrix[i][j] == BLUE){
									score += 2;
									isShown[i][j] = false;
								}
								else if (gameMatrix[i][j] == SCISSORS or gameMatrix[i][j] == PAPPER){
									score -= 1;
								}
							}
							else if (gameMatrix[x][y] == SCISSORS){
								if (gameMatrix[i][j] == PAPPER or gameMatrix[i][j] == RED or gameMatrix[i][j] == BLUE){
									score + 2;
									isShown[i][j] == false;
								}
								else if (gameMatrix[i][j] == ROCK or gameMatrix[i][j] == SCISSORS){
									score -= 1;
								}
							}
						}
					}
				}
			}
		}
	}
}

void checkForRadius1Vertical(int x, int y)
{
	for (int i = x - 1; i < x + 2; i ++){
		for (int j = y - 1; j < y + 4; j ++){
			if (i != x or (j != y and j != y + 1 and j != y + 2)){
				if (i >= 0 and i < 15){
					if (j >= 0 and j < 15){
						if (isShown[x][y] and isShown[i][j]){
							if (gameMatrix[x][y] == ROCK){
								if (gameMatrix[i][j] == SCISSORS or gameMatrix[i][j] == RED or gameMatrix[i][j] == BLUE){
									score += 2;
									isShown[i][j] = false;
								}
								else if (gameMatrix[i][j] == PAPPER or gameMatrix[i][j] == ROCK){
									score -= 1;
								}
							}
							else if (gameMatrix[x][y] == PAPPER){
								if (gameMatrix[i][j] == ROCK or gameMatrix[i][j] == RED or gameMatrix[i][j] == BLUE){
									score += 2;
									isShown[i][j] = false;
								}
								else if (gameMatrix[i][j] == SCISSORS or gameMatrix[i][j] == PAPPER){
									score -= 1;
								}
							}
							else if (gameMatrix[x][y] == SCISSORS){
								if (gameMatrix[i][j] == PAPPER or gameMatrix[i][j] == RED or gameMatrix[i][j] == BLUE){
									score + 2;
									isShown[i][j] == false;
								}
								else if (gameMatrix[i][j] == ROCK or gameMatrix[i][j] == SCISSORS){
									score -= 1;
								}
							}
						}
					}
				}
			}
		}
	}
}

void checkForRadius2And3Horizontal(int x, int y)
{
	for (int i = x - 3; i < x + 6; i ++){
		for (int j = y - 3; j < y + 4; j ++){
			//there is an error
				if (i >= 0 and i < 15){
					if (j >= 0 and j < 15){
						if (isShown[x][y] and isShown[i][j]){
							if (gameMatrix[x][y] == ROCK){
								if (gameMatrix[i][j] == SCISSORS){
									score += 3;
									isShown[i][j] = false;
								}
							}
							else if (gameMatrix[x][y] == PAPPER){
								if (gameMatrix[i][j] == ROCK){
									score += 3;
									isShown[i][j] = false;
								}
							}
							else if (gameMatrix[x][y] == SCISSORS){
								if (gameMatrix[i][j] == PAPPER){
									score += 3;
									isShown[i][j] = false;
								}
							}
						}
					}
				}
		}
	}
}

void checkForRadius2And3Vertical(int x, int y)
{
	for (int i = x - 3; i < x + 4; i ++){
		for (int j = y - 3; j < y - 6; j ++){
			//there is an error
				if (i >= 0 and i < 15){
					if (j >= 0 and j < 15){
						if (isShown[x][y] and isShown[i][j]){
							if (gameMatrix[x][y] == ROCK){
								if (gameMatrix[i][j] == SCISSORS){
									score += 3;
									isShown[i][j] = false;
								}
							}
							else if (gameMatrix[x][y] == PAPPER){
								if (gameMatrix[i][j] == ROCK){
									score += 3;
									isShown[i][j] = false;
								}
							}
							else if (gameMatrix[x][y] == SCISSORS){
								if (gameMatrix[i][j] == PAPPER){
									score += 3;
									isShown[i][j] = false;
								}
							}
						}
					}
				}
		}
	}
}

void checkForTriples()
{
	for (int i = 0; i < 15; i ++){
		for (int j = 0; j < 15; j ++){
			if (i < 12){
				if (gameMatrix[i + 1][j] == gameMatrix[i][j] and gameMatrix[i + 2][j] == gameMatrix[i][j] and isShown[i][j] and isShown[i + 1][j] and isShown[i + 2][j]){
					//change score
					score += 10;
					//destroy neighbours
					checkForRadius1Horizontal(i, j);
					checkForRadius2And3Horizontal(i, j);
					//destroy triplete
					isShown[i][j] = false;
					isShown[i + 1][j] = false;
					isShown[i + 2][j] = false;
					display();
				}
			}
			if (j < 12){
				if (gameMatrix[i][j + 1] == gameMatrix[i][j] and gameMatrix[i][j + 2] == gameMatrix[i][j] and isShown[i][j] and isShown[i][j + 1] and isShown[i][j + 2]){
					//change score
					score += 10;
					//destroy neighbours
					checkForRadius1Vertical(i, j);
					checkForRadius2And3Vertical(i, j);
					//destroy triplete
					isShown[i][j] = false;
					isShown[i][j + 1] = false;
					isShown[i][j + 2] = false;
					display();
				}
			}
		}
	}
	for (int i = 14; i >= 0; i --){
		for (int j = 14; j >= 0; j --){
			if (i > 2){
				if (gameMatrix[i - 1][j] == gameMatrix[i][j] and gameMatrix[i - 2][j] == gameMatrix[i][j] and isShown[i][j] and isShown[i - 1][j] and isShown[i - 2][j]){
					//change score
					score += 10;
					//destroy neighbours
					checkForRadius1Horizontal(i - 2, j);
					checkForRadius2And3Horizontal(i - 2, j);
					//destroy triplete
					isShown[i][j] = false;
					isShown[i - 1][j] = false;
					isShown[i - 2][j] = false;
					display();
				}
			}
			if (j > 2){
				if (gameMatrix[i][j - 1] == gameMatrix[i][j] and gameMatrix[i][j - 2] == gameMatrix[i][j] and isShown[i][j] and isShown[i][j - 1] and isShown[i][j - 2]){
					//change score
					score += 10;
					//destroy neighbours
					checkForRadius1Vertical(i, j - 2);
					checkForRadius2And3Vertical(i, j - 2);
					//destroy triplete
					isShown[i][j] = false;
					isShown[i][j - 1] = false;
					isShown[i][j - 2] = false;
					display();
				}
			}
		}
	}
}

void idle()
{
	if (currentMousePresses == 2){
		currentMousePresses = 0;
	}
}

void displayGameOver()
{
	glPushMatrix();
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);  //RGBA values of text color
		glRasterPos2i(-10, 12);

		char string[128];

		sprintf(string, "GAME OVER, SCORE IS %d", score);

		int len = strlen(string);
		for (int i = 0; i < len; i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
		}
	glPopMatrix();
}

void displayText()
{
	glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  //RGBA values of text color
		glRasterPos2i(-10, 12);


		char string[128];

		sprintf(string, "SCORE: %d, MOVES: %d", score, remainingMoves);

		int len = strlen(string);
		for (int i = 0; i < len; i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
		}
	glPopMatrix();
}

void display()
{
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef( 0, 0, -35 );

	for (int i = 0; i < 45; i+=3){
		for (int j = 0; j < 45; j+=3){
			glPushMatrix();
				glTranslatef(-21 + i, -21 + j, -35);
				//glRotatef(20, 1, 0, 0);
				if((gameMatrix[i/3][j/3] != BLUE) && (gameMatrix[i/3][j/3] != RED) && (gameMatrix[i/3][j/3] != BEIGE))
				{
					glColor3f(1, 1, 1);
					if(gameMatrix[i/3][j/3] == ROCK){
						loadTextureFromFile("rock.bmp");
					}
					else if(gameMatrix[i/3][j/3] == PAPPER){
						loadTextureFromFile("paper.bmp");
					}
					else if(gameMatrix[i/3][j/3] == SCISSORS){
						loadTextureFromFile("scissors.bmp");
					}
					glEnable(GL_TEXTURE_2D);
				}
				else
				{
					glColor3f(gameColors[gameMatrix[i/3][j/3]][0], gameColors[gameMatrix[i/3][j/3]][1], gameColors[gameMatrix[i/3][j/3]][2]);
				}
				if (isShown[i/3][j/3]){
					drawCube();	
				}
				glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}

	if (gameHasStarted and !gameStopped){
		displayText();
	}

	if (!gameHasStarted and gameStopped){
		displayGameOver();
	}

	glutSwapBuffers();
}

void init()
{
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat mat_shininess[] = { 100.0 };
	GLfloat light_position[] = { 0, 0, 3, -1.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);

 	// clear the drawing buffer.
 	glClear(GL_COLOR_BUFFER_BIT);

 	glMatrixMode(GL_PROJECTION);
 	glLoadIdentity();
 	GLint viewport[4];
 	glGetIntegerv(GL_VIEWPORT, viewport);
 	double aspect = (double)viewport[2] / (double)viewport[3];
 	gluPerspective(60, aspect, 1, 100);

 	//game matrix initialization
 	for (int i = 0; i < 15; i ++){
 		for (int j = 0; j < 15; j++){
 			gameMatrix[i][j] = 0;
 			isShown[i][j] = true;
 		}
 	}

 	display();
}

int main (int argc,char** argv)
{

	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutInitWindowPosition(100, 100);

	windowID = glutCreateWindow("VRAXAPSA III");
	createmenu();

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutSpecialFunc(processSpecialKeys);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();
	glutMainLoop();
	return 0;
}
