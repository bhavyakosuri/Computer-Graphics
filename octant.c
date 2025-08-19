// Disable atexit hack to avoid __glut*WithExit errors in original GLUT
#define GLUT_DISABLE_ATEXIT_HACK

#include <windows.h>      // Required for OpenGL on Windows
#include <GL/gl.h>        // Core OpenGL
#include <GL/glu.h>       // OpenGL Utilities
#include <GL/glut.h>      // GLUT (original)
#include <stdio.h>
#include <stdlib.h>

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);      // White background
    glMatrixMode(GL_PROJECTION);          
    glLoadIdentity();
    gluOrtho2D(0.0, 400.0, 0.0, 400.0);    // 2D coordinate system
}

void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Traditional Bresenham's line drawing
void drawBresenhamLine(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int x_inc = (x0 < x1) ? 1 : -1;
    int y_inc = (y0 < y1) ? 1 : -1;
    int x = x0, y = y0;

    setPixel(x, y);

    if (dx > dy) {
        int p = 2 * dy - dx;
        for (int i = 0; i < dx; i++) {
            if (p >= 0) {
                y += y_inc;
                p -= 2 * dx;
            }
            x += x_inc;
            p += 2 * dy;
            setPixel(x, y);
        }
    } else {
        int p = 2 * dx - dy;
        for (int i = 0; i < dy; i++) {
            if (p >= 0) {
                x += x_inc;
                p -= 2 * dy;
            }
            y += y_inc;
            p += 2 * dx;
            setPixel(x, y);
        }
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(2.0);

    int cx = 200, cy = 200;
    printf("Drawing 8 lines from center (%d, %d):\n", cx, cy);

    // Octant 1
    glColor3f(1.0, 0.0, 0.0);  // Red
    printf("Octant 1 (Red): (%d,%d) to (%d,%d)\n", cx, cy, cx+80, cy+40);
    drawBresenhamLine(cx, cy, cx+80, cy+40);

    // Octant 2
    glColor3f(0.0, 1.0, 0.0);  // Green
    printf("Octant 2 (Green): (%d,%d) to (%d,%d)\n", cx, cy, cx+40, cy+80);
    drawBresenhamLine(cx, cy, cx+40, cy+80);

    // Octant 3
    glColor3f(0.0, 0.0, 1.0);  // Blue
    printf("Octant 3 (Blue): (%d,%d) to (%d,%d)\n", cx, cy, cx+40, cy-80);
    drawBresenhamLine(cx, cy, cx+40, cy-80);

    // Octant 4
    glColor3f(1.0, 1.0, 0.0);  // Yellow
    printf("Octant 4 (Yellow): (%d,%d) to (%d,%d)\n", cx, cy, cx+80, cy-40);
    drawBresenhamLine(cx, cy, cx+80, cy-40);

    // Octant 5
    glColor3f(1.0, 0.0, 1.0);  // Magenta
    printf("Octant 5 (Magenta): (%d,%d) to (%d,%d)\n", cx, cy, cx-80, cy-40);
    drawBresenhamLine(cx, cy, cx-80, cy-40);

    // Octant 6
    glColor3f(0.0, 1.0, 1.0);  // Cyan
    printf("Octant 6 (Cyan): (%d,%d) to (%d,%d)\n", cx, cy, cx-40, cy-80);
    drawBresenhamLine(cx, cy, cx-40, cy-80);

    // Octant 7
    glColor3f(0.5, 0.5, 0.5);  // Gray
    printf("Octant 7 (Gray): (%d,%d) to (%d,%d)\n", cx, cy, cx-40, cy+80);
    drawBresenhamLine(cx, cy, cx-40, cy+80);

    // Octant 8
    glColor3f(1.0, 0.5, 0.0);  // Orange
    printf("Octant 8 (Orange): (%d,%d) to (%d,%d)\n", cx, cy, cx-80, cy+40);
    drawBresenhamLine(cx, cy, cx-80, cy+40);

    // Center dot
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(5.0);
    setPixel(cx, cy);

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 27: // ESC
            exit(0);
        case 'r':
        case 'R':
            glutPostRedisplay();
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham's Line in 8 Octants");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    printf("Bresenham's Line Algorithm - 8 Octants Demo\n");
    printf("Press 'R' to redraw or ESC to exit\n\n");

    glutMainLoop();
    return 0;
}
