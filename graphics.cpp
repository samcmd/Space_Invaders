//
// Created by sschu on 4/19/2021.
// Space Invaders C++ graphics program
// Control a ship and shoot down the enemies that approach
// Orange barriers prevent you from shooting from certain spots
// If the enemies passes your barriers you will die
//

#include "circle.h"
#include "graphics.h"
#include "rect.h"
#include "time.h"
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

GLdouble width, height;
int wd;
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grassGreen(26/255.0, 176/255.0, 56/255.0);
const color white(1, 1, 1);
const color brickRed(201/255.0, 20/255.0, 20/255.0);
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color black(0, 0, 0);
const color magenta(1, 0, 1);
const color orange(1, 163/255.0, 22/255.0);
const color cyan(0, 1, 1);

vector<Rect> barrier;
vector<Circle> stars;
vector<unique_ptr<Shape>> enemies1;
vector<unique_ptr<Shape>> enemies2;
Rect bullet;
vector<unique_ptr<Shape>> user;
Rect deathBox;
int userX = 250;
int userY = 450;
int USERYMAX = 420;

void initBullet() {
    bullet.setCenter(0, 0);
    bullet.setSize(3, 20);
    bullet.setColor(black);
}

void initStars() {
    for (int i = 0; i < 100; ++i) {
        stars.push_back(Circle(1, 1, 1, 1, rand() % int(width), (rand() % int(height)), rand() % 5 - 2));
        stars[i].setColor(white);
    }
}

void initBarrier() {
    int totalBarrierWidth = 0;
    dimensions barrierSize;

    // Populate barriers
    totalBarrierWidth = 55;
    while (totalBarrierWidth < width - 100) {
        // Populate this vector of barriers
        // Barrier height 40
        barrierSize.height = 40;
        // Barrier width 60
        barrierSize.width = 60;
        barrier.push_back(Rect(purple,totalBarrierWidth+(barrierSize.width/2),height-((barrierSize.height/2)+100),barrierSize));
        totalBarrierWidth += barrierSize.width + 50;
    }
}

void initEnemies() {
    enemies1.clear();
    enemies2.clear();
    dimensions enemiesArm(15, 30);
    dimensions enemies1Bottom(30, 20);
    dimensions enemies2Bottom(15, 20);
    // First enemy
    enemies1.push_back(make_unique<Rect>(grassGreen, 280, 120, enemiesArm));
    enemies1.push_back(make_unique<Rect>(grassGreen, 310, 120, enemiesArm));
    enemies1.push_back(make_unique<Rect>(grassGreen, 295, 90, enemies2Bottom));
    enemies1.push_back(make_unique<Rect>(grassGreen, 295, 105, enemies1Bottom));
    // Second enemy
    enemies1.push_back(make_unique<Rect>(grassGreen, 20, 120, enemiesArm));
    enemies1.push_back(make_unique<Rect>(grassGreen, 50, 120, enemiesArm));
    enemies1.push_back(make_unique<Rect>(grassGreen, 35, 90, enemies2Bottom));
    enemies1.push_back(make_unique<Rect>(grassGreen, 35, 105, enemies1Bottom));
    // Third enemy
    enemies1.push_back(make_unique<Rect>(grassGreen, 400, 120, enemiesArm));
    enemies1.push_back(make_unique<Rect>(grassGreen, 430, 120, enemiesArm));
    enemies1.push_back(make_unique<Rect>(grassGreen, 415, 90, enemies2Bottom));
    enemies1.push_back(make_unique<Rect>(grassGreen, 415, 105, enemies1Bottom));
    // Fourth enemy
    enemies1.push_back(make_unique<Rect>(grassGreen, 150, 120, enemiesArm));
    enemies1.push_back(make_unique<Rect>(grassGreen, 180, 120, enemiesArm));
    enemies1.push_back(make_unique<Rect>(grassGreen, 165, 90, enemies2Bottom));
    enemies1.push_back(make_unique<Rect>(grassGreen, 165, 105, enemies1Bottom));

    // First enemy 2nd row
    enemies2.push_back(make_unique<Rect>(grassGreen, 340, 60, enemiesArm));
    enemies2.push_back(make_unique<Rect>(grassGreen, 370, 60, enemiesArm));
    enemies2.push_back(make_unique<Rect>(grassGreen, 355, 30, enemies2Bottom));
    enemies2.push_back(make_unique<Rect>(grassGreen, 355, 45, enemies1Bottom));
    // Second enemy 2nd row
    enemies2.push_back(make_unique<Rect>(grassGreen, 80, 60, enemiesArm));
    enemies2.push_back(make_unique<Rect>(grassGreen, 110, 60, enemiesArm));
    enemies2.push_back(make_unique<Rect>(grassGreen, 95, 30, enemies2Bottom));
    enemies2.push_back(make_unique<Rect>(grassGreen, 95, 45, enemies1Bottom));
    // Third enemy 2nd row
    enemies2.push_back(make_unique<Rect>(grassGreen, 460, 60, enemiesArm));
    enemies2.push_back(make_unique<Rect>(grassGreen, 490, 60, enemiesArm));
    enemies2.push_back(make_unique<Rect>(grassGreen, 475, 30, enemies2Bottom));
    enemies2.push_back(make_unique<Rect>(grassGreen, 475, 45, enemies1Bottom));
    // Fourth enemy 2nd row
    enemies1.push_back(make_unique<Rect>(grassGreen, 210, 60, enemiesArm));
    enemies1.push_back(make_unique<Rect>(grassGreen, 240, 60, enemiesArm));
    enemies1.push_back(make_unique<Rect>(grassGreen, 225, 30, enemies2Bottom));
    enemies1.push_back(make_unique<Rect>(grassGreen, 225, 45, enemies1Bottom));
}

void initUser() {
    // Initialize the user to be a brick red space ship
    // centered in the middle bottom of the graphics window
    user.clear();
    dimensions body(7,35);
    dimensions connector(20,5);
    dimensions side(5,22);
    user.push_back(make_unique<Rect>(brickRed, userX, userY, body));
    user.push_back(make_unique<Rect>(brickRed, userX, userY+8, connector));
    user.push_back(make_unique<Rect>(brickRed, userX-10, userY+5, side));
    user.push_back(make_unique<Rect>(brickRed, userX+10, userY+5, side));
}

// Kills the player if enemy reaches this box
void initDeathBox() {
    deathBox.setCenter(250,380);
    deathBox.setSize(500, 2);
    deathBox.setColor(black);
}

void init() {
    width = 500;
    height = 500;
    srand(time(0));
    initBullet();
    initStars();
    initBarrier();
    initEnemies();
    initUser();
    initDeathBox();
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(black.red, black.green, black.blue, 1.0f);
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

    /*
     * Draw here
     */

    deathBox.draw();

    for (Circle &star : stars) {
        star.draw();
    }

    for (Rect &r : barrier) {
        r.setColor(orange);
        r.draw();
    }

    for (unique_ptr<Shape> &s : enemies1) {
        // #polymorphism
        s->draw();
    }

    for (unique_ptr<Shape> &s : enemies2) {
        // #polymorphism
        s->draw();
    }

    // Check if the bullet is overlapping with the barriers
    for (int i = 0; i < barrier.size(); i++) {
        if (barrier[i].isOverlapping(bullet)){
            bullet.setColor(black);
            bullet.setCenter(0,0);
        }
    }

    // Check if the user is overlapping with the enemies
    // Only check the center Rect object within the enemy
    for (int i = 3; i < enemies1.size(); i += 4) {
        if (dynamic_cast<Rect&>(*enemies1[i]).isOverlapping(deathBox)){
            glutDestroyWindow(wd);
            exit(0);
        }
    }

    // Check if the user is overlapping with the enemies
    // Only check the center Rect object within the enemy
    for (int i = 3; i < enemies2.size(); i += 4) {
        if (dynamic_cast<Rect&>(*enemies2[i]).isOverlapping(deathBox)){
            glutDestroyWindow(wd);
            exit(0);
        }
    }

    // Check if the bullet is overlapping with the enemies
    // Only check the center Rect object within the enemy
    for (int i = 3; i < enemies1.size(); i += 4) {
        if (dynamic_cast<Rect&>(*enemies1[i]).isOverlapping(bullet)){
            dynamic_cast<Rect&>(*enemies1[i]).setColor(black);
            dynamic_cast<Rect&>(*enemies1[i]).setCenter(550,550);
            dynamic_cast<Rect&>(*enemies1[i-1]).setColor(black);
            dynamic_cast<Rect&>(*enemies1[i-1]).setCenter(550,550);
            dynamic_cast<Rect&>(*enemies1[i-2]).setColor(black);
            dynamic_cast<Rect&>(*enemies1[i-2]).setCenter(550,550);
            dynamic_cast<Rect&>(*enemies1[i-3]).setColor(black);
            dynamic_cast<Rect&>(*enemies1[i-3]).setCenter(550,550);
        }
    }

    // Check if the bullet is overlapping with the enemies
    // Only check the center Rect object within the enemy
    for (int i = 3; i < enemies2.size(); i += 4) {
        if (dynamic_cast<Rect&>(*enemies2[i]).isOverlapping(bullet)){
            dynamic_cast<Rect&>(*enemies2[i]).setColor(black);
            dynamic_cast<Rect&>(*enemies2[i]).setCenter(550,550);
            dynamic_cast<Rect&>(*enemies2[i-1]).setColor(black);
            dynamic_cast<Rect&>(*enemies2[i-1]).setCenter(550,550);
            dynamic_cast<Rect&>(*enemies2[i-2]).setColor(black);
            dynamic_cast<Rect&>(*enemies2[i-2]).setCenter(550,550);
            dynamic_cast<Rect&>(*enemies2[i-3]).setColor(black);
            dynamic_cast<Rect&>(*enemies2[i-3]).setCenter(550,550);
        }
    }

    bullet.draw();

    for (unique_ptr<Shape> &s : user) {
        // #polymorphism
        s->draw();
    }

    int enemiesKilled = 0;

    for (int i = 3; i < enemies1.size(); i += 4) {
        if (dynamic_cast<Rect&>(*enemies1[i]).getGreen() == 0)
            enemiesKilled++;
    }

    for (int i = 3; i < enemies2.size(); i += 4) {
        if (dynamic_cast<Rect&>(*enemies2[i]).getGreen() == 0)
            enemiesKilled++;
    }

    if (enemiesKilled >= 8) {
        for (int i = 0; i < barrier.size(); i++) {
            barrier[i].setCenter(550,550);
            USERYMAX = 30;
        }
    }

    glFlush();  // Render now
}

void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
    if (key == 32) {
        if (USERYMAX >= userY)
            bullet.setCenter(userX, USERYMAX-20);
        else
            bullet.setCenter(userX, userY-20);
    }
    bullet.setColor(cyan);
    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            userY += 5;
            dynamic_cast<Rect&>(*user[0]).setCenter(userX, userY);
            dynamic_cast<Rect&>(*user[1]).setCenter(userX, userY+8);
            dynamic_cast<Rect&>(*user[2]).setCenter(userX-10, userY+5);
            dynamic_cast<Rect&>(*user[3]).setCenter(userX+10, userY+5);
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            userX -= 5;
            dynamic_cast<Rect&>(*user[0]).setCenter(userX, userY);
            dynamic_cast<Rect&>(*user[1]).setCenter(userX, userY+8);
            dynamic_cast<Rect&>(*user[2]).setCenter(userX-10, userY+5);
            dynamic_cast<Rect&>(*user[3]).setCenter(userX+10, userY+5);
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            userX += 5;
            dynamic_cast<Rect&>(*user[0]).setCenter(userX, userY);
            dynamic_cast<Rect&>(*user[1]).setCenter(userX, userY+8);
            dynamic_cast<Rect&>(*user[2]).setCenter(userX-10, userY+5);
            dynamic_cast<Rect&>(*user[3]).setCenter(userX+10, userY+5);
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            if (USERYMAX >= userY) {
                dynamic_cast<Rect&>(*user[0]).setCenter(userX, userY);
                dynamic_cast<Rect&>(*user[1]).setCenter(userX, userY+8);
                dynamic_cast<Rect&>(*user[2]).setCenter(userX-10, userY+5);
                dynamic_cast<Rect&>(*user[3]).setCenter(userX+10, userY+5);
            }
            else {
                userY -= 5;
                dynamic_cast<Rect&>(*user[0]).setCenter(userX, userY);
                dynamic_cast<Rect&>(*user[1]).setCenter(userX, userY+8);
                dynamic_cast<Rect&>(*user[2]).setCenter(userX-10, userY+5);
                dynamic_cast<Rect&>(*user[3]).setCenter(userX+10, userY+5);
            }
            glutPostRedisplay();
            break;
    }

    glutPostRedisplay();
}

void cursor(int x, int y) {
    // Set the user's center point to be the coordinates
    // passed in as parameters to this function. This will make
    // the user ship move with the mouse.
    if (USERYMAX >= y) {
        dynamic_cast<Rect&>(*user[0]).setCenter(x, userY);
        dynamic_cast<Rect&>(*user[1]).setCenter(x, userY+8);
        dynamic_cast<Rect&>(*user[2]).setCenter(x-10, userY+5);
        dynamic_cast<Rect&>(*user[3]).setCenter(x+10, userY+5);
        userX = x;
    }
    else {
        dynamic_cast<Rect&>(*user[0]).setCenter(x, y);
        dynamic_cast<Rect&>(*user[1]).setCenter(x, y+8);
        dynamic_cast<Rect&>(*user[2]).setCenter(x-10, y+5);
        dynamic_cast<Rect&>(*user[3]).setCenter(x+10, y+5);
        userX = x;
        userY = y;
    }

    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (USERYMAX >= y)
            bullet.setCenter(x, USERYMAX-20);
        else
            bullet.setCenter(x, y-20);
    }
    bullet.setColor(cyan);
    glutPostRedisplay();
}

void enemyTimer(int dummy) {
    // Make the two vectors of enemies move.

    for (unique_ptr<Shape> &s : enemies1) {
        // Move all the enemies to the left and down
        s->moveX(-1);
        s->moveY(+.3);
        // If a shape has moved off the screen
        if (s->getCenterX() < -20) {
            // Set it to the right of the screen so that it passes through again
            s->setCenterX(520);
        }
    }

    for (unique_ptr<Shape> &s : enemies2) {
        // Move all the enemies to the left and down
        s->moveX(-1);
        s->moveY(+.3);
        // If a shape has moved off the screen
        if (s->getCenterX() < -20) {
            // Set it to the right of the screen so that it passes through again
            s->setCenterX(520);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, enemyTimer, dummy);
}

void bulletTimer(int dummy) {
    // Make the user's attack move.

    bullet.moveY(-10);

    glutPostRedisplay();
    glutTimerFunc(30, bulletTimer, dummy);
}

void starTimer(int dummy) {
    // Make the background stars move.

    for (Circle &star : stars) {
        star.moveY(star.getRadius());
        if (star.getTopY() > height) {
            star.setCenter(rand() % int(width), -star.getRadius());
        }
    }

    glutPostRedisplay();
    glutTimerFunc(7, starTimer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Space Invaders" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, enemyTimer, 0);
    glutTimerFunc(0, bulletTimer, 0);
    glutTimerFunc(0, starTimer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}