#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#include <stdlib.h>
#include<string.h>
#define PI 3.14159265359

//Vektor kamere
float lx=0.0f,lz=-1.0f, ly=0.0f;
//pozicija kamere(igraca)
float x=5.0f, z=9.0f,  y=0.0f;

//Ugao pomeranja misa i osetljivost misa
float angle_x=0.0f, angle_y=0.0f;
GLfloat mouse_x, mouse_y;
float sensitivity_of_mouse = 0.2;


// Odbrojavanje do kraja igre 
GLfloat game_time = 0;
int game_over;
GLfloat diff_time;
GLfloat beginTime;

//Odbrojavanje
char disp_time[1000];
int tmp;
//Parametar rotacije meda
float rotation_parametar=0;

// Broj osvojenih bodova
int score;
char disp_score[256];

static int timer_ongoing;

// Tajmer za ispaljivanje kuglice 
float t;
static int move_ball;

// Trenutne koordinate kuglice 
float x_ball; 
float y_ball; 
float z_ball; 

// Vektor pravac kuglice
float bx;
float by;
float bz;

// Brzina kuglice 
float v;

float random_x[5]; 
float random_y[5];
float random_z[5];

static int window_width, window_height;

static void on_display();
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_mouse(int button, int state, int x, int y);
static void on_mouse_motion(int x, int y);
static void on_timer(int value);
static void specialKeys(int key, int xx, int yy);


static void initialize();
static void draw_wood();
static void random_position_for_bears();
static void draw_bear();

static void moving_ball(int value);
static void shoot();
static void drawPointer() ;

static void print_score();
static void print_time();


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
    glutMouseFunc(on_mouse);
	glutPassiveMotionFunc(on_mouse_motion);
	

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
    
     for(int i=0; i<4; i++)
    {
        random_x[i] = rand()/(float)RAND_MAX *6;
        random_y[i] = rand() / (float)RAND_MAX;
        random_z[i] = rand() / (float)RAND_MAX *5;

        
    }
   
   
    // Odbrojavanje do kraja igre 
	game_over = 0;
	game_time = 0;
	
    timer_ongoing = 0;
	
	t = 0;
	move_ball = 0;
	//Vektor pravca kuglice
    bx = 0.0f;
	by = 0.0f;
	bz = -1.0f;

	v = 8.0f;
	
	score = 0;
    
    
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
        case 'g':
		case 'G':
			if(!timer_ongoing){
				glutTimerFunc(0, on_timer, 0);
				beginTime = glutGet(GLUT_ELAPSED_TIME);
				score = 0;
				timer_ongoing = 1;
		
			}
			break;
        case 'R':
        case 'r':
        		beginTime = glutGet(GLUT_ELAPSED_TIME);
				score = 0;
                game_time=0;
             
    }
    
    
}

static void on_timer(int value){

	if(value != 0)
		return;
	
	//Broji milisekunde od prethodnog poziva ove f-je	
	float newTime = glutGet(GLUT_ELAPSED_TIME);
    
	diff_time = (newTime - beginTime)/1000.;
	beginTime = newTime; 
 	game_time += diff_time; // odbrojavanje do kraja igre
 	
    int tmp = sprintf(disp_time,"%.2f", game_time);

	
	rotation_parametar += 1;
 	
	if(game_time >= 20.00){
		game_over = 1;
		timer_ongoing = 0;
        print_score();
     //   printf("Kraj igre!\nOsvojeni broj poena je: %d\n", score);
      //  exit(0);
	}
	
	if(timer_ongoing)
		glutTimerFunc(0, on_timer, 0);
}



void draw_wood(){


    //Pod
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.15, 0);
    glBegin(GL_POLYGON);
        glNormal3f(0, 1, 0);

        glVertex3f(10, 0, 10);
        glVertex3f(10, 0, -5);
        glVertex3f(-1, 0, -5);
        glVertex3f(-1, 0, 10);
    glEnd();

    // Levi zid 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.25, 0);
    glBegin(GL_POLYGON);
        glNormal3f(1, 0, 0);
            
        glVertex3f(-1, 0, 10);
        glVertex3f(-1, 0, -5);
        glVertex3f(-1, 5, -5);
        glVertex3f(-1, 5, 10);
    glEnd();
   
    
    // Desni zid
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.25, 0);
    glBegin(GL_POLYGON);
        glNormal3f(-1, 0, 0);
        
        glVertex3f(10, 0, -5);
        glVertex3f(10, 5, -5);
        glVertex3f(10, 5, 10);
        glVertex3f(10, 0, 10);
    glEnd();
          

    //Prednji zid 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.45, 0);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);
        
        glVertex3f(-1, 0, -5);
        glVertex3f(10, 0, -5);
        glVertex3f(10, 5, -5);
        glVertex3f(-1, 5, -5);
    glEnd();
    
    // Zadnji zid
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.45, 0);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, -1);
        
        glVertex3f(-1, 0, 10);
        glVertex3f(10, 0, 10);
        glVertex3f(10, 5, 10);
        glVertex3f(-1, 5, 10);
    glEnd();
    
    // Plafona 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0.50, 0.89);
    glBegin(GL_POLYGON);
        glNormal3f(0, -10, 0);
        
        glVertex3f(-1, 5, 10);
        glVertex3f(10, 5, 10);
        glVertex3f(10, 5, -5);
        glVertex3f(-1, 5, -5);
    glEnd();
    
}
    

static void draw_bear()
{
    glRotatef(rotation_parametar/1000, 0, 1, 0);
    glTranslatef(0,0.65,0);
    
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
    float fraction = 0.15f; //brzina pomeraja
    
    switch(key){
    case GLUT_KEY_LEFT:
        if (x > 9)
            x -= 0.25;
         else if (x < -0.5)
            x += 0.25;
        else if (z < -4)
            z += 0.25;
        else if (z > 9)
            z -= 0.25;
       
        else {
            x += lz * fraction;
            z -= lx * fraction;
        }
        break;
    case GLUT_KEY_RIGHT:
         if (x > 9)
            x -= 0.25;
         else if (x < -0.5)
            x += 0.25;
        else if (z < -4)
            z += 0.25;
        else if (z > 9)
            z -= 0.25;
       
        
        else {
            x -= lz * fraction;
            z += lx * fraction;
        }
        break;
    case GLUT_KEY_UP:
         if (x > 9)
            x -= 0.25;
         else if (x < -0.5)
            x += 0.25;
        else if (z < -4)
            z += 0.25;
        else if (z > 9)
            z -= 0.25;
       
        else{
           x += lx * fraction;
           z += lz * fraction;
        }
        break;
    case GLUT_KEY_DOWN:
        if (x > 9)
            x -= 0.25;
         else if (x < -0.5)
            x += 0.25;
        else if (z < -4)
            z += 0.25;
        else if (z > 9)
            z -= 0.25;
       
        else{
           x -= lx * fraction;
           z -= lz * fraction;
        }
        break;
    
    }  
}

static void on_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt( x, 1, z,
                x+lx, 1+ly, z+lz,
                0, 1, 0 );
    
    //draw_axis(15);
    draw_wood();
    random_position_for_bears();
    
    // Koordinate kuglice- inicijalizacija
    x_ball = x;
	y_ball =y;
	z_ball = z;
	shoot();
    
    drawPointer(); 
    print_time();  	
    print_score();
    
    glutSwapBuffers();
}

static void random_position_for_bears()
{
     for(int i = 0; i < 4; i++)
    {
       
			glPushMatrix();
                if(i%2==0)
                    glTranslatef(random_x[i]+0.5,0,random_z[i]);
                else if(i%2==1)   
                    glTranslatef(random_x[i],0,random_z[i]-0.5);
                draw_bear();
            glPopMatrix();
        
    }
}

// Nisan (iscrtavanje) 
static void drawPointer() 
{  
	glMatrixMode(GL_PROJECTION); 
	
    glPushMatrix();  
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); 
	
    glPushMatrix(); 
	glLoadIdentity();
	
	glColor3f(1, 0, 0);
	gluOrtho2D(0.0, window_width, window_height, 0.0);
	char display_string[20];
	
    int words = sprintf(display_string,"%s", "O");
	
    if(words < 0)
		exit(1);
	
    glRasterPos2i(window_width/2, window_height/2); 
	
    int d = (int) strlen(display_string);
	
    for (int i = 0; i < d; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, display_string[i]);
	
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix(); 
    
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix(); 
    
	glutPostRedisplay();
}


static void on_mouse(int button, int state, int x, int y){
	(void)x;
	(void)y;
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){
				if(!move_ball && timer_ongoing == 1){
					bx = lx;
					by = ly;
					bz = lz;
					t = 0;
					glutTimerFunc(20, moving_ball, 1);
					move_ball = 1; 
				}
			}		
			break;
	}
}

// Ispaljivanje kuglice 
static void shoot(){
	glPushMatrix();
	glColor3f(0.75, 0, 0.65);
	
	x_ball = x_ball + t * bx * v;
	y_ball = y_ball + 1 + t * by * v; 
	z_ball = z_ball + t * bz * v;
	
	glTranslatef(x_ball, y_ball, z_ball);
	glutSolidSphere( 0.05, 30, 30);
	glPopMatrix();
}

static void moving_ball(int value){
	
	if (value != 1)
		return;
	
	// Loptica je izasla izvan zidova 
	if(x_ball > 10.0f || x_ball < -1.0f || y_ball > 5.0f || y_ball < 0.0f  || z_ball > 10.0f || z_ball <-5.0f)
    {
		move_ball = 0;
        t = 0;
	return;        
    }
	
	//Provera da li je kuglica pogodila medu
	int i;
	for(i = 0; i <4; i++)
    {
        if(z_ball  <= random_z[i] + 0.65 && z_ball >= random_z[i] - 0.65 
            && y_ball <= random_y[i] + 0.65 && y_ball >= random_y[i] - 0.65
            && x_ball <= random_x[i] + 0.65 && x_ball >= random_x[i] - 0.65)
        {
            
		// Novi meda
		random_x[i] = rand()/(float)RAND_MAX*6;
		random_y[i] = rand()/(float)RAND_MAX;
		random_z[i] = rand()/(float)RAND_MAX*5;
		
        score += 1;
	
		// Ako je kuglica pogodila medu nestaje
		x_ball = 100;
		y_ball = 100;
		z_ball = 100;
			
        }
	}
	
	t += 0.2f;
    
	if (move_ball)
		glutTimerFunc(20, moving_ball, value);
}

static void on_mouse_motion(int x, int y){
	
	// Ako kursor izadje van prozora
	if( x > window_width-2.0f){
		glutWarpPointer(window_width/2, window_height/2);
	}
	else if (x < 2.0f){
		glutWarpPointer(window_width/2, window_height/2);
	}
	else if (y > window_height-2.0f){
		glutWarpPointer(window_width/2, window_height/2);
	}
	else if (y < 2.0f){ 
		glutWarpPointer(window_width/2, window_height/2);
	}
	else {
		
		glutSetCursor(GLUT_CURSOR_NONE);
        angle_x += (mouse_y - y)*sensitivity_of_mouse;
        angle_y += (x - mouse_x)*sensitivity_of_mouse;
        
		mouse_x = x;
		mouse_y = y;
		
		//Dozvoljeno kretanje misa levo-desno
		if(angle_x > 60.0f)
			angle_x= 60.0f;
		if(angle_x < -20.0f)
			angle_x = -20.0f;
		//Dozvoljeno kretanje misa dole-gore
		if(angle_y > -45.0f)
			angle_y = -45.0f;
		if(angle_y < -135.0f)
			angle_y = -135.0f;
		
        //Vektor pomeranja kamere
        lx = cos(PI/180.0*angle_x)*cos(PI/180.0*angle_y);
		ly = sin(PI/180.0*angle_x);
		lz = cos(PI/180.0*angle_x) * sin(PI/180.0*angle_y);
	}
}

//Ispis rezultata
static void print_score()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            
            gluOrtho2D(0, 600, 600, 0);
            char text[11];
            int chars=0;
            
            chars= sprintf(text, "%s%d", "Score:\n", score);
            
            glColor3f(0.75, 0, 0.25);
            glRasterPos2d(100, 100);
            
            int i;
            
            for(i=0; i<chars; i++)
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
            
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// Ispis vremena
static void print_time() 
{  
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix();  
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity();
	
	glColor3f(0.75, 0, 0.25);
	gluOrtho2D(0.0, window_width, window_height, 0.0);    
    
	
    char display_string[20];
	int words = sprintf(display_string,"%s", "Time:");
	
    glRasterPos2d(50, 50);

    int d = (int) strlen(display_string);
	for (int i = 0; i < d; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, display_string[i]);
	
	glRasterPos2d(50, 70);

    int l = (int) strlen(disp_time);
	for (int i = 0; i < l; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, disp_time[i]);
	
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix(); 
	glutPostRedisplay();
}
