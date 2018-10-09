#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Bengine {
	class Camera2D {
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);

		void update();

		void setPosition(glm::vec2 position) { _position = position; _needsMatrixUpdate = true; }
		void setScale(float scale) { _scale = scale; _needsMatrixUpdate = true; }

		glm::vec2 getPosition() { return _position; }
		float getScale() { return _scale; }
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }
		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

	private:
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
		float _scale;
		bool _needsMatrixUpdate;
		int _screenWidth, _screenHeight;
	};
}