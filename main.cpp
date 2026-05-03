#include <GL/glut.h>
#include <cmath>

float fanAngle = 0.0f;
bool isFanOn = false;

float catX = -0.6f;
bool isCatMoving = false;

float jumpY = 0.0f;
float velocityY = 0.0f;
bool isJumping = false;

bool isNight = false;

float camAngleX = 20.0f;
float camAngleY = -30.0f;
float zoom = -3.0f;

void initLighting() {
    GLfloat light_pos[] = {0.0f, 1.5f, 1.0f, 1.0f};
    GLfloat ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

void drawFan() {
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.0f);
    glRotatef(fanAngle, 0.0f, 1.0f, 0.0f);

    glColor3f(0.2f, 0.2f, 0.2f);
    glutSolidCube(0.05);

    for (int i = 0; i < 4; i++) {
        glRotatef(90, 0, 1, 0);
        glPushMatrix();
        glTranslatef(0.2f, 0.0f, 0.0f);
        glScalef(0.4f, 0.02f, 0.1f);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawCat() {
    glPushMatrix();
    glTranslatef(catX, -0.5f + jumpY, 0.0f);

    glColor3f(1.0f, 0.7f, 0.2f);
    glutSolidCube(0.2);

    glPushMatrix();
    glTranslatef(0.15f, 0.1f, 0.0f);
    glutSolidCube(0.1);
    glPopMatrix();

    glPopMatrix();
}

void drawRoom() {
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex3f(-1, -0.7f, -1);
    glVertex3f(1, -0.7f, -1);
    glVertex3f(1, -0.7f, 1);
    glVertex3f(-1, -0.7f, 1);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(camAngleX, 1, 0, 0);
    glRotatef(camAngleY, 0, 1, 0);

    if (isNight) {
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    } else {
        glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
    }

    drawRoom();
    drawFan();
    drawCat();

    glutSwapBuffers();
}

void idle() {
    if (isFanOn) {
        fanAngle += 2.0f;
        if (fanAngle > 360) fanAngle -= 360;
    }

    if (isCatMoving) {
        catX += 0.002f;
        if (catX > 1.0f) catX = -1.0f;
    }

    if (isJumping) {
        velocityY -= 0.0008f;
        jumpY += velocityY;

        if (jumpY <= 0.0f) {
            jumpY = 0.0f;
            isJumping = false;
            velocityY = 0.0f;
        }
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'f':
        case 'F':
            isFanOn = !isFanOn;
            break;
        case 's':
        case 'S':
            isCatMoving = !isCatMoving;
            break;
        case 'j':
        case 'J':
            if (!isJumping) {
                isJumping = true;
                velocityY = 0.02f;
            }
            break;
        case 'l':
        case 'L':
            isNight = !isNight;
            break;
        case '+':
            zoom += 0.1f;
            break;
        case '-':
            zoom -= 0.1f;
            break;
        case 27:
            exit(0);
    }
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) camAngleY -= 5;
    if (key == GLUT_KEY_RIGHT) camAngleY += 5;
    if (key == GLUT_KEY_UP) camAngleX -= 5;
    if (key == GLUT_KEY_DOWN) camAngleX += 5;
}

void init() {
    glEnable(GL_DEPTH_TEST);
    initLighting();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Indoor Simulation");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}