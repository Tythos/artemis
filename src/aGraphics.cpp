/*
 *  aGraphics.cpp
 *  Artemis
 *
 *  Created by Brian Kirkpatrick on 10/7/08.
 *  Copyright 2008 Tythos Creatives, Brian Kirkpatrick. All rights reserved.
 *
 */

#include "aGraphics.h"
#include "SDL_opengl.h"
#include "kQuat.h"

aGraphics::aGraphics() {
	// Set up initial graphics dimensions
	windowOk = false;
	screen = NULL;
	hTypewriter = new aTypewriter();
}

aGraphics::~aGraphics() {
	if (screen != NULL) {
		bool t = false;
		SDL_FreeSurface(screen);
		screen = NULL;
		t = false;
	}
}

bool aGraphics::setScreen(int newHeight, int newWidth, int newBpp) {
	// Assign new screen attributes if valid
	width = newWidth;
	height = newHeight;
	bpp = newBpp;
	
	int colorBits = int(float(bpp) / 4.0f);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, colorBits);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, colorBits);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, colorBits);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, bpp - 3 * colorBits);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Initialize screen context
	if (screen != NULL) {
		SDL_FreeSurface(screen);
		screen = NULL;
	}
	screen = SDL_SetVideoMode(width, height, bpp, SDL_WINDOW_OPENGL | SDL_SWSURFACE | SDL_WINDOW_RESIZABLE);
	if (screen == NULL) {
		windowOk = false;
		return windowOk;
	} else {
		windowOk = true;
	}
	SDL_WM_SetCaption("Sdl Window", NULL);
	isWindowed = false;
	bgColor.r = 255; bgColor.g = 255; bgColor.b = 255;
	//screenLock = SDL_CreateSemaphore(1);
	if (!declareSettings()) {
		windowOk = false;
	}
	return windowOk;
}

bool aGraphics::declareSettings() {
	// Gl settings
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glDepthFunc(GL_LEQUAL);
	return true;
}

void aGraphics::printAttributes() {
    // Print out attributes of the context we created
    int nAttr;
    int i;    
    int attr[] = { SDL_GL_RED_SIZE, SDL_GL_BLUE_SIZE, SDL_GL_GREEN_SIZE,
	SDL_GL_ALPHA_SIZE, SDL_GL_BUFFER_SIZE, SDL_GL_DEPTH_SIZE };
	
    char *desc[] = { "Red size: %d bits\n", "Blue size: %d bits\n", "Green size: %d bits\n", "Alpha size: %d bits\n", "Color buffer size: %d bits\n", "Depth bufer size: %d bits\n" };
    nAttr = sizeof(attr) / sizeof(int);
    
    for (i = 0; i < nAttr; i++) {
        int value;
		SDL_GLattr myAttr = SDL_GLattr(attr[i]);
        SDL_GL_GetAttribute (myAttr, &value);
        printf (desc[i], value);
    }
}

void aGraphics::resize(int w, int h) {
	// Release and refresh properties
	if (screen == NULL) { SDL_FreeSurface(screen); }
	screen = SDL_SetVideoMode(w, h, bpp, SDL_OPENGL | SDL_SWSURFACE | SDL_RESIZABLE);
	if (screen == NULL) {
		windowOk = false;
		return;
	}
	height = h;
	width = w;
	declareSettings();
}

bool aGraphics::toggleFullscreen() {
	if (isWindowed) {
		// Reset screen surface
		screen = SDL_SetVideoMode(width, height, bpp, SDL_OPENGL | SDL_SWSURFACE | SDL_RESIZABLE | SDL_FULLSCREEN);
		if (screen == NULL) {
			windowOk = false;
		} else {
			windowOk = true;
			isWindowed = false;
			declareSettings();
		}
	} else {
		// Reset screen surface
		screen = SDL_SetVideoMode(width, height, bpp, SDL_OPENGL | SDL_SWSURFACE | SDL_RESIZABLE);
		if (screen == NULL) {
			windowOk = false;
		} else {
			windowOk = true;
			isWindowed = true;
			declareSettings();
		}
	}
	return windowOk;
}

void aGraphics::go2d() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
}

void aGraphics::go3d() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 12.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void aGraphics::go3d(aCamera * cam) {
	kQuat * pos = cam->getPosition();
	kQuat * trg = cam->getTarget();
	kQuat * upq = cam->getUp();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(cam->getFieldAngle(), cam->getAspectRatio(), cam->getNearClip(), cam->getFarClip());
	gluLookAt(pos->getVecI(), pos->getVecJ(), pos->getVecK(), trg->getVecI(), trg->getVecJ(), trg->getVecK(), upq->getVecI(), upq->getVecJ(), upq->getVecK());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void aGraphics::swapBuffers() {
	// Show current buffer on screen
	SDL_GL_SwapBuffers();
}

void aGraphics::clearScreen() {
	// Fills screen surface with background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, bgColor.r, bgColor.g, bgColor.b));
}

void aGraphics::testLoop() {
	// Executes a test drawing loop that renders various colors and primitives
	glClear(GL_COLOR_BUFFER_BIT);
	static float x = 0.0f, y = 0.0f;
	static float angle = 0.0f;
	angle += 0.01f;
	if (angle >= 360.0f)
		angle = 0.0f;
	glLoadIdentity();
	glTranslatef(-x, -y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glEnable(GL_POINT_SMOOTH);
	
	glBegin(GL_TRIANGLES); {
		glColor3f(0.1f, 0.3f, 0.5f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glColor3f(0.3f, 0.5f, 0.1f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.5f, 0.1f, 0.3f);
		glVertex3f(0.0f, 0.0f, 1.0f);
	} glEnd();
	
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0f);
	glBegin(GL_LINES); {
		glColor3f(0.2f, 0.4f, 0.2f);
		glVertex3f(-1.0f, -0.5f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 0.3f, 0.0f);
	} glEnd();
	
	glFlush();
}

void aGraphics::drawCube(float xPos, float yPos, float zPos) {
	glPushMatrix(); {
		glTranslatef(xPos, yPos, zPos);
		glBegin(GL_QUADS); { // Top
			glNormal3f(0.0f, 1.0f, 0.0f);
			glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
			glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
			glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
			glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
		} glEnd();
		glBegin(GL_QUADS); { // Front
			glNormal3f(0.0f, 0.0f, 1.0f);
			glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
			glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
			glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
			glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
		} glEnd();
		glBegin(GL_QUADS); { // Right
			glNormal3f(1.0f, 0.0f, 0.0f);
			glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
			glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
			glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
		} glEnd();
		glBegin(GL_QUADS); { // Left
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
			glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
			glColor3f(0.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
			glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
		} glEnd();
		glBegin(GL_QUADS); { // Bottom
			glNormal3f(0.0f, -1.0f, 0.0f);
			glColor3f(0.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
			glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
			glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
		} glEnd();
		glBegin(GL_QUADS); { // Back
			glNormal3f(0.0f, 0.0f, -1.0f);
			glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
			glColor3f(0.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
			glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
		} glEnd();
	} glPopMatrix();
}

void aGraphics::drawTexturedCube(float xPos, float yPos, float zPos) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix(); {
		glTranslatef(xPos, yPos, zPos);
		
		glBegin(GL_QUADS); { // Top face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
		} glEnd();
		
		glBegin(GL_QUADS); { // Front face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
		} glEnd();
		
		glBegin(GL_QUADS); { // Right face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
		} glEnd();
		
		glBegin(GL_QUADS); { // Left face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
		} glEnd();
		
		glBegin(GL_QUADS); { // Bottom face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
		} glEnd();
		
		glBegin(GL_QUADS); { // Back face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
		} glEnd();
	} glPopMatrix();	
}