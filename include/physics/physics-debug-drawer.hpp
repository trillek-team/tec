#pragma once

#include <LinearMath/btIDebugDraw.h>
#include <vector>
#include <memory>
#include "resources/mesh.hpp"

namespace tec {
	class VertexBufferObject;

	class PhysicsDebugDrawer : public btIDebugDraw {
	public:
		PhysicsDebugDrawer();

		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

		virtual void drawAABB(const btVector3& from, const btVector3& to, const btVector3& color);

		virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

		virtual void reportErrorWarning(const char* warningString);

		virtual void draw3dText(const btVector3& location, const char* textString);

		virtual void setDebugMode(int debugMode);

		virtual int getDebugMode() const { return m_debugMode; }

		void UpdateVertexBuffer();
	private:
		int m_debugMode;
		std::shared_ptr<VertexBufferObject> vert_buffer;
		std::vector<VertexData> verts;
		std::vector<unsigned int> indices;
	};
}
