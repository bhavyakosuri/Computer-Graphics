#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Animation variables
float time_val = 0.0f;
float fish1_x = -50, fish2_x = -80, fish3_x = -100;
float submarine_x = 50, submarine_y = 150;
float propeller_angle = 0;
float lid_angle = 0;
float bubble_y[5] = {50, 60, 40, 70, 55};
float bubble_x[5] = {120, 135, 110, 180, 165};

// =================================================================
// ALGORITHM IMPLEMENTATIONS
// =================================================================

// DDA Line Algorithm Implementation
void drawLineDDA(int x1, int y1, int x2, int y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float step;
    
    if (abs(dx) > abs(dy))
        step = abs(dx);
    else
        step = abs(dy);
        
    float x_inc = dx / step;
    float y_inc = dy / step;
    
    float x = x1, y = y1;
    
    glBegin(GL_POINTS);
    for (int i = 0; i <= step; i++) {
        glVertex2i((int)(x + 0.5), (int)(y + 0.5));
        x += x_inc;
        y += y_inc;
    }
    glEnd();
}

// Bresenham Line Algorithm Implementation
void drawLineBresenham(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    int x = x0, y = y0;
    
    glBegin(GL_POINTS);
    while (1) {
        glVertex2i(x, y);
        
        if (x == x1 && y == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
    glEnd();
}

// Midpoint Circle Algorithm Implementation
void drawCircleMidpoint(int xc, int yc, int radius) {
    int x = 0;
    int y = radius;
    int p = 1 - radius;
    
    glBegin(GL_POINTS);
    
    // Plot initial points
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
    
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        
        // Plot 8-way symmetry points
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);
    }
    glEnd();
}

// Scanline Fill Algorithm for Polygon
void scanlineFillPolygon(int vertices[][2], int n) {
    int ymin = SCREEN_HEIGHT, ymax = 0;
    
    // Find ymin and ymax
    for (int i = 0; i < n; i++) {
        if (vertices[i][1] < ymin) ymin = vertices[i][1];
        if (vertices[i][1] > ymax) ymax = vertices[i][1];
    }
    
    // For each scanline
    for (int y = ymin; y <= ymax; y++) {
        int intersections[20];
        int count = 0;
        
        // Find intersections with polygon edges
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            int y1 = vertices[i][1];
            int y2 = vertices[j][1];
            
            if (y1 != y2 && y >= (y1 < y2 ? y1 : y2) && y < (y1 > y2 ? y1 : y2)) {
                int x1 = vertices[i][0];
                int x2 = vertices[j][0];
                int x = x1 + (float)(y - y1) * (x2 - x1) / (y2 - y1);
                intersections[count++] = x;
            }
        }
        
        // Sort intersections
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (intersections[i] > intersections[j]) {
                    int temp = intersections[i];
                    intersections[i] = intersections[j];
                    intersections[j] = temp;
                }
            }
        }
        
        // Fill between pairs
        glBegin(GL_POINTS);
        for (int i = 0; i < count; i += 2) {
            for (int x = intersections[i]; x <= intersections[i + 1]; x++) {
                glVertex2i(x, y);
            }
        }
        glEnd();
    }
}

// =================================================================
// OBJECT DRAWING FUNCTIONS
// =================================================================

// Draw submarine using multiple algorithms
void drawSubmarine(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    
    // Submarine body (Scanline fill)
    glColor3f(0.2f, 0.4f, 0.2f);
    int sub_body[][2] = {{0, 20}, {150, 20}, {170, 40}, {150, 60}, {0, 60}, {-20, 40}};
    scanlineFillPolygon(sub_body, 6);
    
    // Submarine outline (Bresenham)
    glColor3f(0.1f, 0.3f, 0.1f);
    glPointSize(2.0f);
    for (int i = 0; i < 6; i++) {
        int next = (i + 1) % 6;
        drawLineBresenham(sub_body[i][0], sub_body[i][1], 
                         sub_body[next][0], sub_body[next][1]);
    }
    
    // Periscope (DDA)
    glColor3f(0.3f, 0.3f, 0.3f);
    glPointSize(3.0f);
    drawLineDDA(120, 60, 120, 90);
    drawLineDDA(120, 90, 140, 90);
    
    // Porthole (Midpoint Circle)
    glColor3f(0.5f, 0.7f, 1.0f);
    glPointSize(2.0f);
    drawCircleMidpoint(40, 40, 12);
    
    // Propeller (with rotation transformation)
    glPushMatrix();
    glTranslatef(170, 40, 0.0f);
    glRotatef(propeller_angle, 0.0f, 0.0f, 1.0f);
    glColor3f(0.6f, 0.6f, 0.6f);
    glPointSize(2.0f);
    drawLineDDA(-15, 0, 15, 0);
    drawLineDDA(0, -15, 0, 15);
    glPopMatrix();
    
    glPopMatrix();
}

// Draw fish using algorithms and transformations
void drawFish(float x, float y, float scale, float r, float g, float b, int direction) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale * direction, scale, 1.0f);
    
    // Fish body (Scanline fill)
    glColor3f(r, g, b);
    int fish_body[][2] = {{0, 10}, {30, 5}, {35, 15}, {30, 25}, {0, 20}};
    scanlineFillPolygon(fish_body, 5);
    
    // Fish outline (Bresenham)
    glColor3f(r * 0.7f, g * 0.7f, b * 0.7f);
    glPointSize(2.0f);
    for (int i = 0; i < 5; i++) {
        int next = (i + 1) % 5;
        drawLineBresenham(fish_body[i][0], fish_body[i][1], 
                         fish_body[next][0], fish_body[next][1]);
    }
    
    // Fish tail (DDA)
    glColor3f(r * 0.8f, g * 0.8f, b * 0.8f);
    glPointSize(2.0f);
    drawLineDDA(35, 15, 50, 5);
    drawLineDDA(35, 15, 50, 25);
    drawLineDDA(50, 5, 50, 25);
    
    // Fish eye (Midpoint Circle)
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);
    drawCircleMidpoint(8, 15, 4);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircleMidpoint(8, 15, 2);
    
    glPopMatrix();
}

// Draw coral using DDA and scanline fill
void drawCoral(float x, float y, float height, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    
    // Coral branches (DDA)
    glColor3f(r, g, b);
    glPointSize(3.0f);
    
    // Main branch
    drawLineDDA(0, 0, 5, height);
    
    // Side branches with sway animation
    float sway = sin(time_val + x * 0.01f) * 3.0f;
    drawLineDDA(5, height * 0.3f, 15 + sway, height * 0.5f);
    drawLineDDA(5, height * 0.6f, -10 + sway, height * 0.8f);
    drawLineDDA(5, height * 0.8f, 12 + sway, height);
    
    // Coral polyps (Midpoint Circles)
    glColor3f(r * 1.2f, g * 1.2f, b * 1.2f);
    glPointSize(2.0f);
    for (int i = 0; i < 5; i++) {
        drawCircleMidpoint(15 + sway + i * 3, height * 0.5f + i * 5, 3);
    }
    
    glPopMatrix();
}

// Draw bubbles using circle algorithm and transformations
void drawBubbles() {
    glColor3f(0.8f, 0.9f, 1.0f);
    glPointSize(1.5f);
    
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        
        // Bubble movement transformation
        float scale = 1.0f + (bubble_y[i] / 200.0f);
        glTranslatef(bubble_x[i], bubble_y[i], 0.0f);
        glScalef(scale, scale, 1.0f);
        
        // Draw bubble (Midpoint Circle)
        int radius = 5 + i * 2;
        drawCircleMidpoint(0, 0, radius);
        
        glPopMatrix();
        
        // Update bubble position
        bubble_y[i] += 1.0f + i * 0.5f;
        if (bubble_y[i] > SCREEN_HEIGHT + 20) {
            bubble_y[i] = 0;
            bubble_x[i] = 100 + rand() % 200;
        }
    }
}

// Draw seaweed using DDA with sway animation
void drawSeaweed(float x, float height) {
    glColor3f(0.1f, 0.6f, 0.1f);
    glPointSize(4.0f);
    
    glPushMatrix();
    glTranslatef(x, 0, 0.0f);
    
    // Sway transformation
    float sway = sin(time_val + x * 0.02f) * 8.0f;
    glRotatef(sway, 0.0f, 0.0f, 1.0f);
    
    // Draw seaweed stem (DDA)
    drawLineDDA(0, 0, 0, height);
    
    // Draw leaves
    for (int i = 20; i < height; i += 25) {
        float leaf_sway = sin(time_val * 2 + i * 0.1f) * 5.0f;
        drawLineDDA(0, i, 10 + leaf_sway, i + 8);
        drawLineDDA(0, i, -10 + leaf_sway, i + 8);
    }
    
    glPopMatrix();
}

// Draw treasure chest with opening animation
void drawTreasureChest(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    
    // Chest base (Scanline fill)
    glColor3f(0.6f, 0.3f, 0.1f);
    int chest_base[][2] = {{0, 0}, {60, 0}, {60, 30}, {0, 30}};
    scanlineFillPolygon(chest_base, 4);
    
    // Chest base outline (Bresenham)
    glColor3f(0.4f, 0.2f, 0.0f);
    glPointSize(2.0f);
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        drawLineBresenham(chest_base[i][0], chest_base[i][1], 
                         chest_base[next][0], chest_base[next][1]);
    }
    
    // Chest lid with rotation transformation
    glPushMatrix();
    glTranslatef(0, 30, 0.0f);
    glRotatef(-lid_angle, 1.0f, 0.0f, 0.0f);  // Opening animation
    
    glColor3f(0.7f, 0.4f, 0.2f);
    int chest_lid[][2] = {{0, 0}, {60, 0}, {60, 20}, {0, 20}};
    scanlineFillPolygon(chest_lid, 4);
    
    glPopMatrix();
    
    // Treasure (visible when chest opens)
    if (lid_angle > 30.0f) {
        glColor3f(1.0f, 0.8f, 0.0f);
        glPointSize(2.0f);
        drawCircleMidpoint(30, 35, 8);
        drawCircleMidpoint(20, 32, 5);
        drawCircleMidpoint(40, 33, 6);
    }
    
    glPopMatrix();
}

// =================================================================
// CLIPPING DEMONSTRATION
// =================================================================

void drawClippingDemo() {
    // Define clipping window
    int clip_x = 550, clip_y = 100;
    int clip_width = 200, clip_height = 150;
    
    // Draw clipping window boundary
    glColor3f(1.0f, 1.0f, 0.0f);
    glPointSize(2.0f);
    drawLineBresenham(clip_x, clip_y, clip_x + clip_width, clip_y);
    drawLineBresenham(clip_x + clip_width, clip_y, clip_x + clip_width, clip_y + clip_height);
    drawLineBresenham(clip_x + clip_width, clip_y + clip_height, clip_x, clip_y + clip_height);
    drawLineBresenham(clip_x, clip_y + clip_height, clip_x, clip_y);
    
    // Enable scissor test for clipping
    glEnable(GL_SCISSOR_TEST);
    glScissor(clip_x, clip_y, clip_width, clip_height);
    
    // Draw fish that moves in and out of clipping window
    float clip_fish_x = 500 + sin(time_val * 0.5f) * 150;
    drawFish(clip_fish_x, 170, 0.8f, 1.0f, 0.2f, 0.2f, 1);
    
    // Draw coral that's partially clipped
    drawCoral(clip_x + 50, clip_y, 80, 1.0f, 0.5f, 0.8f);
    
    glDisable(GL_SCISSOR_TEST);
}

// =================================================================
// MAIN DISPLAY AND ANIMATION
// =================================================================

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Ocean background gradient effect
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.1f, 0.3f);  // Deep blue bottom
    glVertex2f(0, 0);
    glVertex2f(SCREEN_WIDTH, 0);
    glColor3f(0.0f, 0.3f, 0.6f);  // Lighter blue top
    glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
    glVertex2f(0, SCREEN_HEIGHT);
    glEnd();
    
    // Draw ocean floor
    glColor3f(0.8f, 0.7f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(SCREEN_WIDTH, 0);
    glVertex2f(SCREEN_WIDTH, 80);
    glVertex2f(0, 80);
    glEnd();
    
    // Draw sunlight rays (DDA)
    glColor3f(1.0f, 1.0f, 0.8f);
    glPointSize(1.0f);
    for (int i = 0; i < 5; i++) {
        float x = 100 + i * 150;
        float alpha = 0.3f + 0.2f * sin(time_val + i);
        glColor3f(alpha, alpha, alpha * 0.8f);
        drawLineDDA(x, SCREEN_HEIGHT, x + 30, 200);
        drawLineDDA(x + 10, SCREEN_HEIGHT, x + 40, 200);
    }
    
    // Draw submarine with complex transformations
    drawSubmarine(submarine_x + cos(time_val * 0.3f) * 50, 
                  submarine_y + sin(time_val * 0.2f) * 20);
    
    // Draw school of fish with different transformations
    int fish_direction1 = (fish1_x > SCREEN_WIDTH + 60) ? -1 : 1;
    int fish_direction2 = (fish2_x > SCREEN_WIDTH + 60) ? -1 : 1;
    int fish_direction3 = (fish3_x > SCREEN_WIDTH + 60) ? -1 : 1;
    
    drawFish(fish1_x, 200 + sin(time_val) * 10, 1.0f, 1.0f, 0.4f, 0.0f, fish_direction1);
    drawFish(fish2_x, 280 + cos(time_val * 1.2f) * 15, 0.8f, 0.0f, 0.8f, 0.6f, fish_direction2);
    drawFish(fish3_x, 350 + sin(time_val * 0.8f) * 12, 1.2f, 0.9f, 0.9f, 0.2f, fish_direction3);
    
    // Draw coral reef
    drawCoral(200, 80, 60, 1.0f, 0.4f, 0.6f);
    drawCoral(250, 80, 80, 0.2f, 0.8f, 0.6f);
    drawCoral(300, 80, 50, 1.0f, 0.8f, 0.2f);
    drawCoral(350, 80, 70, 0.8f, 0.2f, 0.8f);
    
    // Draw seaweed forest
    drawSeaweed(100, 120);
    drawSeaweed(130, 100);
    drawSeaweed(450, 140);
    drawSeaweed(480, 110);
    drawSeaweed(650, 130);
    drawSeaweed(680, 95);
    
    // Draw treasure chest
    drawTreasureChest(400, 80);
    
    // Draw bubbles
    drawBubbles();
    
    // Draw clipping demonstration
    drawClippingDemo();
    
    // Algorithm labels
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(10, 550);
    
    glFlush();
}

void timer(int value) {
    // Update animation variables
    time_val += 0.1f;
    
    // Update fish positions
    fish1_x += 2.0f;
    fish2_x += 1.5f;
    fish3_x += 1.8f;
    
    // Reset fish when they go off screen
    if (fish1_x > SCREEN_WIDTH + 60) fish1_x = -60;
    if (fish2_x > SCREEN_WIDTH + 60) fish2_x = -60;
    if (fish3_x > SCREEN_WIDTH + 60) fish3_x = -60;
    
    // Update propeller rotation
    propeller_angle += 15.0f;
    if (propeller_angle >= 360.0f) propeller_angle = 0.0f;
    
    // Update treasure chest animation
    if (time_val > 5.0f && lid_angle < 60.0f) {
        lid_angle += 2.0f;
    }
    
    glutPostRedisplay();
    glutTimerFunc(50, timer, 0);  // 20 FPS
}

void init() {
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Enable point smoothing for better visual quality
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    srand(time(NULL));  // Initialize random seed for bubbles
}

int main(int argc, char** argv) {
    printf("\n🌊 UNDERWATER OCEAN SCENE - Computer Graphics Case Study 🌊\n");
    printf("=====================================================\n");
    printf("Algorithms Demonstrated:\n");
    printf("✅ DDA Algorithm - Coral branches, submarine periscope, fish tails\n");
    printf("✅ Bresenham Line - Object outlines, clipping window\n");
    printf("✅ Midpoint Circle - Bubbles, fish eyes, treasure, porthole\n");
    printf("✅ Scanline Fill - Fish bodies, coral, submarine hull\n");
    printf("✅ 2D Transformations - Swimming, rotation, scaling, translation\n");
    printf("✅ Clipping - Scissor test demonstration with moving objects\n");
    printf("=====================================================\n\n");
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("🌊 UNDERWATER OCEAN SCENE - 19CSE433 Case Study 🌊");
    
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    
    printf("Press Ctrl+C to exit the program.\n");
    printf("Watch the complex animations and transformations!\n\n");
    
    glutMainLoop();
    return 0;
}


