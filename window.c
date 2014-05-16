# include <GLUT/glut.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <assert.h>

# define SCALE 0.0045	/*How big the graphics on the screen are in comparison with the real size*/

# define RIGHT 600.0	/*Size of the box in x*/
# define TOPB  600.0	/*Size of the box in y*/
# define FRONT 600.0	/*Size of the box in z*/

# define HALFX 0.5*(RIGHT)
# define HALFY 0.5*(TOPB)
# define HALFZ 0.5*(FRONT)

int WindowDump(void);

double t_max=4000000.0;	/*Maximum time for the simulation*/
double t=0.0;		/*General Time*/
double tp=0.0;		/*Time for the platform*/

double left_end=0.0,top=TOPB,bottom=0.0;
double delta_t=1.0e-2;	/*Time-step for integration*/
int counter;
char label[100];	/*Array for the label of the window*/

GLdouble golden=1.618034;
GLdouble lookx=2.5;
GLint width=600, height=600;
GLfloat angle=0.0;
GLfloat light_source1[] = { 2.0, 2.0, 20.0, 1.0};
GLfloat light_source2[] = { 2.0, 0.0, 2.0, 1.0};
GLfloat light_color1[] = {0.9, 1.0, 0.9, 1.0};
GLfloat light_color2[] = {1.0, 0.0, 0.5, 1.0};

/*************************************/
/*****OPENGL FUNCTIONS START HERE*****/
/*************************************/

void spin (void)

{
    angle+=1.0;
	glutPostRedisplay();
}

void drawString (char *s)

{
  	unsigned int i;
  	for (i = 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_8_BY_13, s[i]);
}


int WindowDump(void)

{
	int i,j;
	FILE *fptr;
	
	//	static int counter = 0; /* This supports animation sequences */
	char fname[2000];
	unsigned char *image;
	GLint stereo=1;
	
	/* Allocate our buffer for the image */
	if ((image = malloc(3*width*height*sizeof(char))) == NULL)
	{
		fprintf(stderr,"Failed to allocate memory for image\n");
		return(1);
	}
	
	glPixelStorei(GL_PACK_ALIGNMENT,1);
	
	/* Open the file */
	if (stereo)	sprintf(fname,"fgrav_%3.4lf.raw",t_max+t);
	
	else	sprintf(fname,"C_%04d.raw",counter);
	
	if ((fptr = fopen(fname,"w")) == NULL)
	{
		fprintf(stderr,"Failed to open file for window dump\n");
		return(1);
	}
	
	/* Copy the image into our buffer */
	
	glReadBuffer(GL_BACK_LEFT);
	glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);
	
	/* Write the raw file */
	fprintf(fptr,"P6\n%d %d\n255\n",width,height);
	
	for (j=height-1;j>=0;j--)
	{
		for (i=0;i<width;i++)
		{
			fputc(image[3*j*width+3*i+0],fptr);
			fputc(image[3*j*width+3*i+1],fptr);
			fputc(image[3*j*width+3*i+2],fptr);
		}
	}
	fclose(fptr);
	
	/* Clean up */
	counter++;
	free(image);
	return(0);
}

void display (void)

{
	int count=0,i=0;
	
    t=t+delta_t;
    tp=tp+delta_t;
	
	printf ("t = %0.4lf seg  \n",t);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* Clear the window */
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glLoadIdentity();
	
	gluLookAt (lookx,lookx*golden,lookx*golden*golden,
			   SCALE*HALFX,SCALE*HALFY,SCALE*HALFZ,
			   0.0,1.0,0.0);
    
    glRotatef(angle,1,0,0);
//    glRotatef(angle,0,1,0);
//    glRotatef(angle,0,0,1);
    
	/*******BEGIN TO DRAW THE BEADS****************************/
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_color2);
    glPushMatrix();
    glTranslatef(SCALE*(HALFX), SCALE*(HALFY), SCALE*(HALFZ));
    glutSolidSphere(1.0, 10., 10.);
    glPopMatrix();
    glLightfv(GL_LIGHT0,GL_POSITION,light_source1);
    
    
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	
	/*******FINISH DRAWING THE BEADS**************************/
	
	/*******BEGIN TO DRAW THE BOX*****************************/
		
	glLineWidth(1.0);
	glColor3f(1.0,1.0,1.0);
	
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0,SCALE*(bottom),0.0);
	glVertex3f(SCALE*(RIGHT),SCALE*(bottom),0.0);
	glVertex3f(SCALE*(RIGHT),SCALE*(top),0.0);
	glVertex3f(0.0,SCALE*(top),0.0);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0,SCALE*(bottom),SCALE*(FRONT));
	glVertex3f(SCALE*(RIGHT),SCALE*(bottom),SCALE*(FRONT));
	glVertex3f(SCALE*(RIGHT),SCALE*(top),SCALE*(FRONT));
	glVertex3f(0.0,SCALE*(top),SCALE*(FRONT));
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0,SCALE*(bottom),0.0);
	glVertex3f(0.0,SCALE*(bottom),SCALE*(FRONT));
	glVertex3f(SCALE*(RIGHT),SCALE*(bottom),SCALE*(FRONT));
	glVertex3f(SCALE*(RIGHT),SCALE*(bottom),0.0);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0,SCALE*(top),0.0);
	glVertex3f(0.0,SCALE*(top),SCALE*(FRONT));
	glVertex3f(SCALE*(RIGHT),SCALE*(top),SCALE*(FRONT));
	glVertex3f(SCALE*(RIGHT),SCALE*(top),0.0);
	glEnd();
	
	glLineWidth(1.0);
    
	/*******FINISH DRAWING THE BOX*****************************/
    WindowDump();
	glutSwapBuffers();
	count=0;
	assert(t<t_max);
}

void keyboard (unsigned char key, int x, int y)

{
	int keystate[1],i=0;
	switch (key)
	{
		case 27: exit(0);
		default: {printf("Nothing was calculated!!!!\n\a");break;}
	}
}

void reshape (int width, int height)

{
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,light_color1);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,light_color2);
	
	glViewport (0,0, (GLsizei) width, (GLsizei) height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60, (GLfloat)width/(GLfloat)height, 1.0, 100.0);
	glMatrixMode (GL_MODELVIEW);
}

int main (int argc, char** argv)

{
	/********OpenGL infinite loop********/
	glutInit (&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (width,height);
	glutInitWindowPosition (500,10);
	glutCreateWindow ("Asteroid Formation - 3D Gravity");
	glutDisplayFunc (display);
	glutKeyboardFunc (keyboard);
	glutReshapeFunc (reshape);
	glutIdleFunc (spin);
	glutMainLoop ();
	return (0);
}
