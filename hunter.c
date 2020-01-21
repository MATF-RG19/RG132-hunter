#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#include <stdlib.h>
#define PI 3.14

float lx=0.0f,lz=-1.0f;
float x=0.0f,z=5.0f, y=0;
float angle = 0.0f;


float random_x[5]; 
float random_y[5];
float random_z[5];
//vreme proteklo od pocetka simulacije
//static float hours;
static int window_width, window_height;
static void on_display();
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void specialKeys(int key, int xx, int yy);
static void random_position_for_bears();

//static void on_timer(int value);

static void draw_wood();
static void draw_bear();
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

void draw_axis(float len) {
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(len,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,len,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,len);
    glEnd();
}

static void initialize()
{
    
    glClearColor(0.75,0.75 , 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    
     for(int i=0; i<5; i++)
    {
        random_x[i] = rand()/(float)RAND_MAX *9;
        random_y[i] = rand() / (float)RAND_MAX;
        random_z[i] = rand() / (float)RAND_MAX *8;
    }
    
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float light_position[] = {-1, 1, 1, 0};
    float light_ambient[] = {.3f, .3f, .3f, 1};
    float light_diffuse[] = {.7f, .7f, .7f, 1};
    float light_specular[] = {.7f, .7f, .7f, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
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
        case 'r':
        case 'R':
            //namestiti timer na 0 opet
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
          

    //Prednji zid 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.45, 0);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);
        
        glVertex3f(-10, 0, -10);
        glVertex3f(10, 0, -10);
        glVertex3f(10, 5, -10);
        glVertex3f(-10, 5, -10);
    glEnd();
    
    // Zadnji zid
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
    

static void draw_bear()
{
    glTranslatef(0,0.75,0);
    
    glPushMatrix();
    glColor3f(0.54, 0.29, 0.07);
    glutSolidSphere(0.65, 20, 20);
    glColor4f(0.54, 0.30, 0.07, -0.2);
    glTranslatef(0,0.85,0);
    
    glutSolidSphere(0.45, 15, 15);
    //glColor4f(0.54, 0.30, 0, -0.2);
    glTranslatef(-0.30,0.45,0);
    
    glColor3f(0.61, 0.29, 0.07);
    glutSolidSphere(0.15, 10, 10);
    glTranslatef(0.30,-0.45,0);

    glTranslatef(0.30,0.45,0);
    glColor3f(0.61, 0.29, 0.07);
    glutSolidSphere(0.15, 10, 10);
    glTranslatef(-0.30,-0.45,0);
    glPopMatrix();
}

static void specialKeys(int key, int xx, int yy)
{

    float fraction = 0.1f;

    switch (key) {
    case GLUT_KEY_RIGHT:
        if (x > 9.9)
            x -= 1;
        else if (z < -9.9)
            z += 1;
        else if (z > 9.9)
            z -= 1;
        else if (x < -9.9)
            x += 1;
        
        else {
            x += lz * fraction;
            z -= lx * fraction;
        }
        break;
    case GLUT_KEY_LEFT:
        if (x > 9.9)
            x -= 1;
        else if (z < -9.9)
            z += 1;
        else if (z > 9.9)
            z -= 1;
        else if (x < -9.9)
            x += 1;
        
        else {
            x -= lz * fraction;
            z += lx * fraction;
        }
        break;
  /*      case GLUT_KEY_UP :
            if (x > 9.9)
                x -= 0.25;
            else if (z < -9.9)
                z += 0.25;
            else if (z > 9.9)
                z -= 0.25;
            else if (x < -9.9)
                x += 0.25;
            else{
            x += lx * fraction;
            z += lz * fraction;
            }
            break;
        case GLUT_KEY_DOWN :
              if (x < -9.9)
                x += 0.25;
            else if( z > 9.9)
                z -= 0.25;
            else if (z < -9.9)
                z += 0.25;
            else if (x > 9.9)
                x -= 0.25;  
            else{
            x -= lx * fraction;
            z -= lz * fraction;
            }
            break;
   */ }
  glutPostRedisplay();
    
}

static void on_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt( x, 1, -6,
                x+lx, 1, z+lz,
                0, 1, 0);
    
    draw_axis(15);
    draw_wood();
    random_position_for_bears();
    
    glutSwapBuffers();
}

static void random_position_for_bears()
{
     for(int i = 0; i < 5; i++)
    {
			glPushMatrix();
                if(i%2==0)
                    glTranslatef(-1*random_x[i],0,random_z[i]);
                else if(/*i>5 && */i%2==0)   
                    glTranslatef(random_x[i],0,-1*random_z[i]);
                else
                    glTranslatef(random_x[i],0,random_z[i]);
               
                draw_bear();
            glPopMatrix();
		}
}
