#include "stdafx.h"
#include "SpriteBatch.h"
#include <algorithm>

namespace Bengine {
	Glyph::Glyph(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint Texture, float Depth, const Color & color) : texture(Texture), depth(Depth) {
		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		botLeft.color = color;
		botLeft.setPosition(destRect.x, destRect.y);
		botLeft.setUV(uvRect.x, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		botRight.color = color;
		botRight.setPosition(destRect.x + destRect.z, destRect.y);
		botRight.setUV(uvRect.x + uvRect.z, uvRect.y);
	}

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0) {}


	SpriteBatch::~SpriteBatch() {}

	void SpriteBatch::init() {
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType) {
		_sortType = sortType;
		_renderBatches.clear();
		_glyphs.clear();
	}

	void SpriteBatch::end() {
		_glyphPointers.resize(_glyphs.size());
		for (int i = 0; i < _glyphs.size(); i++) {
			_glyphPointers[i] = &_glyphs[i];
		}
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const Color & color) {
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::renderBatch() {
		glBindVertexArray(_vao);
		for (int i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	//void SpriteBatch::renderBatch() {
	//	glBindVertexArray(_vao);
	//	for (int i = 0; i < _renderBatches.size(); i++) {
	//		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
	//		for (int j = 0; j < _renderBatches[i].numVertices/6 - 1; j++) {
	//			glDrawArrays(GL_TRIANGLES, j*6, (j+1)*6);
	//		}
	//	}
	//	glBindVertexArray(0);
	//}

	void SpriteBatch::createRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(_glyphPointers.size() * 6);
		if (_glyphPointers.empty()) {
			return;
		}

		int offset = 0;
		int cv = 0; //current vertex
		_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
		vertices[cv++] = _glyphPointers[0]->topLeft;
		vertices[cv++] = _glyphPointers[0]->botLeft;
		vertices[cv++] = _glyphPointers[0]->botRight;
		vertices[cv++] = _glyphPointers[0]->botRight;
		vertices[cv++] = _glyphPointers[0]->topRight;
		vertices[cv++] = _glyphPointers[0]->topLeft;
		offset += 6;

		int cg = 1; //current glyph
		for (int cg = 1; cg < _glyphPointers.size(); cg++) {
			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
			} else {
				_renderBatches.back().numVertices += 6;
			}
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			vertices[cv++] = _glyphPointers[cg]->botLeft;
			vertices[cv++] = _glyphPointers[cg]->botRight;
			vertices[cv++] = _glyphPointers[cg]->botRight;
			vertices[cv++] = _glyphPointers[cg]->topRight;
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray() {
		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);
		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs() {
		switch (_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph * a, Glyph * b) {
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph * a, Glyph * b) {
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph * a, Glyph * b) {
		return (a->texture < b->texture);
	}
}