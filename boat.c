#include<stdlib.h>
#include<stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  // Boat base (trapezoid)
  glBegin(GL_LINE_LOOP);
    glVertex2i(200, 200);
    glVertex2i(400, 200);
    glVertex2i(350, 150);
    glVertex2i(250, 150);
  glEnd();

  // Sail (triangle)
  glBegin(GL_LINE_LOOP);
    glVertex2i(300, 200);
    glVertex2i(300, 300);
    glVertex2i(350, 200);
  glEnd();

  // Mast (vertical line)
  glBegin(GL_LINES);
    glVertex2i(300, 200);
    glVertex2i(300, 300);
  glEnd();

  glFlush();
}

void myInit(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0); // black background
  glColor3f(1.0, 1.0, 1.0); // white lines
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(100, 150);
  glutCreateWindow("Simple Boat Outline");
  myInit();
  glutDisplayFunc(display);
  glutMainLoop();
}
