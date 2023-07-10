#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 
#include <algorithm>

// Variables de posición y rotación del personaje
float posX = 0.0f;
float posZ = 0.0f;
float rotY = 0.0f;

//float lookAngle = 0.0f;   // Ángulo de elevación de la mirada
//float lookStep = 5.0f;    // Incremento de ángulo para la elevación/bajada de la mirada



// Tamaño del laberinto
const int mazeSize = 10;
const float mazeScale = 2.0f;

// Estructura de datos para el laberinto
int maze[mazeSize][mazeSize] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {0, 0, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {0, 0, 0, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1}
};

// Textura de la pared
unsigned int wallTexture;

// Función para cargar la textura de la pared
void loadWallTexture() {
    int width, height, channels;
    unsigned char* image = stbi_load("D:/pared.jpg", &width, &height, &channels, 0);
    if (image) {
        glGenTextures(1, &wallTexture);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
}

// Función para dibujar el laberinto
void drawMaze() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wallTexture);

    for (int i = 0; i < mazeSize; i++) {
        for (int j = 0; j < mazeSize; j++) {
            if (maze[i][j] == 1) {
                glPushMatrix();
                glTranslatef((float)j * mazeScale, 0.0f, (float)i * mazeScale);
                glScalef(mazeScale, mazeScale, mazeScale);

                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(-0.5f, -0.5f, 0.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(0.5f, -0.5f, 0.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(0.5f, 0.5f, 0.0f);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(-0.5f, 0.5f, 0.0f);
                glEnd();

                glPopMatrix();
            }
        }
    }

    glDisable(GL_TEXTURE_2D);
}

// Función para dibujar el piso
void drawFloor() {
    glColor3f(0.5f, 0.5f, 1.0f);  // Color azul del piso

    glBegin(GL_QUADS);
    glVertex3f(-mazeScale * mazeSize * 0.5f, -0.5f, -mazeScale * mazeSize * 0.5f);
    glVertex3f(mazeScale * mazeSize * 0.5f, -0.5f, -mazeScale * mazeSize * 0.5f);
    glVertex3f(mazeScale * mazeSize * 0.5f, -0.5f, mazeScale * mazeSize * 0.5f);
    glVertex3f(-mazeScale * mazeSize * 0.5f, -0.5f, mazeScale * mazeSize * 0.5f);
    glEnd();
}

// Función de dibujado
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Aplicar transformaciones de la cámara
    gluLookAt(posX, 1.0f, posZ, posX + sin(rotY), 1.0f, posZ - cos(rotY), 0.0f, 1.0f, 0.0f);
    //gluLookAt(posX, -7.0f, posZ, posX + sin(rotY), -6.0f, posZ - cos(rotY), 0.0f, 1.0f, 0.0f);
    //gluLookAt(posX, posY + sin(lookAngle), posZ, posX + sin(rotY), posY - sin(lookAngle), posZ - cos(rotY), 0.0f, 1.0f, 0.0f);


    // Dibujar el piso
    drawFloor();

    // Dibujar el laberinto
    drawMaze();

    glutSwapBuffers();
}

// Función de manejo de teclado
void handleKeypress(unsigned char key, int x, int y) {
    float moveStep = 0.1f;
    float rotateStep = 0.1f;

    switch (key) {
    case 'w':
        // Avanzar hacia adelante
        posX += moveStep * sin(rotY);
        posZ -= moveStep * cos(rotY);
        break;
    case 's':
        // Retroceder
        posX -= moveStep * sin(rotY);
        posZ += moveStep * cos(rotY);
        break;
    case 'a':
        // Girar a la izquierda
        rotY += rotateStep;
        break;
    case 'd':
        // Girar a la derecha
        rotY -= rotateStep;
        break;

    }
    

    glutPostRedisplay();
}

// Función de redimensionamiento de la ventana
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    //gluPerspective(-45.0, (double)w / (double)h, 1.0, 200.0);
}

// Función principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Laberinto 3D");

    glEnable(GL_DEPTH_TEST);

    // Cargar la textura de la pared
    loadWallTexture();

    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);

    glutMainLoop();

    return 0;
}
