#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#include <stdlib.h>
#include<stdbool.h>
#define PI 3.14

// http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/

//Vektor kretanja (pravca) kamere
float lx=0.0f,lz=-1.0f, ly=0;
//pozicija kamere u x,z ravni
float x=0.0f,z=5.0f,y=0;
float angle = 0.0f;


static int window_width, window_height;
static void on_display();
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void specialKeys(int key, int xx, int yy);
static void random_position_for_bears();
static void on_timer(int value);

static void draw_wood();
static void initialize();

int main(int argc, char** argv)
{
    
    glutInit(&argc, argv);
    glutCreateWindow("hunter");
    glutInitWindowPosition(100,100);
    glutInitWindowSize(600,600);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    
    initialize();
    
    glutKeyboardFunc(on_keyboard);
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutSpecialFunc(specialKeys);

    glutFullScreen();
    
    
    glutMainLoop();
    
    return 0;
}

static void initialize()
{
    
    glClearColor(0.75,0.75 , 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    
}

static void on_reshape(int width, int height)
{
    window_height = height;
    window_width = width;
    glViewport(0, 0, window_width, window_height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 60, window_width/(float)window_height, 1, 500);
    
}


static void on_keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
    
    }
    
    
}


void draw_wood(){


    //Pod
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.15, 0);
    glBegin(GL_POLYGON);
        glNormal3f(0, 1, 0);

        glVertex3f(10, 0, 10);
        glVertex3f(10, 0, -10);
        glVertex3f(-10, 0, -10);
        glVertex3f(-10, 0, 10);
    glEnd();

    // Levi zid 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.25, 0);
    glBegin(GL_POLYGON);
        glNormal3f(1, 0, 0);
            
        glVertex3f(-10, 0, 10);
        glVertex3f(-10, 0, -10);
        glVertex3f(-10, 5, -10);
        glVertex3f(-10, 5, 10);
    glEnd();
    
    //Zadnji zid 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.45, 0);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);
        
        glVertex3f(-10, 0, -10);
        glVertex3f(10, 0, -10);
        glVertex3f(10, 5, -10);
        glVertex3f(-10, 5, -10);
    glEnd();
    
    
    
    // Desni zid
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.25, 0);
    glBegin(GL_POLYGON);
        glNormal3f(-1, 0, 0);
        
        glVertex3f(10, 0, -10);
        glVertex3f(10, 5, -10);
        glVertex3f(10, 5, 10);
        glVertex3f(10, 0, 10);
    glEnd(); 

    // Prednji zid
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.45, 0);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, -1);
        
        glVertex3f(-10, 0, 10);
        glVertex3f(10, 0, 10);
        glVertex3f(10, 5, 10);
        glVertex3f(-10, 5, 10);
    glEnd();
    
    // Plafona 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.50, 0.89);
    glBegin(GL_POLYGON);
        glNormal3f(0, -10, 0);
        
        glVertex3f(-10, 5, 10);
        glVertex3f(10, 5, 10);
        glVertex3f(10, 5, -10);
        glVertex3f(-10, 5, -10);
    glEnd();
    
}
  
static void specialKeys(int key, int xx, int yy)
{

    float fraction = 0.1f;

    switch (key) {
        case GLUT_KEY_LEFT :
            angle -= 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_RIGHT :
            angle += 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_UP :
            x += lx * fraction;
            z += lz * fraction;
            break;
        case GLUT_KEY_DOWN :
            x -= lx * fraction;
            z -= lz * fraction;
            break;
    }
  glutPostRedisplay();
    
}

static void on_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt( x, 1.0f, z,
                x+lx, 1.0f, z+lz,
                0.0f, 1.0f, 0.0f);
    draw_wood();
    
    glutSwapBuffers();
    
}
