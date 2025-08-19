#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Vertex positions (v0 to v5)
int x[] = {100, 180, 240, 220, 140, 100};
int y[] = {300, 330, 270, 200, 180, 240};

// Colors for labels (matching the example image)
float labelColors[6][3] = {
    {1, 0, 0},    // v0 - red
    {0, 1, 0},    // v1 - green
    {0, 0, 1},    // v2 - blue
    {0, 1, 1},    // v3 - cyan
    {1, 0, 1},    // v4 - magenta
    {1, 1, 0.5}   // v5 - yellowish
};

// Draw pixel
void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Midpoint Circle Algorithm
void midpointCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int p = 1 - r;
    while (x <= y) {
        setPixel(xc + x, yc + y);
        setPixel(xc - x, yc + y);
        setPixel(xc + x, yc - y);
        setPixel(xc - x, yc - y);
        setPixel(xc + y, yc + x);
        setPixel(xc - y, yc + x);
        setPixel(xc + y, yc - x);
        setPixel(xc - y, yc - x);
        x++;
        if (p < 0)
            p += 2 * x + 1;
        else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}

// Bresenham Line Drawing Algorithm
void drawLine(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    while (1) {
        setPixel(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

// Draw vertex label
void drawLabel(int x, int y, const char *label, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2i(x - 6, y - 4);
    for (int i = 0; i < strlen(label); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[i]);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw white LineLoop (v0→v1→...→v5→v0)
    glColor3f(1, 1, 1);
    for (int i = 0; i < 6; i++) {
        int j = (i + 1) % 6;
        drawLine(x[i], y[i], x[j], y[j]);
    }

    // Draw white circles and colored vertex labels
    for (int i = 0; i < 6; i++) {
        glColor3f(1, 1, 1); // white circle
        midpointCircle(x[i], y[i], 12);

        char label[4];
        sprintf(label, "v%d", i);
        drawLabel(x[i], y[i], label, labelColors[i][0], labelColors[i][1], labelColors[i][2]);
    }

    glFlush();
}

void init() {
    glClearColor(0, 0, 0, 1); // Black bg
    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 400, 0, 400);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("LineLoop with Midpoint Circle & Colored Labels");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
