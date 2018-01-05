/*
 *  aPanel.cpp
 *  Artemis
 *
 *  Created by Brian Kirkpatrick on 10/8/08.
 *  Copyright 2008 sdfTythos Creatives, Brian Kirkpatrick. All rights reserved.
 *
 */

#include "aPanel.h"
#include "aGraphics.h"

aPanel::aPanel() {
	x.value = 0.0f; y.value = 0.0f;
	w.value = 64.0f; h.value = 64.0f;
	visible = true;
	isTranslucent = false;
	next = NULL;
	tex = new aTexture();
	bgColor = new aColor(1.0f, 1.0f, 1.0f, 1.0f);
	children = NULL;
}

aPanel::aPanel(float nx, float ny, float nw, float nh) {
	x.value = nx; y.value = ny;
	w.value = nw; h.value = nh;
	visible = true;
	isTranslucent = false;
	next = NULL;
	tex = new aTexture();
	bgColor = new aColor(1.0f, 1.0f, 1.0f, 1.0f);
	children = NULL;
}

aPanel::~aPanel() {
/*	if (next != NULL) {
		delete next;
		next = NULL;
	}
	if (tex != NULL) {
		delete tex;
		tex = NULL;
	}
	if (children != NULL) {
		delete children;
		children = NULL;
	}*/
	if (bgColor != NULL) {
		delete bgColor;
		bgColor = NULL;
	}
}

void aPanel::update(float dt) {
	// Update panel (nothing for base)
}

void aPanel::render(aGraphics * context) {
	// Draw image to screen surface
	if (!visible) { return; }

	// Calculate location, position
	int screenWidth = context->getWidth();
	int screenHeight = context->getHeight();
	float centerX = x.unit == UNIT_PIX ? x.value / ((float)screenWidth) : x.value;
	float centerY = y.unit == UNIT_PIX ? y.value / ((float)screenHeight) : y.value;
	float left, right, bottom, top;
	switch (w.align) {
		case ALIGN_NEGATIVE:
			left = centerX;
			right = w.unit == UNIT_PIX ? centerX + w.value / (float)screenWidth : centerX + w.value;
			break;
		case ALIGN_POSITIVE:
			left = w.unit == UNIT_PIX ? centerX - w.value / (float)screenWidth : centerX - w.value;
			right = centerX;
			break;
		case ALIGN_MIDDLE:
		default:
			left = w.unit == UNIT_PIX ? centerX - 0.5f * w.value / (float)screenWidth : centerX - 0.5f * w.value;
			right = w.unit == UNIT_PIX ? centerX + 0.5f * w.value / (float)screenWidth : centerX + 0.5f * w.value;
			break;
	}
	switch (h.align) {
		case ALIGN_NEGATIVE:
			bottom = centerY;
			top = h.unit == UNIT_PIX ? centerY + h.value / (float)screenHeight : centerY + h.value;
			break;
		case ALIGN_POSITIVE:
			bottom = h.unit == UNIT_PIX ? centerY - h.value / (float)screenHeight : centerY - h.value;
			top = centerY;
			break;
		case ALIGN_MIDDLE:
		default:
			bottom = h.unit == UNIT_PIX ? centerY - 0.5f * h.value / (float)screenHeight : centerY - 0.5f * h.value;
			top = h.unit == UNIT_PIX ? centerY + 0.5f * h.value / (float)screenHeight : centerY + 0.5f * h.value;
			break;
	}

	// Draw 2d quad
	bgColor->setAll();

	if (tex != NULL && tex->isLoaded()) {
		// Draw textured panel
		tex->bind();
		glBegin(GL_QUADS); {
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(left, bottom);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(right, bottom);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(right, top);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(left, top);
		} glEnd();
		tex->unbind();
	} else {
		// Draw untextured panel
		glBegin(GL_QUADS); {
			glVertex2f(left, bottom);
			glVertex2f(right, bottom);
			glVertex2f(right, top);
			glVertex2f(left, top);
		} glEnd();
	}
}

void aPanel::setImage(kString filename) {
	// Loads panel image from file
	if (tex != NULL) {
		tex->loadFromFile(filename);
	}
}

void aPanel::setImage(SDL_Surface * surf) {
	// Copy surface to image using current display format
	if (tex != NULL) {
		tex->loadFromSurface(surf);
	}
}

aPanel * aPanel::get(int n) {
	if (n <= 0 || next == NULL) {
		return this;
	} else {
		return next->get(n-1);
	}
}

void aPanel::set(aPanel * value) {
	if (next == NULL) {
		next = value;
		value->next = NULL;
	} else {
		next->set(value);
	}
}

int aPanel::listLength() {
	if (next == NULL) {
		return 1;
	} else {
		return next->listLength() + 1;
	}
}

void aPanel::setX(float v) {
	this->x.value = v;
}

void aPanel::setY(float v) {
	this->y.value = v;
}

void aPanel::setW(float v) {
	this->w.value = v;
}

void aPanel::setH(float v) {
	this->h.value = v;
}

void aPanel::setVisible(bool v) {
	this->visible = v;
}

bool aPanel::isWithin(ScreenDimension xCoord, ScreenDimension yCoord, aGraphics * context) {
	// Returns true if the given screen coordinate is within the panel
	// First, convert to percent coords
	float panelX = x.toPct((float)(context->getWidth()));
	float panelY = y.toPct((float)(context->getHeight()));
	float panelWidth = w.toPct((float)(context->getWidth()));
	float panelHeight = h.toPct((float)(context->getHeight()));
	float panelLeft, panelRight, panelTop, panelBottom;
	switch (w.align) {
		case ALIGN_NEGATIVE:
			// Center point is on left of panel
			panelLeft = panelX;
			panelRight = panelX + panelWidth;
			break;
		case ALIGN_POSITIVE:
			// Center point is on right of panel
			panelLeft = panelX - panelWidth;
			panelRight = panelX;
			break;
		case ALIGN_MIDDLE:
		default:
			// Center point is in middle of panel
			panelLeft = panelX - panelWidth / 2.0f;
			panelRight = panelX + panelWidth / 2.0f;
	}
	switch (h.align) {
		case ALIGN_NEGATIVE:
			// Center point is on bottom of panel
			panelBottom = panelY;
			panelTop = panelY + panelHeight;
			break;
		case ALIGN_POSITIVE:
			// Center point is on top of panel
			panelBottom = panelY - panelHeight;
			panelTop = panelY;
			break;
		case ALIGN_MIDDLE:
		default:
			// Center point is in middle of panel
			panelBottom = panelY - panelHeight / 2.0f;
			panelTop = panelY + panelHeight / 2.0f;
	}

	// Then convert test coord to pct
	float testX = xCoord.toPct((float)(context->getWidth()));
	float testY = 1.0f - yCoord.toPct((float)(context->getHeight()));

	// Test!
	if (panelLeft < testX && testX < panelRight && panelBottom < testY && testY < panelTop) {
		return true;
	} else {
		return false;
	}
}

void aPanel::setBgColor(float r, float g, float b, float a) {
	bgColor->set(r, g, b, a);
}
