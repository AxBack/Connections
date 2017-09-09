#include "item_shader.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

bool ItemShader::init(AAssetManager* pAssetManager, const IMesh& mesh)
{
	std::string file;
	if(!loadFile(pAssetManager, "shaders/ItemShader.vs", file))
		return false;

	const char* pFile = file.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &pFile, nullptr);
	glCompileShader(vs);
	if(!verifyShader(vs))
		return false;

	if(!loadFile(pAssetManager, "shaders/ItemShader.ps", file))
		return false;

	pFile = file.c_str();

	GLuint ps = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ps, 1, &pFile, nullptr);
	glCompileShader(ps);
	if(!verifyShader(ps))
		return false;

	m_program = glCreateProgram();
	glAttachShader(m_program, ps);
	glAttachShader(m_program, vs);
	glLinkProgram(m_program);

	GLint param = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &param);
	if(param == GL_FALSE)
	{
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &param);
		GLchar* pLog = new GLchar[param];
		glGetProgramInfoLog(m_program, param, nullptr, pLog);
		LOGE("init( %s )", pLog);
		SAFE_DELETE_ARRAY(pLog);
		return false;
	}

	glDeleteShader(vs);
	glDeleteShader(ps);

	m_viewProjectionLocation = glGetUniformLocation(m_program, VIEW_PROJECTION);

	bindTo(mesh);

	return true;
}

void ItemShader::bindTo(const IMesh& mesh)
{
	if(m_vao <= 0)
		glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
	glBindBuffer(GL_ARRAY_BUFFER, mesh.getStaticBuffer());

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.getDynamicBuffer());

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Instance), 0);
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Instance), BUFFER_OFFSET(12));
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);
}