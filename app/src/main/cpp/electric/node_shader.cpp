#include "node_shader.h"
#include "vertex.h"

namespace Electric {

#define VIEW_PROJECTION "viewProjection"
#define CAMERA_POS "up"
#define RIGHT "right"

	bool NodeShader::init(AAssetManager* pAssetManager, const Mesh& mesh)
	{

		m_program = createProgram(pAssetManager, "shaders/NodeShader_vs.glsl", "shaders/NodeShader_ps.glsl");
		m_viewProjectionLocation = getLocation(m_program, VIEW_PROJECTION);
		m_upLocation = getLocation(m_program, CAMERA_POS);
		m_rightLocation = getLocation(m_program, RIGHT);

		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
		glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionVertex), 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.getDynamicBuffer());

		glEnableVertexAttribArray(1); //offset
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(NodeInstance), 0);
		glVertexAttribDivisor(1, 1);

		glEnableVertexAttribArray(2); // size
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(NodeInstance), BUFFER_OFFSET(3));
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3); // charge
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(NodeInstance), BUFFER_OFFSET(4));
		glVertexAttribDivisor(3, 1);

		glEnableVertexAttribArray(4); // color
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(NodeInstance), BUFFER_OFFSET(5));
		glVertexAttribDivisor(4, 1);

		glBindVertexArray(0);

		return true;
	}
}
