#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <cmath>
using namespace std;

float x_center, y_center, r;

void run()
{
    glutPostRedisplay();
}

void draw_circle(float x, float y, float r)
{
    float x0 = 0, y0 = r, p = 5./4. - r;
    glBegin(GL_POINTS);
    while(x0 <= y0)
    {
        glVertex2d(x0+x, y0+y);
        glVertex2d(x0+x, -y0+y);
        glVertex2d(-x0+x, -y0+y);
        glVertex2d(-x0+x, y0+y);
        glVertex2d(y0+x, x0+y);
        glVertex2d(y0+x, -x0+y);
        glVertex2d(-y0+x, -x0+y);
        glVertex2d(-y0+x, x0+y);
        if(p<0)
        {
            x0 += 1;
            p += (2*x0 + 1);
        }
        else
        {
            x0 += 1;
            y0 -= 1;
            p += (2*x0 + 1 - 2*y0);
        }
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3d(0.6, 0.7, 0.8);
    draw_circle(x_center, y_center, r);

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    cout << "Enter center coordinate x: ";
    cin >> x_center;
    cout << "Enter center coordinate y: ";
    cin >> y_center;
    cout << "Enter radius: ";
    cin >> r;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    glutInitWindowSize (800,800);
    glutCreateWindow("Circle");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glutDisplayFunc(&display);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-400, 400, -400, 400, 0, 1);
    glutIdleFunc(&run);
    glutMainLoop();
    return EXIT_SUCCESS;
}
