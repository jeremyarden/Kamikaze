/*
 *
 * Demonstrates how to load and display an Wavefront OBJ file.
 * Using triangles and normals as static object. No texture mapping.
 *
 * OBJ files must be triangulated!!!
 * Non triangulated objects wont work!
 * You can use Blender to triangulate
 *
 */

#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#define KEY_ESCAPE 27

using namespace std;

/************************************************************************
  Window
 ************************************************************************/

typedef struct {
    int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

/*Constants*/
GLfloat initAnglePyramid = 0.5f;  // Rotational angle for pyramid [NEW]
GLfloat initAngleCubeX = 0.0f;     // Rotational angle for cube [NEW]
GLfloat initAngleCubeY = 0.0f;
GLfloat initAngleCubeZ = 0.0f;
GLfloat initScale = 1.0f;

GLfloat initCameraX = 0.0f;
GLfloat initCameraY = 0.0f;
GLfloat initCameraZ = 5.0f;

GLfloat initPickX = 0.0f;
GLfloat initPickY = 0.0f;
GLfloat initPickZ = -7.0f;

GLfloat initUpVectorX = 0.0f;
GLfloat initUpVectorY = 1.0f;
GLfloat initUpVectorZ = 0.0f;

GLfloat initTheta = 0.0f;
GLfloat initPhi = 0.0f;


/* Global variables */
char title[] = "3D Shapes with animation";
GLfloat anglePyramid = initAnglePyramid;  // Rotational angle for pyramid [NEW]
GLfloat angleCubeX = initAngleCubeX;     // Rotational angle for cube [NEW]
GLfloat angleCubeY = initAngleCubeY;
GLfloat angleCubeZ = initAngleCubeZ;

GLfloat scale = initScale;

GLfloat cameraX = initCameraX;
GLfloat cameraY = initCameraY;
GLfloat cameraZ = initCameraZ;

GLfloat pickX = initPickX;
GLfloat pickY = initPickY;
GLfloat pickZ = initPickZ;

GLfloat upVectorX = initUpVectorX;
GLfloat upVectorY = initUpVectorY;
GLfloat upVectorZ = initUpVectorZ;

GLfloat theta = initTheta;
GLfloat phi = initPhi;


/***************************************************************************
  OBJ Loading
 ***************************************************************************/

class Model_OBJ
{
  public:
	Model_OBJ();
    void calculateNormal(float* coord1,float* coord2,float* coord3, float* norm );
    int Load(char *filename);	// Loads the model
	void Draw();					// Draws the model on the screen
	void Release();				// Release the model

	float* normals;							// Stores the normals
    float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles

};


#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9
using namespace std;

Model_OBJ::Model_OBJ()
{
	this->TotalConnectedTriangles = 0;
	this->TotalConnectedPoints = 0;
}

void Model_OBJ::calculateNormal( float *coord1, float *coord2, float *coord3 ,float* norm)
{
   /* calculate Vector1 and Vector2 */
   float va[3], vb[3], vr[3], val;
   va[0] = coord1[0] - coord2[0];
   va[1] = coord1[1] - coord2[1];
   va[2] = coord1[2] - coord2[2];

   vb[0] = coord1[0] - coord3[0];
   vb[1] = coord1[1] - coord3[1];
   vb[2] = coord1[2] - coord3[2];

   /* cross product */
   vr[0] = va[1] * vb[2] - vb[1] * va[2];
   vr[1] = vb[0] * va[2] - va[0] * vb[2];
   vr[2] = va[0] * vb[1] - vb[0] * va[1];

   /* normalization factor */
   val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );

	norm[0] = vr[0]/val;
	norm[1] = vr[1]/val;
	norm[2] = vr[2]/val;
    cout<<"anjing2"<<norm;

}


int Model_OBJ::Load(char* filename)
{
	string line;
	ifstream objFile (filename);
	if (objFile.is_open())													// If obj file is open, continue
	{

		objFile.seekg (0, ios::end);										// Go to end of the file,
		long fileSize = objFile.tellg();									// get file size
		objFile.seekg (0, ios::beg);										// we'll use this to register memory for our 3d model

		vertexBuffer = (float*) malloc (fileSize);							// Allocate memory for the verteces
		Faces_Triangles = (float*) malloc(fileSize*sizeof(float));			// Allocate memory for the triangles
		normals  = (float*) malloc(fileSize*sizeof(float));					// Allocate memory for the normals

		int triangle_index = 0;												// Set triangle index to zero
		int normal_index = 0;												// Set normal index to zero
        int i = 0;

		while (! objFile.eof() )											// Start reading file data
		{
		    i = i+1;
			getline (objFile,line);											// Get line from file

			if (line.c_str()[0] == 'v'&& line.c_str()[1]!= 't')										// The first character is a v: on this line is a vertex stored.
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf

				sscanf(line.c_str(),"%f %f %f ",							// Read floats from the line: v X Y Z
					&vertexBuffer[TotalConnectedPoints],
					&vertexBuffer[TotalConnectedPoints+1],
					&vertexBuffer[TotalConnectedPoints+2]);

				TotalConnectedPoints += POINTS_PER_VERTEX;					// Add 3 to the total connected points
			}
			if (line.c_str()[0] == 'f')										// The first character is an 'f': on this line is a point stored
			{
		    	line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf

				int vertexNumber[4] = { 0, 0, 0 };
				int tempa,tempb,tempc;
                sscanf(line.c_str(),"%d/%d %d/%d %d/%d ",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],
					&tempa,								// First point of our triangle. This is an
					&vertexNumber[1],
					&tempb,					     // pointer to our vertexBuffer list
					&vertexNumber[2],
					&tempc);										// each point represents an X,Y,Z.

				vertexNumber[0] -= 1;										// OBJ file starts counting from 1
				vertexNumber[1] -= 1;										// OBJ file starts counting from 1
				vertexNumber[2] -= 1;										// OBJ file starts counting from 1


				/********************************************************************
				 * Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z).
				 * The vertexBuffer contains all verteces
				 * The triangles will be created using the verteces we read previously
				 */

				int tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					Faces_Triangles[triangle_index + tCounter   ] = vertexBuffer[3*vertexNumber[i] ];
					Faces_Triangles[triangle_index + tCounter +1 ] = vertexBuffer[3*vertexNumber[i]+1 ];
					Faces_Triangles[triangle_index + tCounter +2 ] = vertexBuffer[3*vertexNumber[i]+2 ];
					tCounter += POINTS_PER_VERTEX;
				}

				/*********************************************************************
				 * Calculate all normals, used for lighting
				 */

				float coord1[3] = { Faces_Triangles[triangle_index], Faces_Triangles[triangle_index+1],Faces_Triangles[triangle_index+2]};

				float coord2[3] = {Faces_Triangles[triangle_index+3],Faces_Triangles[triangle_index+4],Faces_Triangles[triangle_index+5]};
				float coord3[3] = {Faces_Triangles[triangle_index+6],Faces_Triangles[triangle_index+7],Faces_Triangles[triangle_index+8]};
				float norm[3];
				this->calculateNormal( coord1, coord2, coord3,norm);
				tCounter = 0;

				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					normals[normal_index + tCounter ] = norm[0];

					normals[normal_index + tCounter +1] = norm[1];
					normals[normal_index + tCounter +2] = norm[2];
					tCounter += POINTS_PER_VERTEX;

				}

				triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
				normal_index += TOTAL_FLOATS_IN_TRIANGLE;
				TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;
			}
		}
		objFile.close();														// Close OBJ file
	}
	else
	{
		cout << "Unable to open file";
	}
	return 0;
}

void Model_OBJ::Release()
{
	free(this->Faces_Triangles);
	free(this->normals);
	free(this->vertexBuffer);
}

void Model_OBJ::Draw()
{
 	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
 	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3,GL_FLOAT,	0,Faces_Triangles);				// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
}

/***************************************************************************
 * Program code
 ***************************************************************************/

Model_OBJ obj;
float g_rotation;
glutWindow win;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt( 0,1,4, 0,0,0, 0,1,0);
	glScalef(scale, scale, scale);
	glRotatef(angleCubeX, 1.0f, 0.0f, 0.0f);  // Rotate about (1,1,1)-axis [NEW]
    glRotatef(angleCubeY, 0.0f, 1.0f, 0.0f);
    glRotatef(angleCubeZ, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);
    gluLookAt(cameraX, cameraY, cameraZ, cameraX+pickX, cameraY+pickY, cameraZ+pickZ,  upVectorX, upVectorY, upVectorZ);
	glPushMatrix();
		glRotatef(90,0,1,0);
		g_rotation++;
		obj.Draw();
	glPopMatrix();
	glutSwapBuffers();
}


void initialize ()
{
    glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, win.width, win.height);
	GLfloat aspect = (GLfloat) win.width / win.height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);
    glMatrixMode(GL_MODELVIEW);
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.1f, 0.0f, 0.5f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

/*
void keyboard ( unsigned char key, int x, int y )
{
  switch ( key ) {
    case KEY_ESCAPE:
      exit ( 0 );
      break;
    default:
      break;
  }
}
*/


void reset() {
	anglePyramid = initAnglePyramid;  // Rotational angle for pyramid [NEW]
	angleCubeX = initAngleCubeX;     // Rotational angle for cube [NEW]
	angleCubeY = initAngleCubeY;
	angleCubeZ = initAngleCubeZ;

	scale = initScale;

	cameraX = initCameraX;
	cameraY = initCameraY;
	cameraZ = initCameraZ;

	phi = initPhi;
	theta = initTheta;

}

void keyboard(unsigned char key, int x, int y){
    switch(key) {
        case 'a': case 'A': angleCubeY += 2.5f;  glutPostRedisplay(); break;
        case 'd': case 'D': angleCubeY -= 2.5f;  glutPostRedisplay(); break;
        case 'w': case 'W': angleCubeX += 2.5f;  glutPostRedisplay(); break;
        case 's': case 'S': angleCubeX -= 2.5f;  glutPostRedisplay(); break;
        case 'q': case 'Q': angleCubeZ += 2.5f;  glutPostRedisplay(); break;
        case 'e': case 'E': angleCubeZ -= 2.5f;  glutPostRedisplay(); break;
        case 'r': case 'R': reset();  glutPostRedisplay(); break;
        case '+': case '=': scale += 0.05f;  glutPostRedisplay(); break;
        case '-': scale -= 0.05f;  glutPostRedisplay(); break;

    }
}


int main(int argc, char **argv)
{
	// set window values
	win.width = 640;
	win.height = 480;
	win.title = "OpenGL/GLUT OBJ Loader.";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  // Display Mode
	glutInitWindowSize(win.width,win.height);					// set window size
	glutCreateWindow(win.title);								// create Window
	glutDisplayFunc(display);									// register Display Function
	glutIdleFunc( display );									// register Idle Function
    glutKeyboardFunc( keyboard );								// register Keyboard Handler
	initialize();
	//D:/Juro/Tugas-Tugas/KULIAH/GRAFKOM/CobaOBJ/atena.obj
	obj.Load("C:/Users/Yanichi/Documents/untitled.obj");
	glutMainLoop();												// run GLUT mainloop
	return 0;
}

