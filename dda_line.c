#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

float x1, y1, x2, y2;

int drawn = 0;

void display(void) {
    if (drawn) return;  // Prevent multiple draws

    printf("Display called once\n");
    drawn = 1;

    float dx, dy, x, y, step, Xin, Yin;
    int i;

    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

    dx = x2 - x1;
    dy = y2 - y1;

    step = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);

    Xin = dx / step;
    Yin = dy / step;

    x = x1;
    y = y1;

    glBegin(GL_POINTS);
    for (i = 0; i <= step; i++) {
        glVertex2i((int)(x + 0.5), (int)(y + 0.5)); // Round to nearest int
        x += Xin;
        y += Yin;
    }
    glEnd();

    glFlush();
}

void myInit(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);   // Black background
    glColor3f(1.0, 1.0, 1.0);           // White drawing color
    glPointSize(2.0);                   // Visible point size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0); // Define coordinate system
}

int main(int argc, char** argv) {
    printf("Enter x1: ");
    scanf("%f", &x1);
    printf("Enter y1: ");
    scanf("%f", &y1);
    printf("Enter x2: ");
    scanf("%f", &x2);
    printf("Enter y2: ");
    scanf("%f", &y2);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Fixed DDA Line Drawing");
    myInit();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
