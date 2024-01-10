#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <GL/glut.h>

#define pi (2*acos(0.0))

struct pointt
{
	double x,y,z;
};

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

//Sphere variable
double dirAngle;
double ownAxisAngle;
point direction, upVector, translate;
double ballSpeed;


void drawGrid()
{
    glColor3f(0.6, 0.6, 0.6); // grey
    glBegin(GL_LINES);{
        for (int i = -100; i <= 100; i++){
            // lines parallel to z-axis
            glVertex3f(i * 20, -30, 700);
            glVertex3f(i * 20, -30, -700);

            // lines parallel to X-axis
            // glVertex3f(-500, i * 20, 0);
            // glVertex3f(500, i * 20, 0);
			glVertex3f(-500, -30, i * 20);
            glVertex3f(500, -30, i * 20);
			glVertex3f(-500, -30, (100+i) * 20);
            glVertex3f(500, -30, (100+i) * 20);
			
        }
    }glEnd();
}

void drawDirectionLine(){
	glColor3f(1, 0, 0);//red
	glBegin(GL_LINES);{
		//for the line
		for(int i = -2; i <= 2; i++){
			glVertex3f(i, -27, -150);
        	glVertex3f(i, -27, 0);
		}
		//for the arrow
		for(int i = 0; i < 5; i++){
			//left part
			glVertex3f((-i+0), -27, -150);
			glVertex3f((-i-10), -27, -120);
			//right part
			glVertex3f((i+0), -27, -150);
			glVertex3f((i+10), -27, -120);
		}
	}glEnd();
}

double colorVal1 = 0.8, colorVal2 = 0.2, colorVal;
void drawSphere(double radius, int sectors, int stacks)
{
	struct pointt points[100][100];
	double phi, theta;
	double rSinPhi,rcosPhi;
    //stack n sectors divided. so every part is a quad
	//generate starting points of the quads
	for(int i=0; i<=stacks; i++){
		phi = ((double)i / (double)stacks)* (pi/2);
		rcosPhi = radius*cos(phi);//rcos(phi)
		rSinPhi = radius*sin(phi);//rsin(phi), phi->stackAngle
		for(int j=0; j<=sectors; j++){
			theta = ((double)j / (double)sectors) * (2*pi);
			points[i][j].x= rcosPhi * cos(theta);//x=rcos(phi)cos(theta)
			points[i][j].y= rcosPhi * sin(theta);//y=rcos(phi)sin(theta)
			points[i][j].z= rSinPhi;//z=rsin(phi)
		}
	}

    //one row of point matrix -> one stack(starting points of all sectors (col wise))

	//draw quads using generated points
	for(int i=0; i<stacks; i++){
		// glColor3f(1.0*i/stacks, 1.0*i/stacks, 1.0*i/stacks);
		for(int j=0; j<sectors; j++)
		{
			if(j % 2 !=0){
				colorVal = colorVal1;
			}
			else{
				colorVal= colorVal2;
			}
			glColor3f(colorVal,1.0*i/stacks,0.5*i/stacks);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //z axis in height of the sphere
				//range of phi [-pi/2,pi/2], but we have only positive phi
				//so make it negative for the lower part from the center
				//lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawBoundaryLR(double xx1, double xx2, double zz){
	glColor3f(0.21, 0.105, 0.03);//red
	glBegin(GL_QUADS);{
		//left
		glVertex3f(xx1, 50, -zz);
		glVertex3f(xx1, -50, -zz);
		glVertex3f(xx1, -50, zz);
		glVertex3f(xx1, 50, zz);
		//right
		glVertex3f(xx2, 50, -zz);
		glVertex3f(xx2, -50, -zz);
		glVertex3f(xx2, -50, zz);
		glVertex3f(xx2, 50, zz);
		//upper
		glVertex3f(xx1, 50, zz);
		glVertex3f(xx1, -50, zz);
		glVertex3f(xx2, -50, zz);
		glVertex3f(xx2, 50, zz);
		//lower
		glVertex3f(xx1, 50, -zz);
		glVertex3f(xx1, -50, -zz);
		glVertex3f(xx2, -50, -zz);
		glVertex3f(xx2, 50, -zz);
	}glEnd();
}

double hypotenuse;
double angleRadian;
//for simulation(reflection and all)
int timeSimulation;

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case 'j': //rotate left(along y axis, cc)
            dirAngle += 2;
			if(dirAngle >= 360){
				dirAngle = dirAngle - 360;
			}
			angleRadian = dirAngle*3.1416/180;
			//printf("direc Angle %f\n", dirAngle);
			hypotenuse = sqrt(pow(direction.x, 2) + pow(direction.z,2));
			direction.x = hypotenuse * sin(-angleRadian);//chitro in my khata
			if(dirAngle > -90 && dirAngle < 90){
				direction.z = -sqrt(pow(hypotenuse,2) - pow(direction.x,2));//negative means forward
			}
			else{
				direction.z = sqrt(pow(hypotenuse,2) - pow(direction.x,2));//positive means backward towards me
			}
			//direction.printData();
			break; 
        case 'l': //rotate right (along y axis, Clockwise)
            dirAngle -= 2;
			if(dirAngle <= -360){
				dirAngle = dirAngle + 360;
			}
			angleRadian = dirAngle*3.1416/180;
			//printf("direc Angle %f\n", dirAngle);
			hypotenuse = sqrt(pow(direction.x, 2) + pow(direction.z,2));
			direction.x = hypotenuse * sin(-angleRadian);//chitro in my khata
			if(dirAngle > -90 && dirAngle < 90){
				direction.z = -sqrt(pow(hypotenuse,2) - pow(direction.x,2));//negative means forward
			}
			else{
				direction.z = sqrt(pow(hypotenuse,2) - pow(direction.x,2));//pos means backward
			}
			//direction.printData();
			break;  
		case 'i':	
			ownAxisAngle -= 7;
			translate = translate.addPoint(direction.scalarMultiplication(ballSpeed));
			break;
		case 'k':		
			ownAxisAngle += 7;
			translate = translate.addPoint(direction.scalarMultiplication(-ballSpeed));
			break;
		case 't':
			timeSimulation = 1 - timeSimulation;
			break;



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


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				printf("%f\n",dirAngle);
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();
	gluLookAt(camPos.x, camPos.y, camPos.z, camPos.x+l.x, camPos.y+l.y, camPos.z+l.z, u.x, u.y, u.z);
    glMatrixMode(GL_MODELVIEW);

    //the floor
	drawGrid();
    //the ball
	glPushMatrix();
	//rotating the sphere and direction
	glTranslatef(translate.x, translate.y, translate.z);
	glRotatef(dirAngle, 0, 1, 0);
	glRotatef(ownAxisAngle, 1, 0, 0);
	drawSphere(30,20,24);
	glPopMatrix();

	glPushMatrix();
	//rotating the direction
	glTranslatef(translate.x, translate.y, translate.z);
	glRotatef(dirAngle, 0, 1, 0);
	drawDirectionLine();
	glPopMatrix();
	drawBoundaryLR(-300,300, 300);

	//for double buffer
	glutSwapBuffers();
}

void collitionCheck(int time){
	if(timeSimulation){
		ownAxisAngle -= 25;
		translate = translate.addPoint(direction.scalarMultiplication(5*ballSpeed));
		if((translate.x < -270 && translate.z < -269) || (translate.x > 270 && translate.z < -269) || (translate.x < -270 && translate.z > 269) || (translate.x > 270 && translate.z > 269)){
			direction = direction.scalarMultiplication(-1);
			dirAngle = dirAngle + 180;
			printf("1)%f\n", dirAngle);
		}
		else if(translate.x < -270 || translate.x > 270){
			direction.x = -direction.x;
			dirAngle = 360 - dirAngle;
			if(dirAngle > 360){
				dirAngle = dirAngle - 360;
			}
			//dirAngle = dirAngle+180;
			printf("2)%f\n", dirAngle);
		}
		else if(translate.z < -269 || translate.z > 269){
			direction.z = -direction.z;
			dirAngle = 180-dirAngle;
			//dirAngle = dirAngle+180;
			printf("3)%f\n", dirAngle);
		}
	}
	else{
		angleRadian = dirAngle*3.1416/180;
		// printf("direc Angle %f\n", dirAngle);
		hypotenuse = sqrt(pow(direction.x, 2) + pow(direction.z, 2));
		direction.x = hypotenuse * sin(-angleRadian); // chitro in my khata
		if (dirAngle > -90 && dirAngle < 90)
		{
			direction.z = -sqrt(pow(hypotenuse, 2) - pow(direction.x, 2)); // negative means forward
		}
		else
		{
			direction.z = sqrt(pow(hypotenuse, 2) - pow(direction.x, 2)); // pos means backward
		}
	}
	glutPostRedisplay();
	glutTimerFunc(100, collitionCheck, 0);
}

void animate(){
	glutPostRedisplay();
}

void init(){
	

	//clear the screen
	glClearColor(0,0,0,0);

	camPos = point(0,0,200);
    l = point(0,0,-200);
    u = point(0,1,0);
    r = point(1,0,0);
    angle = 0.03;
    speed = 5;
	dirAngle = 0;
	ownAxisAngle = 0;
	direction = point(0, 0, -1);//z=-1 means forward
	upVector = point(0,1,0);
	translate = point(0,0,0);
	ballSpeed = 4;

	//simulation
	timeSimulation = 0;

	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(900, 700);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Rolling the ball");
	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)
	glutTimerFunc(300, collitionCheck, 0);

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
