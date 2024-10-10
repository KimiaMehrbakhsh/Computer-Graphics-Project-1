/*
COMP 390: Computer Graphics
Final Project
By: Kimia Mehr Bakhsh
Student Number: 3678102
Dear Dr. Liaqat thank you for giving me a second chance.

Description: this project is presenting option 1. I have tried to demonstrate all the requirements
that was given in the project description, but also be more creative and try to play around with different techniques that
I have learned in this course. For instance, I have created some of my object as lists, I have used transformation techniques
such as translate and scale in scaleAndTranslateHouse() to create an additional object.

I have also tried to incorporate more efficient codes such creating and reusing vertices and indicies when drawing
pyramids and cubes. I used a beautiful background photo to demonstrate the texture background and togther with 
the foggy atmospheric effect, it intensifies the effect of gloominess in the scene. 

In addition I have added three light sources which you can find the code in init(). One light is positioned 
on top of the scene and the other two are directing at two differnt sides, one is blue and the other is red.

If I had more time I would have loved to add on more objects such as another person. I also was trying to add lights to 
the light bulps in the scene but I don't think that is possible. This is the best I could've done with the time I had.

I have tried to listen to your constructive feedback in my previous attempt, and did not use any code from previous
 assignments and tried to create an original work from scratch. I hope this is more to your liking. Thank  you again for 
 your time and help.
*/

#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

//image
GLubyte* image;
GLubyte* l_texture;
BITMAPFILEHEADER fileheader;
BITMAPINFOHEADER infoheader;
RGBTRIPLE rgb;

GLuint textName;

GLuint robotList , groundList, houseList, carList;


// Lighting coefficients
float ambient_coef = 0.3;
float diffuse_coef = 0.7;

//define material properties 
GLfloat mat_ambient[] = { 0.25, 0.25, 0.25, 2.0 };
GLfloat mat_diffuse[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f }; // High specular component
GLfloat mat_shininess[] = { 76.8f }; // High shininess for sharp highlights



//Define the eight vertices of the cube
GLfloat vertices[8][3] = {
	{0.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0,1.0,0.0},
	{0.0,1.0,0.0},
	{0.0,0.0,1.0},
	{1.0,0.0,1.0},
	{1.0,1.0,1.0},
	{0.0,1.0,1.0}
};

//Define the indices for each face of the cube
GLubyte indices[6][4] = {
	{0, 1, 2, 3},  // Face 1 (Front)
	{4, 5, 6, 7},  // Face 2 (Back)
	{0, 3, 7, 4},  // Face 3 (Left)
	{1, 2, 6, 5},  // Face 4 (Right)
	{3, 2, 6, 7},  // Face 5 (Top)
	{0, 1, 5, 4}   // Face 6 (Bottom)
};

GLfloat carVertices[8][3] = {
	 {-5.0, 0.5, -0.5},
	{ -2.0, 0.5, -0.5},
	{ -2.0,  1.5, -0.5},
	{-5.0,  1.5, -0.5},
	{-5.0, 0.5,  1.0},
	{ -2.0, 0.5,  1.0},
	{ -2.0,  1.5,  1.0},
	{-5.0,  1.5,  1.0}
};

GLfloat roofVertices[8][3]{
	 {-4.5, 0.5, -0.25},
	{ -2.5, 0.5, -0.25},
	{ -2.5,  1.75, -0.25},
	{-4.5,  1.75, -0.25},
	{-4.5, 0.5,  0.75},
	{ -2.5, 0.5,  0.75},
	{ -2.5,  1.75,  0.75},
	{ -4.5,  1.75,  0.75 }
};

GLfloat bumperVertices[8][3]{
	 {-2.0, 0.5, -0.25},
	{ -1.8, 0.5, -0.25},
	{ -1.8,  0.75, -0.25},
	{-2.0, 0.75, -0.25},
	{-2.0, 0.5,  0.75},
	{ -1.8, 0.5,  0.75},
	{ -1.8,  0.75,  0.75},
	{ -2.0,  0.75,  0.75 }
};

GLfloat houseVertices[8][3] = {
	{3.0, 1.0, -0.5},
	{ 4.5, 1.0, -0.5},
	{ 4.5,  3.0, -0.5},
	{3.0,  3.0, -0.5},
	{3.0, 1.0,  1.0},
	{ 4.5, 1.0,  1.0},
	{ 4.5,  3.0,  1.0},
	{3.0,  3.0,  1.0}
};

GLfloat doorVertices[8][3] = {
	{3.5, 1.0, 0.25},
	{ 4.2, 1.0, 0.52},
	{ 4.2,  1.7, 0.25},
	{3.5,  1.7, 0.25},
	{3.5, 1.0,  1.5},
	{ 4.2, 1.0,  1.5},
	{ 4.2,  1.7,  1.5},
	{3.5,  1.7,  1.5}
};

GLfloat lWindowVertices[8][3] = {
	{3.2, 2.8, 0.25},
	{ 3.5, 2.8, 0.52},
	{ 3.5,  2.6, 0.25},
	{3.2,  2.6, 0.25},
	{3.2, 2.8,  1.5},
	{ 3.5, 2.8,  1.5},
	{ 3.5,  2.6,  1.5},
	{3.2,  2.6,  1.5}
};

GLfloat rWindowVertices[8][3] = {
	{4.0, 2.8, 0.25},
	{ 4.3, 2.8, 0.52},
	{ 4.3,  2.6, 0.25},
	{4.0,  2.6, 0.25},
	{4.0, 2.8,  1.5},
	{ 4.3, 2.8,  1.5},
	{ 4.3,  2.6,  1.5},
	{4.0,  2.6,  1.5}
};

GLfloat pyramidVertices[8][3] = {
	{ 4.5,  3.0, -0.5},
	{ 4.5,  3.0,  1.0},
	{3.0,  3.0,  1.0},
	{3.0,  3.0, -0.5},
	{3.75,4.0,-0.75}
};

//Define the indices for each face of the pyramid
GLubyte pyramidIndices[4][3] = {
	{0, 1, 4},  // Face 1 (Front)
	{1, 2, 4},  // Face 2 (Right)
	{2, 3, 4},  // Face 3 (Back)
	{3, 0, 4}   // Face 4 (Left)
};

//the image for the texture background
void makeImage(void)
{
	int i, j = 0;
	FILE* l_file;

	string fn = "texture.bmp";
	const char* filename = fn.c_str();

	//open image file, return if error
	fopen_s(&l_file, filename, "rb");
	if (l_file == NULL) return;

	//read file header and header info
	fread(&fileheader, sizeof(fileheader), 1, l_file);
	fseek(l_file, sizeof(fileheader), SEEK_SET);
	fread(&infoheader, sizeof(infoheader), 1, l_file);

	//allocate space for the image file
	l_texture = (GLubyte*)malloc(infoheader.biWidth * infoheader.biHeight * 4);
	memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);

	//read data
	j = 0;
	for (i = 0; i < infoheader.biWidth * infoheader.biHeight; i++) {
		fread(&rgb, sizeof(rgb), 1, l_file);

		l_texture[j + 0] = (GLubyte)rgb.rgbtRed; // Red component

		l_texture[j + 1] = (GLubyte)rgb.rgbtGreen; // Green component

		l_texture[j + 2] = (GLubyte)rgb.rgbtBlue; // Blue component

		l_texture[j + 3] = (GLubyte)255; // Alpha value

		j += 4; // Go to the next position

	}
	fclose(l_file); // Closes the file stream

	// Generate a texture ID and bind the texture
	glGenTextures(1, &textName);
	glBindTexture(GL_TEXTURE_2D, textName);

	// Create the texture in OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

//background with positions for the picture 
void background() {
	glBindTexture(GL_TEXTURE_2D, textName);
	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-8.0, -0.53, -8.0);  // Bottom-left corner (start of ground)
	glTexCoord2f(1.0, 0.0); glVertex3f(8.0, -0.53, -8.0);   // Bottom-right corner (along X-axis)
	glTexCoord2f(1.0, 1.0); glVertex3f(8.0, 10.0, -8.0);   // Top-right corner (along X-axis, above ground)
	glTexCoord2f(0.0, 1.0); glVertex3f(-8.0, 10.0, -8.0);  // Top-left corner (above ground)
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


//scale and translate the house
void scaleAndTranslateHouse(float tx, float ty, float tz, float sx, float sy, float sz) {
	glPushMatrix(); //save the current transformation matrix

	glTranslatef(tx, ty, tz); // apply translation
	glScalef(sx, sy, sz); // apply scaling
	glCallList(houseList);

	glPopMatrix(); //restore the original transformation matrix
}

//the brown ground
void ground()
{
	groundList = glGenLists(1);
	glNewList(groundList, GL_COMPILE);

	glPushMatrix();
	glColor3f(0.36, 0.25, 0.20);

	glBegin(GL_POLYGON);
	glVertex3f(-8.0, -1.9, -8.0);  // Front-left
	glVertex3f(-8.0, -1.9, 8.0);   // Far-left
	glVertex3f(8.0, -1.9, 8.0);    // Far-right
	glVertex3f(8.0, -1.9, -8.0);   // Front-right
	glEnd();

	glPopMatrix();
	glEndList();
}

//first 3D composite object: a robot
void robot()
{
	robotList = glGenLists(1);
	glNewList(robotList, GL_COMPILE);


	glColor3f(0.0, 0.0, 0.0); //grey
	glBegin(GL_QUADS);

	//body of the robot (cube)
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			glVertex3fv(vertices[indices[i][j]]);
		}
	}
	//glPopMatrix();
	glEnd();

	//head of the robot (sphere)
	glColor3f(0.6, 0.6, 0.6); //grey
	glTranslatef(0.5f, 1.5f, 0.5f); 
	glutSolidSphere(0.5f, 50, 50);
	glPopMatrix();

	glColor3f(0.5, 0.0, 0.0);

	//left arm (cylinder)
	GLUquadric* quadric = gluNewQuadric();
	glPushMatrix();
	glTranslatef(-0.54, -0.5, 0.5);
	glRotatef(45, 1.0, 0.0, 0.0); 
	gluCylinder(quadric, 0.1f, 0.1f, 1.0f, 30, 30); 
	glPopMatrix(); 


	//left hand (cone)
	glPushMatrix();
	glTranslatef(-0.75f, -1.47f, 0.5f);
	glScalef(0.2, 0.2, 0.2);
	glutSolidTetrahedron();
	glPopMatrix(); 

	//right arm (cylinder)
	glPushMatrix();  
	glTranslatef(0.78f, -1.0f, 0.5f); 
	glRotatef(-49.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadric, 0.1f, 0.1f, 1.0f, 30, 30); 
	glPopMatrix();  

	//right hand (cone)
	glPushMatrix();
	glTranslatef(0.7f, -1.18f, 0.5f); 
	glScalef(0.2, 0.2, 0.2);
	glRotatef(-15, 1.0, 0.0, 0.0);
	glutSolidTetrahedron();
	glPopMatrix();  

	//left leg
	glPushMatrix();
	glTranslatef(-0.25, -1.8, 0.2);
	glScalef(0.4, 0.4, 0.4);
	glutSolidTetrahedron();
	glPopMatrix();

	//right leg
	glPushMatrix();
	glTranslatef(0.25, -1.8, 0.2);
	glScalef(0.4, 0.4, 0.4);
	glutSolidTetrahedron();
	glPopMatrix();

	gluDeleteQuadric(quadric);

	glEndList();  // End the display list
}


//second 3D object is a lamp
void lampPost()
{
	GLUquadric* quadric = gluNewQuadric();

	//the pole
	glColor3f(0.3, 0.3, 0.3);
	glPushMatrix();
	glTranslatef(7.0, 3.0, -4.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(quadric, 0.3f, 0.3f, 4.0f, 30, 30);
	glPopMatrix();

	//the lamp shade
	glPushMatrix();
	glTranslatef(6.0f, 2.3f, -4.0f); 
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.5f, 0.8f, 30, 30); 
	glPopMatrix();

	//supporting arm
	glColor3f(0.3f, 0.3f, 0.3f); // Dark grey color for the support arms
	glPushMatrix();
	glTranslatef(7.0f, 3.0f, -4.0f); 
	glRotatef(-90, 0.0f, 1.0f, 0.0f); // Rotate it to extend horizontally
	gluCylinder(quadric, 0.05f, 0.05f, 1.0f, 30, 30); 
	glPopMatrix();

	//light bulb
	glColor3f(1.0f, 1.0f, 0.8f); 
	glPushMatrix();
	glTranslatef(6.0f, 2.2f, -4.0f); 
	glutSolidSphere(0.2f, 30, 30); 

	glPopMatrix();

	//right arm
	glColor3f(0.3f, 0.3f, 0.3f); 
	glPushMatrix();
	glTranslatef(7.0f, 3.0f, -4.0f); 
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadric, 0.05f, 0.05f, 1.0f, 30, 30); 
	glPopMatrix();

	//the right lamp shade
	glPushMatrix();
	glTranslatef(8.0f, 2.3f, -4.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.5f, 0.8f, 30, 30);
	glPopMatrix();

	//light bulb
	glColor3f(1.0f, 1.0f, 0.8f); 
	glPushMatrix();
	glTranslatef(8.0f, 2.2f, -4.0f); 
	glutSolidSphere(0.2f, 30, 30);
	glPopMatrix();

}

//third 3D object is a tree
void tree()
{
	GLUquadric* quadric = gluNewQuadric();

	//trunk
	glColor3f(0.55, 0.27, 0.07);
	glPushMatrix();
	glTranslatef(-1.0, 2.5, -4.0);
	glRotatef(90, 1.0, 0.0, 0.0); 
	gluCylinder(quadric, 0.5, 0.5, 3.0, 30, 30);
	glPopMatrix();

	//left branch
	glPushMatrix();
	glTranslatef(-1.0, 2.5, -4.0);
	glRotatef(-60, 0.5, 1.0, 1.0);
	gluCylinder(quadric, 0.1, 0.1, 2.0, 30, 30);
	glPopMatrix();

	//back branch
	glPushMatrix();
	glTranslatef(-1.0, 2.5, -4.0);
	glRotatef(-170, 0.5, 1.0, 1.0);
	gluCylinder(quadric, 0.1, 0.1, 2.0, 30, 30);
	glPopMatrix();

	//right branch
	glPushMatrix();
	glTranslatef(-1.0, 2.5, -4.0);
	glRotatef(90, 0.0, 1.0, 1.0);
	gluCylinder(quadric, 0.1, 0.1, 2.0, 30, 30);
	glPopMatrix();

	//leaves on the branches
	glColor3f(0.0f, 0.8f, 0.0f); 
	glPushMatrix();
	glTranslatef(-2.0, 3.5, -2.0);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.0, 3.8, -1.8);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 3.7, -2.0);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.8, 3.5, -2.0);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 3.8, -2.0);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 3.5, -2.0);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef( -0.5, 4.0, -2.0);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 4.3, -3.5);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef( -0.3, 4.0, -3.5);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

}

//fourth 3D object is a car
void car()
{
	carList = glGenLists(1);
	glNewList(carList, GL_COMPILE);

	//cube for the car body
	glColor3f(0.3, 0.3, 0.2);
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			glVertex3fv(carVertices[indices[i][j]]);
		}
	}
	glEnd();

	//smaller cube for the roof
	glColor3f(0.0, 0.0, 0.5);
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			glVertex3fv(roofVertices[indices[i][j]]);
		}
	}
	glEnd();

	//sphere for the back wheel
	glPushMatrix();
	glColor3f(0.1, 0.1, 0.1);
	glTranslatef(-2.5, 0.3, -0.5);
	glutSolidSphere(0.5f, 50, 50);

	glPopMatrix();


	//sphere for the front wheel
	glPushMatrix();
	glColor3f(0.1, 0.1, 0.1);
	glTranslatef(-4.8, 0.3, -0.5);
	glutSolidSphere(0.5f, 50, 50);

	glPopMatrix();

	//sphere for the back wheel
	glPushMatrix();
	glColor3f(0.1, 0.1, 0.1);
	glTranslatef(-2.5, 0.3, 0.75);
	glutSolidSphere(0.5f, 50, 50);

	glPopMatrix();

	//sphere for the back wheel
	glPushMatrix();
	glColor3f(0.1, 0.1, 0.1);
	glTranslatef(-4.8, 0.3, 0.75);
	glutSolidSphere(0.5f, 50, 50);

	glPopMatrix();

	//front bumper
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			glVertex3fv(bumperVertices[indices[i][j]]);
		}
	}
	glEnd();

	glEndList();

	glCallList(carList);
}

//fifth 3D object is a house
void house() {

	houseList = glGenLists(1);
	glNewList(houseList, GL_COMPILE);

	//the body of the house
	glColor3f(0.7, 0.5, 0.3);
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			glVertex3fv(houseVertices[indices[i][j]]);
		}
	}
	glEnd();

	//door
	glColor3f(0.4f, 0.2f, 0.1f); // Dark brown color for the door
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			glVertex3fv(doorVertices[indices[i][j]]);
		}
	}
	glEnd();

	//left window
	glColor3f(0.6f, 0.8f, 1.0f); // Light blue color for the window
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			glVertex3fv(lWindowVertices[indices[i][j]]);
		}
	}
	glEnd();

	//right window
	glColor3f(0.6f, 0.8f, 1.0f); // Light blue color for the window
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			glVertex3fv(rWindowVertices[indices[i][j]]);
		}
	}
	glEnd();

	glColor3f(0.8f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			glVertex3fv(pyramidVertices[pyramidIndices[i][j]]);
		}
	}
	glEnd();

	glEndList();
	glCallList(houseList);

}

//draw all the objects
void render()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	background();

	ground();
	robot();
	car();
	house();
	tree();
	lampPost();
	
}

//initialize
void init()
{
	//set the background colour to clear
	glClearColor(1.0, 1.0, 1.0, 1.0);


	// Enable depth testing for proper Z-ordering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); 

	glEnable(GL_COLOR_MATERIAL); // enable coloring
	glEnable(GL_LIGHTING);        // Enable lighting

	glEnable(GL_FOG);

	GLfloat fogColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glFogfv(GL_FOG_COLOR, fogColor);

	// Set fog parameters
	glFogi(GL_FOG_MODE, GL_EXP2); // Exponential squared fog for a denser effect
	glFogf(GL_FOG_DENSITY, 0.02f); // Fog density (adjust this value for thicker or thinner fog)
	glHint(GL_FOG_HINT, GL_DONT_CARE); // Fog quality hint
	glFogf(GL_FOG_START, 5.0f); // Start distance for linear fog (if using GL_LINEAR mode)
	glFogf(GL_FOG_END, 30.0f);  // End distance for linear fog (if using GL_LINEAR mode)



	glEnable(GL_LIGHT0); // Default global light
	glEnable(GL_LIGHT1); // First lamp post light
	glEnable(GL_LIGHT2); // Second lamp post light

	
	GLfloat globalLightColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat globalLightPosition[] = { 0.0f, 10.0f, 0.0f, 0.0f }; // Directional light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, globalLightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, globalLightPosition);

	GLfloat lightPos[] = { 10.0f, 2.2f, 10.0f, 1.0f };
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 0.0f, 0.0f, 1.0f, 0.0f }; //red

	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);

	//light 2
	GLfloat light_position2[] = { -10.0, 2.2, 10.0, 1.0 };
	GLfloat light_ambient2[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse2[] = { 1.0f, 0.0f, 0.0f, 0.0f };//blue
	GLfloat light_specular2[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);


	makeImage();  // Replace with the correct filename
	
	render();
	
}

//set up the view 
void setupCamera() {

	//set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1.0, 1.0, 100.0); // Field of view, aspect ratio, near, far

	//set up the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(8.0, 8.0, 30.0, // Eye position
		0.5, 1.0, 0.5, // Look at position
		0.0, 1.0, 0.0); // Up vector

}
void display() {
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	GLfloat globalLightPosition[] = { 0.0f, 10.0f, 10.0f, 1.0f }; // Directional light
	glLightfv(GL_LIGHT0, GL_POSITION, globalLightPosition);

	GLfloat light_position1[] = {6.0f, 2.2f, 4.0f, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	GLfloat light_position2[] = { -6.0, 2.2, -4.0, 1.0 };
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

	background();
	render();
	

	glCallList(robotList);
	glCallList(groundList);

	scaleAndTranslateHouse(5.0, 0.0, 10.0, 0.5, 0.5, 0.5);

	glutSwapBuffers();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Final Project: option 1");

	glEnable(GL_DEPTH_TEST); // Enable depth testing for correct Z-ordering


	setupCamera();
	init();
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}