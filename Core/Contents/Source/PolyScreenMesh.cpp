/*
 Copyright (C) 2011 by Ivan Safrin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#include "PolyScreenMesh.h"
#include "PolyCoreServices.h"
#include "PolyMaterialManager.h"
#include "PolyMesh.h"
#include "PolyRenderer.h"

using namespace Polycode;

ScreenMesh::ScreenMesh(Mesh *mesh) : ScreenEntity(), texture(NULL) {
	this->mesh = mesh;
	lineSmooth = false;
	lineWidth = 1.0;
	ownsMesh = true;
	updateHitBox();
}

ScreenMesh::ScreenMesh(const String& fileName) : ScreenEntity(), texture(NULL) {
	mesh = new Mesh(fileName);
	lineSmooth = false;
	lineWidth = 1.0;
	
}

ScreenMesh::ScreenMesh(int meshType) : ScreenEntity(), texture(NULL) {
	mesh = new Mesh(meshType);
	lineSmooth = false;
	lineWidth = 1.0;
	
}

ScreenMesh *ScreenMesh::ScreenMeshWithMesh(Mesh *mesh) {
	return new ScreenMesh(mesh);
}

ScreenMesh *ScreenMesh::ScreenMeshWithType(int meshType) {
	return new ScreenMesh(meshType);
}

ScreenMesh::~ScreenMesh() {
	if(ownsMesh) {
		delete mesh;
	}
}

Mesh *ScreenMesh::getMesh() const {
	return mesh;
}

Texture *ScreenMesh::getTexture() const {
	return texture;
}

void ScreenMesh::setTexture(Texture *texture) {
	this->texture = texture;
}

void ScreenMesh::loadTexture(const String& fileName) {
	texture = CoreServices::getInstance()->getMaterialManager()->createTextureFromFile(fileName, false, true);
}

void ScreenMesh::loadTexture(Image *image) {
	texture = CoreServices::getInstance()->getMaterialManager()->createTextureFromImage(image, false, true);
}

void ScreenMesh::Render() {	
	Renderer *renderer = CoreServices::getInstance()->getRenderer();
	
	renderer->setLineSize(lineWidth);
	renderer->setLineSmooth(lineSmooth);
	
	renderer->setTexture(texture);
	if(mesh->useVertexColors) {
		renderer->pushDataArrayForMesh(mesh, RenderDataArray::COLOR_DATA_ARRAY);
	}
	renderer->pushDataArrayForMesh(mesh, RenderDataArray::VERTEX_DATA_ARRAY);
	renderer->pushDataArrayForMesh(mesh, RenderDataArray::TEXCOORD_DATA_ARRAY);	
	renderer->drawArrays(mesh->getMeshType());
}

void ScreenMesh::updateHitBox() {
	Number xmin, ymin, xmax, ymax;
	bool any = false;
	for(int c = 0; c < mesh->getPolygonCount(); c++) {
		Polygon *poly = mesh->getPolygon(c);
		for(int d = 0; d < poly->getVertexCount(); d++) {
			Vertex *v = poly->getVertex(d);
			if (any) {
				xmin = MIN(v->x, xmin);
				ymin = MIN(v->y, ymin);
				xmax = MAX(v->x, xmax);
				ymax = MAX(v->y, ymax);
			} else {
				xmin = v->x; xmax = v->x;
				ymin = v->y; ymax = v->y;
				any = true;
			}
		}
	}
	
	setHitbox(xmax-xmin, ymax-ymin, xmin, ymin);
}
