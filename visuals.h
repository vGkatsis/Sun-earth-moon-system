#define STARSNUM 1000

struct point
{
	float x;
	float y;
	float z;
};

struct face
{
	int vtx[3];
};

struct model
{
	point *obj_points;
	face *obj_faces;
	int vertices;
	int faces;
};

struct star
{
	float xpos;
	float ypos;
	float zpos;
};


//-------- Functions --------------------------------

void Render();																	// The function responsible for drawing everything in the OpenGL context associated to a window. 

void Resize(int w, int h);														// Handle the window size changes and define the world coordinate system and projection type

void Setup();																	// Set up the OpenGL state machine and create a light source

void Idle();

void ReadFile(model*);															//Function for reading a model file

void DisplayModel(model);														// Function for displaying a model

void Keyboard(unsigned char key, int x, int y);									// Function for handling keyboard events.

void sun();																		//Function for sun drawing

void starsfunc();																//Function for stars drawing

void earth();																	//Function for earth drawing

void moon();																	//Function for moon drawing

void axes();																	//Function for axes drawing 

void text(const char *str, float size);											//Function for text drawing