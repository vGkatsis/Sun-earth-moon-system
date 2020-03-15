#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include "gl/glut.h"
#include "visuals.h"

model md;
static float rotx = 0.0;													//Angle of camera rotation on x axis
static float roty = 0.0;													//Angle of camera rotation on y axis
static float rotz = 0.0;													//Angle of camera rotation on z axis
static float earthrot = 0.0;												//Angle of earth rotation on x axis
static float moonrot = 0.0;													//Angle of moon rotation on y axis
static float opacity = 1.0;													//A factor of suns shinning sphere
static char decreasing = 1;													//Flag showing if a shinning sphere is increasing or decreasing
static bool animate = true;													//Flag implementing animation play and pause
static star stars[STARSNUM];												//Number of stars on the sky

using namespace std;

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);						// Clean up the colour of the window and the depth buffer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -100);

	text("PAUSE", 0.02f);													//Text for wristting PAUSE

	glRotatef(rotx, 1, 0, 0);												//Rotate everything around x axis at a rotx angle
	glRotatef(roty, 0, 1, 0);												//Rotate everything around y axis at a roty angle
	glRotatef(rotz, 0, 0, 1);												//Rotate everything around z axis at a rotz angle

	axes();																	//Display x and y axes

	starsfunc();															//Display stars
	
	earth();																//Display the earth

	moon();																	//Display the moon

	sun();																	//Display the sun

	glutSwapBuffers();														// All drawing commands applied to the hidden buffer, so now, bring forwardthe hidden buffer and hide the visible one
}

void Resize(int w, int h)
{
	if (h == 0)																// Define the visible area of the window ( in pixels )
	{
		h = 1;
	}
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);											// Setup viewing volume
	glLoadIdentity();

	gluPerspective(60.0, (float)w / (float)h, 1.0, 500.0);
}

void Idle()
{
	if (animate)															//If motion is enabled
	{
		if (decreasing == 1)												//If opacity shoud decrease
		{
			opacity -= 0.01;												//Decrease it
			if (opacity < 0)												//Opaciti should increase now
			{
				decreasing = 0;												//Set decrease to 0
			}
		}
		else																//If opacity should increase
		{
			opacity += 0.01;												//Increase it
			if (opacity > 1.0)												//Opacity should decrease now
			{
				decreasing = 1;												//Set decrease to 1
			}
		}
	
		earthrot += 0.3f;													//Change the angle of earth rotation
		moonrot += 1.0f;													//Change the angle of moon rotation
	}
	
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:																//Exit animation using ESC key
		delete[] md.obj_points;
		delete[] md.obj_faces;
		exit(0);
		break;
	case 'A':																//Use 'a' key to rotate camera left 
	case 'a': 
		roty += 0.2f;
		break;
	case 'D':																//Use 'd' key to rotate camera right 
	case 'd': 
		roty -= 0.2f;
		break;	
	case 'W':																//Use 'w' key to rotate camera up 
	case 'w':
		rotx += 0.2f;
		break;
	case 'S':																//Use 's' key to rotate camera down 
	case 's':
		rotx -= 0.2f;
		break;
	case 'Q':																//Use 'q' key to rotate camera counter clockwise
	case 'q':
		rotz -= 0.2f;
		break;
	case 'E':																//Use 'e' key to rotate camera clockwise
	case 'e':
		rotz += 0.2f;
		break;
	case ' ':																//Use 'space' key to pause or unpause the animation
		animate = !animate;	
		break;
	default: 
		break;
	}

	glutPostRedisplay();

}

void Setup()
{
	srand(time(NULL));														// initialize random seed
	for (int i = 0; i < STARSNUM; i++)
	{
		do {																//Define a sphere of radius 100 around the sun where no star appears
			stars[i].xpos = (rand() % 481) - 240;
			stars[i].ypos = (rand() % 481) - 240;
			stars[i].zpos = (rand() % 481) - 240;
		} while ((stars[i].xpos < 100) && (stars[i].xpos > -100) && (stars[i].ypos < 100) && (stars[i].ypos > -100) && (stars[i].zpos < 100) && (stars[i].zpos > -100));

	}
	
	ReadFile(&md);															//Read the model from planet.obj

																			//Parameter handling
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);													//Renders a fragment if its z value is less or equal of the stored value
	glClearDepth(1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

																			//Polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

																			//Set up light source
	GLfloat light_position[] = { 0.0, 0.0, -100.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);									// Black background

}

void ReadFile(model *md)
{

	ifstream obj_file("Release\\planet.obj");											// Open the file for reading OBJINFO.TXT

	if (obj_file.fail())
	{
		exit(1);
	}

	char token;
	int trashnum;
	//int pointn;
	//int uvpoint;
	//float num;
	string trash;
//////////////////////READING POINTS STARTS HERE////////////////////////////////
	for (int i = 0; i < 5; i++)
	{
		getline(obj_file, trash);
	}

	obj_file >> token;

	obj_file >> md->vertices;

	md->obj_points = new point[md->vertices];

	getline(obj_file, trash);

	for (int i = 0; i < md->vertices; i++)
	{
		obj_file >> token;

		obj_file >> md->obj_points[i].x;
		obj_file >> md->obj_points[i].y;
		obj_file >> md->obj_points[i].z;
	}
//////////////////////READING POINTS ENDS HERE/////////////////////////////////////

//////////////////////READING UVPOINTS STARTS HERE////////////////////////////////

/*	getline(obj_file,trash);

	obj_file >> token;

	obj_file >> uvpoint;
	getline(obj_file,trash);

	for(int i = 0; i < uvpoint; i++)
	{
		obj_file >> token;

		obj_file >> num;
		cout << "\nNum1: " << num << "\n" << endl;

		obj_file >> num;
		cout << "\nNum2: " << num << "\n" << endl;

		obj_file >> num;
		cout << "\nNum3: " << num << "\n" << endl;
	}
*/

//////////////////////READING UVPOINTS ENDS HERE///////////////////////////////////

//////////////////////READING VERTEX NORMALS STARTS HERE///////////////////////////

/*	getline(obj_file,trash);

	obj_file >> token;

	obj_file >> pointn;
	cout << "\nThe number of npoints is "  << pointn << "\n" << endl;
	getline(obj_file,trash);

	for(int i = 0; i < pointn; i++)
	{
		obj_file >> token;
		obj_file >> token;

		obj_file >> num;
		cout << "\nNumN1: " << num << "\n" << endl;

		obj_file >> num;
		cout << "\nNumN2: " << num << "\n" << endl;

		obj_file >> num;
		cout << "\nNumN3: " << num << "\n" << endl;
	}
*/
//////////////////////READING VERTEX NORMALS ENDS HERE//////////////////////////////


//////////////////////READING FACES STARTS HERE/////////////////////////////////////
	for (int i = 0; i < 9129; i++)															//Skip UV and VN
	{
		getline(obj_file, trash);
	}

	for (int i = 0; i < 4; i++)
	{
		obj_file >> trash;
	}

	obj_file >> md->faces;

	md->obj_faces = new face[md->faces];

	obj_file >> trash;

	getline(obj_file, trash);
	getline(obj_file, trash);
	getline(obj_file, trash);

	for (int i = 0; i < md->faces; i++)
	{
		obj_file >> token;

		obj_file >> md->obj_faces[i].vtx[0];

		obj_file >> token;
		obj_file >> token;
		obj_file >> trashnum;

		obj_file >> md->obj_faces[i].vtx[1];

		obj_file >> token;
		obj_file >> token;
		obj_file >> trashnum;

		obj_file >> md->obj_faces[i].vtx[2];

		obj_file >> token;
		obj_file >> token;
		obj_file >> trashnum;

	}
//////////////////////READING FACES ENDS HERE////////////////////////////////////////
	
	obj_file.close();
}

void DisplayModel(model md)
{

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.faces; i++)
	{
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[0] - 1].x, md.obj_points[md.obj_faces[i].vtx[0] - 1].y, md.obj_points[md.obj_faces[i].vtx[0] - 1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[1] - 1].x, md.obj_points[md.obj_faces[i].vtx[1] - 1].y, md.obj_points[md.obj_faces[i].vtx[1] - 1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[2] - 1].x, md.obj_points[md.obj_faces[i].vtx[2] - 1].y, md.obj_points[md.obj_faces[i].vtx[2] - 1].z);
	}

	glEnd();
	glPopMatrix();

}

void sun()
{
	glColor3f(1.0, 1.0, 0.0);												// Set inner sphere colour to be yellow
	glutSolidSphere(10.0, 30, 24);

	glColor4f(1.0, 1.0, 0.0, opacity);										// Set outer sphere colour to be  yellow and transparent
	glutSolidSphere(15, 30, 24);
}

void starsfunc()
{
	for (int y = 0; y < STARSNUM; y++)
	{
		glPushMatrix();
		glTranslatef(stars[y].xpos, stars[y].ypos, stars[y].zpos);			//Put the star on its random position
		glColor3f(1.0, 1.0, 1.0);											// Set star to white colour
		glutSolidSphere(0.2, 15, 10);										// Draw a star
		glPopMatrix();
	}
}

void earth()
{
	glPushMatrix();
	glRotatef(earthrot, 0, 1, 0);											//Earth will be rotating around y axis
	glTranslatef(40, 0, 0);
	glColor3f(0.0, 0.0, 1.0);												// Set drawing colour
	glScalef(0.014f, 0.014f, 0.014f);
	DisplayModel(md);														//Draw the earth
	glPopMatrix();
}

void moon()
{
	
	glPushMatrix();
	glRotatef(earthrot, 0, 1, 0);											//Moon will be rotating around y axis
	glRotatef(moonrot, 1, 0, 0);											//Moon will be rotating around x axis
	glTranslatef(40, 15, 0);
	glScalef(0.005f, 0.005f, 0.005f);
	glColor3f(1.0, 1.0, 1.0);												// Set drawing colour
	DisplayModel(md);														//Draw the moon
	glPopMatrix();
}

void axes()
{
	glColor3f(0.6, 0.6, 0.6);												// Set drawing colour
	glPushMatrix();
	glTranslatef(0, 0, -1.0);
	glBegin(GL_LINES);
	glVertex2f(0.0, 0.0);
	glVertex2f(100.0, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 100.0);
	glEnd();
	glPopMatrix();
}

void text(const char *str, float size)
{
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0,!animate);										// Set drawing colour and transparency
	glTranslatef(-4.0, 17.0, 0.0);
	glScalef(size, size, size);
	for (int i = 0; i < strlen(str); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();

}