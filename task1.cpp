#include<stdio.h>
#include<stdlib.h>
#include<math.h>
using namespace std;
// #ifdef _linux_
//     #include <GL/glut.h>
// #elif WIN32
//     #include<windows.h>
//     #include<GL/glut.h>
// #endif

#include <GL/glut.h>
int drawgrid;
int drawaxes;

//camera rotation angle
double angle;
//camera moving speed
double speed;

class point {
    public:

    double x, y, z;

    point(){
        x = y = z = 0;
    }
    
    point(double xVal, double yVal, double zVal){
        x = xVal;
        y = yVal;
        z = zVal;
    }
    
    point addPoint(point poin){
        point p(x+poin.x, y+poin.y, z+poin.z);
        return p;
    }

    double dotProduct(point p){
        return (x*p.x)+(y*p.y)+(z*p.z);
    }

    point scalarMultiplication(double scale){
        double x1 = x*scale;
        double y1 = y*scale;
        double z1 = z*scale;
        return point(x1,y1,z1);
    }

    point crossProduct(point p){
        double x1 = y*p.z - z*p.y;
        double y1 = z*p.x - x*p.z;
        double z1 = x*p.y - y*p.x;
        return point(x1, y1, z1);
    }

    void printData(){
        printf("x - %f, ", x);
        printf("y - %f, ", y);
        printf("z - %f\n", z);
    }

};


//Cameras variable
point camPos, l, u, r;


void drawAxes(float size){
    glBegin(GL_LINES);{
        glColor3f(1,0,0);
        
        //x-axis
        glVertex3f(-size,0, 0);
        glVertex3f(size, 0, 0);
        //y-axis
        glVertex3f(0,size, 0);
        glVertex3f(0,-size, 0);
        //z-axis
        glVertex3f(0, 0, size);
        glVertex3f(0, 0, -size);

    }glEnd();
}

void drawSquare(float arm){
    glBegin(GL_QUADS);{
        glColor3f(1,0,0);
        glVertex2f(-arm/2, -arm/2);
        glVertex2f(arm/2, -arm/2);
        glVertex2f(arm/2, arm/2);
        glVertex2f(-arm/2, arm/2);
    }glEnd();
}




void display(){
	//Clear and set bg color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Camera m Load matrix for 3d
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camPos.x, camPos.y, camPos.z, camPos.x+l.x, camPos.y+l.y, camPos.z+l.z, u.x, u.y, u.z);
    glMatrixMode(GL_MODELVIEW);
    drawAxes(100);
    drawSquare(10);

    glutSwapBuffers();

}


void animate(){//changes in display to show
	// angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

//point pos;
void init(){
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    // point temp(10,10,10);
    // pos = temp;
    // camPos = point(10,10,10);
    // l = point(-10,-10,-10);
    // u = point(0,1,0);
    // r = point(1,0,0);
    // angle = 0.03;

    camPos = point(4,4,21);
    l = point(-4,-4,-21);
    u = point(0,1,0);
    r = point(1,0,0);
    angle = 0.03;
    speed = 1;

    //load projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1, 1, 1000);
}



// point pos2(3,3,3);
// point pos3;
void keyBoardListen(unsigned char key, int x, int y){
    switch(key){
        case '1': //rotate left (u axis, clockwise-daane)
            r = r.scalarMultiplication(cos(angle)).addPoint(r.crossProduct(u).scalarMultiplication(sin(-angle)));      
            l = l.scalarMultiplication(cos(angle)).addPoint(l.crossProduct(u).scalarMultiplication(sin(-angle)));
            break; 
        case '2': //rotate right (u axis, antiClockwise)
            l = l.scalarMultiplication(cos(angle)).addPoint(l.crossProduct(u).scalarMultiplication(sin(angle))); 
            r = r.scalarMultiplication(cos(angle)).addPoint(r.crossProduct(u).scalarMultiplication(sin(angle))); 
            break;  
        case '3': //look up (r axis, clockwise)
            l = l.scalarMultiplication(cos(angle)).addPoint(l.crossProduct(r).scalarMultiplication(sin(-angle))); 
            u = u.scalarMultiplication(cos(angle)).addPoint(u.crossProduct(r).scalarMultiplication(sin(-angle)));
            break;           
        case '4': //look down (r axis, aticlockwise)
            l = l.scalarMultiplication(cos(angle)).addPoint(l.crossProduct(r).scalarMultiplication(sin(angle))); 
            u = u.scalarMultiplication(cos(angle)).addPoint(u.crossProduct(r).scalarMultiplication(sin(angle)));
            break;  
        case '5': //tilt (l axis, anticlockwise)
            r = r.scalarMultiplication(cos(angle)).addPoint(r.crossProduct(l).scalarMultiplication(sin(angle))); 
            u = u.scalarMultiplication(cos(angle)).addPoint(u.crossProduct(l).scalarMultiplication(sin(angle)));
            break;   
        case '6': //tilt (l axis, clockwise)
            r = r.scalarMultiplication(cos(angle)).addPoint(r.crossProduct(l).scalarMultiplication(sin(-angle))); 
            u = u.scalarMultiplication(cos(angle)).addPoint(u.crossProduct(l).scalarMultiplication(sin(-angle)));
            break;
        case 'w':
            camPos = camPos.addPoint(u.scalarMultiplication(speed));
            l = l.addPoint(u.scalarMultiplication(-speed));
            break;
        case 's':
            camPos = camPos.addPoint(u.scalarMultiplication(-speed));
            l = l.addPoint(u.scalarMultiplication(speed));
            break;
        default:
            break;
    }
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			camPos = camPos.addPoint(l.scalarMultiplication(-0.5*speed));
			break;
		case GLUT_KEY_UP:		// up arrow key
			camPos = camPos.addPoint(l.scalarMultiplication(0.5*speed));
			break;

		case GLUT_KEY_RIGHT:
			camPos = camPos.addPoint(r.scalarMultiplication(speed));
			break;
		case GLUT_KEY_LEFT:
			camPos = camPos.addPoint(r.scalarMultiplication(-speed));
			break;

		case GLUT_KEY_PAGE_UP:
            camPos = camPos.addPoint(u.scalarMultiplication(speed));
			break;
		case GLUT_KEY_PAGE_DOWN:
			camPos = camPos.addPoint(u.scalarMultiplication(-speed));
            break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}

int main(int argc, char** argv){
    
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("OpenGL Demo");

    init();
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);

    glutKeyboardFunc(keyBoardListen);
    glutSpecialFunc(specialKeyListener);

    glutMainLoop();

    return 0;
}