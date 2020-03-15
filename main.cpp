#include <stdio.h>
#include "gl/glut.h"
#include "visuals.h"


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);															// initialize GLUT library state

	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);						// Set up the display using the GLUT functions to get rid of the window setup details:
																					// - Use true RGB colour mode ( and transparency )
																					// - Enable double buffering for faster window update
																					// - Allocate a Depth-Buffer in the system memory or in the video memory if 3D acceleration available
	
	glutInitWindowSize(480, 480);													// Define the main window size and initial position 
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Planetary System");											// Create and label the main window

	Setup();																		// Configure various properties of the OpenGL rendering context

																					// Callbacks for the GL and GLUT events:

	glutDisplayFunc(Render);														// The rendering function 				
	glutReshapeFunc(Resize);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();																	//Enter main event handling loop
	
	return 0;
}