#include<stdlib.h>
#include<stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw house base (rectangle)
  glBegin(GL_LINE_LOOP);
    glVertex2i(200, 200);
    glVertex2i(400, 200);
    glVertex2i(400, 350);
    glVertex2i(200, 350);
  glEnd();

  // Draw roof (triangle)
  glBegin(GL_LINE_LOOP);
    glVertex2i(200, 350);
    glVertex2i(400, 350);
    glVertex2i(300, 450);
  glEnd();

  // Draw door (rectangle)
  glBegin(GL_LINE_LOOP);
    glVertex2i(270, 200);
    glVertex2i(330, 200);
    glVertex2i(330, 280);
    glVertex2i(270, 280);
  glEnd();

  glFlush();
}

void myInit(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0); // black background
  glColor3f(1.0, 1.0, 1.0); // default drawing color (white)
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(100, 150);
  glutCreateWindow("Simple House Outline");
  myInit();
  glutDisplayFunc(display);
  glutMainLoop();
}
