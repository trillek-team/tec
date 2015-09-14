#include "physics/physics-debug-drawer.hpp"
#include "graphics/vertex-buffer-object.hpp"
#include "graphics/material.hpp"
#include "graphics/shader.hpp"
#include "components/renderable.hpp"
#include "entity.hpp"
#include "cstdio"

namespace tec {
	PhysicsDebugDrawer::PhysicsDebugDrawer() : m_debugMode(0) {

	}

	void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		VertexData from_vert(from.getX(), from.getY(), from.getZ(), color.getX(), color.getY(), color.getZ());
		this->verts.push_back(from_vert);
		VertexData to_vert(to.getX(), to.getY(), to.getZ(), color.getX(), color.getY(), color.getZ());
		this->verts.push_back(to_vert);
		this->indices.push_back(this->indices.size());
		this->indices.push_back(this->indices.size());
	}

	void PhysicsDebugDrawer::drawAABB(const btVector3& from, const btVector3& to, const btVector3& color) {
		VertexData from_vert(from.getX(), from.getY(), from.getZ(), color.getX(), color.getY(), color.getZ());
		this->verts.push_back(from_vert);
		VertexData to_vert(to.getX(), to.getY(), to.getZ(), color.getX(), color.getY(), color.getZ());
		this->verts.push_back(to_vert);
		this->indices.push_back(this->indices.size());
		this->indices.push_back(this->indices.size());
	}

	void PhysicsDebugDrawer::setDebugMode(int debugMode) {
		m_debugMode = debugMode;
	}

	void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString) {
		//glRasterPos3f(location.x(),  location.y(),  location.z());
		//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
	}

	void PhysicsDebugDrawer::reportErrorWarning(const char* warningString) {
		std::printf("%s", warningString);
	}

	void PhysicsDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
		{
			//btVector3 to=pointOnB+normalOnB*distance;
			//const btVector3&from = pointOnB;
			//glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   

			//GLDebugDrawer::drawLine(from, to, color);

			//glRasterPos3f(from.x(),  from.y(),  from.z());
			//char buf[12];
			//sprintf(buf," %d",lifeTime);
			//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
		}
	}
	void PhysicsDebugDrawer::UpdateVertexBuffer() {
		if (!this->vert_buffer) {
			std::shared_ptr<Renderable> ren = std::make_shared<Renderable>(std::make_shared<VertexBufferObject>());
			this->vert_buffer = ren->buffer;
			this->vert_buffer->Load(verts, indices);
			this->verts.clear();
			this->indices.clear();
			this->vert_buffer->GetVertexGroup(0)->material = MaterialMap::Get("material_debug"); // HACK: This should be configurable.
			ren->shader = ShaderMap::Get("debug");
			ren->vertex_groups.insert(vert_buffer->GetVertexGroup(0));
			Entity(0).Add<Renderable>(ren); // HACK: This should be configurable.
			return;
		}
		this->vert_buffer->Load(verts, indices);
		this->verts.clear();
		this->indices.clear();
	}
}
