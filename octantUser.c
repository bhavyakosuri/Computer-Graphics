#define GLUT_DISABLE_ATEXIT_HACK

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Global input points
int cx, cy;
int x1_oct1, y1_oct1;

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 400.0, 0.0, 400.0);
}

void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

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

    int dx = x1_oct1 - cx;
    int dy = y1_oct1 - cy;

    // Octant color array
    float colors[8][3] = {
        {1.0, 0.0, 0.0}, // Octant 1 - Red
        {0.0, 1.0, 0.0}, // Octant 2 - Green
        {0.0, 0.0, 1.0}, // Octant 3 - Blue
        {1.0, 1.0, 0.0}, // Octant 4 - Yellow
        {1.0, 0.0, 1.0}, // Octant 5 - Magenta
        {0.0, 1.0, 1.0}, // Octant 6 - Cyan
        {0.5, 0.5, 0.5}, // Octant 7 - Gray
        {1.0, 0.5, 0.0}  // Octant 8 - Orange
    };

    // Octant endpoint calculations (rotations and reflections)
    int dxs[8] = {
        dx,  dy,   dy,   dx,  -dx, -dy, -dy, -dx
    };
    int dys[8] = {
        dy,  dx,  -dx,  -dy, -dy, -dx,  dx,  dy
    };

    for (int i = 0; i < 8; i++) {
        int x2 = cx + dxs[i];
        int y2 = cy + dys[i];
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        printf("Octant %d: (%d, %d) to (%d, %d)\n", i + 1, cx, cy, x2, y2);
        drawBresenhamLine(cx, cy, x2, y2);
    }

    // Draw center point
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(5.0);
    setPixel(cx, cy);

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: exit(0); // ESC
        case 'r':
        case 'R':
            glutPostRedisplay();
            break;
    }
}

int main(int argc, char** argv) {
    printf("Enter center point (cx cy): ");
    scanf("%d %d", &cx, &cy);

    printf("Enter end point of Octant 1 line (x1 y1) (should be in Octant 1): ");
    scanf("%d %d", &x1_oct1, &y1_oct1);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham's Line in 8 Octants - User Input");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    printf("Press 'R' to redraw, ESC to exit\n\n");

    glutMainLoop();
    return 0;
}
