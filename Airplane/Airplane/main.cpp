/*
 * OGL02Animation.cpp: 3D Shapes with animation
 */
// for MS Windows
// #include <GLUT/GLUT.h>  // GLUT, include glu.h and gl.h
// for GNU/Linux
#include <GL/gl.h>
#include <GL/glut.h>

#include <math.h>

#include <iostream>
using namespace std;

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

int refreshMills = 15;
 
/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}
 
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
   glTranslatef(0.0f, 0.0f, -7.0f);  // Move right and into the screen
   glRotatef(angleCubeX, 1.0f, 0.0f, 0.0f);  // Rotate about (1,1,1)-axis [NEW]
   glRotatef(angleCubeY, 0.0f, 1.0f, 0.0f);
   glRotatef(angleCubeZ, 0.0f, 0.0f, 1.0f);
   glScalef(scale, scale, scale);
   gluLookAt(cameraX, cameraY, cameraZ, pickX, pickY, pickZ,  upVectorX, upVectorY, upVectorZ);
   
   cout << "cameraX " << cameraX << endl;
   cout << "cameraY " << cameraY << endl;
   cout << "cameraZ " << cameraZ << endl;

   // cout << "upVectorX " << upVectorX << endl;
   // cout << "upVectorY " << upVectorY << endl;
   // cout << "upVectorZ " << upVectorZ << endl;


   glBegin(GL_POLYGON);                // Begin drawing the color cube with 6 quads
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
           glVertex3f( 1.0f, 1.0f, -1.0f);
           glVertex3f(-1.0f, 1.0f, -1.0f);
           glVertex3f(-1.0f, 1.0f,  1.0f);
           glVertex3f( 1.0f, 1.0f,  1.0f);
           // Bottom face (y = -1.0f)
           glColor3f(1.0f, 0.5f, 0.0f);     // Orange
           glVertex3f( 1.0f, -1.0f,  1.0f);
           glVertex3f(-1.0f, -1.0f,  1.0f);
           glVertex3f(-1.0f, -1.0f, -1.0f);
           glVertex3f( 1.0f, -1.0f, -1.0f);
      
           // Front face  (z = 1.0f)
           glColor3f(1.0f, 0.0f, 0.0f);     // Red
           glVertex3f( 1.0f,  1.0f, 1.0f);
           glVertex3f(-1.0f,  1.0f, 1.0f);
           glVertex3f(-1.0f, -1.0f, 1.0f);
           glVertex3f( 1.0f, -1.0f, 1.0f);
      
           // Back face (z = -1.0f)
           glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
           glVertex3f( 1.0f, -1.0f, -1.0f);
           glVertex3f(-1.0f, -1.0f, -1.0f);
           glVertex3f(-1.0f,  1.0f, -1.0f);
           glVertex3f( 1.0f,  1.0f, -1.0f);
      
           // Left face (x = -1.0f)
           glColor3f(0.0f, 0.0f, 1.0f);     // Blue
           glVertex3f(-1.0f,  1.0f,  1.0f);
           glVertex3f(-1.0f,  1.0f, -1.0f);
           glVertex3f(-1.0f, -1.0f, -1.0f);
           glVertex3f(-1.0f, -1.0f,  1.0f);
      
           // Right face (x = 1.0f)
           glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
           glVertex3f(1.0f,  1.0f, -1.0f);
           glVertex3f(1.0f,  1.0f,  1.0f);
           glVertex3f(1.0f, -1.0f,  1.0f);
           glVertex3f(1.0f, -1.0f, -1.0f);
        glEnd();  // End of drawing color-cube

   glEnd();  // End of drawing color-cube

   glBegin(GL_POLYGON);                // Begin drawing the color cube with 6 quads
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
           glVertex3f( 4.0f, 1.0f, -1.0f);
           glVertex3f(2.0f, 1.0f, -1.0f);
           glVertex3f(2.0f, 1.0f,  1.0f);
           glVertex3f( 4.0f, 1.0f,  1.0f);
      
           // Bottom face (y = -1.0f)
           glColor3f(1.0f, 0.5f, 0.0f);     // Orange
           glVertex3f( 4.0f, -1.0f,  1.0f);
           glVertex3f(2.0f, -1.0f,  1.0f);
           glVertex3f(2.0f, -1.0f, -1.0f);
           glVertex3f( 4.0f, -1.0f, -1.0f);
      
           // Front face  (z = 1.0f)
           glColor3f(1.0f, 0.0f, 0.0f);     // Red
           glVertex3f( 4.0f,  1.0f, 1.0f);
           glVertex3f(2.0f,  1.0f, 1.0f);
           glVertex3f(2.0f, -1.0f, 1.0f);
           glVertex3f( 4.0f, -1.0f, 1.0f);
      
           // Back face (z = -1.0f)
           glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
           glVertex3f( 4.0f, -1.0f, -1.0f);
           glVertex3f(2.0f, -1.0f, -1.0f);
           glVertex3f(2.0f,  1.0f, -1.0f);
           glVertex3f( 4.0f,  1.0f, -1.0f);
      
           // Left face (x = -1.0f)
           glColor3f(0.0f, 0.0f, 1.0f);     // Blue
           glVertex3f(2.0f,  1.0f,  1.0f);
           glVertex3f(2.0f,  1.0f, -1.0f);
           glVertex3f(2.0f, -1.0f, -1.0f);
           glVertex3f(2.0f, -1.0f,  1.0f);
      
           // Right face (x = 1.0f)
           glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
           glVertex3f(4.0f,  1.0f, -1.0f);
           glVertex3f(4.0f,  1.0f,  1.0f);
           glVertex3f(4.0f, -1.0f,  1.0f);
           glVertex3f(4.0f, -1.0f, -1.0f);
        glEnd();  // End of drawing color-cube
    


   glEnd();  // End of drawing color-cube
 
 
   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
 
   // Update the rotational angle after each refresh [NEW]
//   anglePyramid += 0.2f;
//   angleCube -= 0.15f;
}

float *cross(float x, float y, float z) {
	float *result = new float[3];

	float *up = new float[3];

	up[0] = 0.0f;
	up[1] = 1.0f;
	up[2] = 0.0f;


	result[0] = y * up[2] - z * up[1]; 
    result[1] = z * up[0] - x * up[2]; 
    result[2] = x * up[1] - y * up[0]; 


	return result;

}

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

void arrow(int key, int x, int y) {
	float *newUp;
	switch(key) {
		case GLUT_KEY_LEFT: theta -= 0.01f; glutPostRedisplay(); break;
        case GLUT_KEY_RIGHT: theta += 0.01f;  glutPostRedisplay(); break;
        case GLUT_KEY_DOWN: cameraY -= 0.01f;  glutPostRedisplay(); break;
        case GLUT_KEY_UP: cameraY += 0.01f;  glutPostRedisplay(); break;
	}

	float dx = cameraX - pickX;
	float dy = cameraY - pickY;
	float dz = cameraZ - pickZ;

	float radius = sqrt(dx*dx + dy*dy + dz*dz);
	cout << "radius " << radius << endl;
	cameraX = radius*cos(theta)*sin(theta);
	cameraY = radius*sin(phi)*sin(theta);
	cameraZ = radius*cos(theta);

	pickX += cameraX;
	pickY += cameraY;
	pickZ += cameraZ;
}
 

/* Called back when timer expired [NEW] */
void timer(int value) {
   glutPostRedisplay();      // Post re-paint request to activate display()
   glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}
 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}
 
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {


   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event

   initGL();
    glutKeyboardFunc(keyboard);// Our own OpenGL initialization
    glutSpecialFunc(arrow);
   //glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}
