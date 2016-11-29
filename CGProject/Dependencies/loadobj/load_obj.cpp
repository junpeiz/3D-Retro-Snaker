#include "load_obj.h"

bool Object::initVAO()
{
    if (!is_loaded){
        std::cout << "Faild loading OBJ" << std::endl;
        return false;
    }
    for (auto i = 0; i < vertices.size(); ++i){
        if (!vertices[i].empty())
        {
            vertexArray.push_back(GLuint(i));
            vertexBuffer.push_back(GLuint(i));
            
            glGenBuffers(1, &vertexBuffer.back());
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.back());
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices[i].size(),
                         vertices[i].data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            #ifdef __APPLE__
            glGenVertexArraysAPPLE(1, &vertexArray.back());
            glBindVertexArrayAPPLE(vertexArray.back());
            #elif defined _WIN32 || defined _WIN64
            glGenVertexArrays(1, &vertexArray.back());
            glBindVertexArray(vertexArray.back());
            #endif
            
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.back());
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        }
    }
    glBindVertexArrayCrossOp(0);
    return true;
}

void Object::_drawWithMtl()
{
    auto VAOIndex = 0;
    for (auto i = 0; i < vertices.size(); ++i)
    {
        if (!vertices[i].empty())
        {
            vector<string> &__mtl = mtl_record[i];
            
            /* p is face */
            auto p = 1, last_p = 0;
            string last_mtl, cur_mtl = __mtl[0];
            while (p < __mtl.size())
            {
                while (1)
                {
                    if (p == __mtl.size() - 1)
                    {
                        last_mtl = __mtl[p].empty() ? last_mtl: __mtl[p];
                        break;
                    }
                    else if (!__mtl[p].empty())
                    {
                        last_mtl = cur_mtl;
                        cur_mtl = __mtl[p];
                        break;
                    }
                    else
                        ++p;
                }
                changeMtl(mtl[last_mtl]);
                
                glBindVertexArrayCrossOp(vertexArray[VAOIndex]);
                auto vertice_gap = p - last_p;
                if (p == __mtl.size() - 1)
                    ++vertice_gap;
                
                if (i == 3)
                    glDrawArrays(GL_TRIANGLES, last_p * 3, vertice_gap * 3);
                else
                {
                    vector<GLsizei> count(vertice_gap, i);
                    vector<GLint> first(count.size());
                    std::partial_sum(count.begin(), count.end(), first.begin());
                    std::for_each(first.begin(), first.end(), [i, last_p](GLint &a)
                                  {
                                      a += (last_p * i - i);
                                  });
                    glMultiDrawArrays(GL_TRIANGLE_FAN, first.data(),
                                      count.data(), (GLsizei)count.size());
                }
                last_p = p;
                p++;
            }
            VAOIndex++;
        }
    }
    glBindVertexArrayCrossOp(0);
}

void Object::centerOBJ()
{
	centerX = 0, centerY = 0, centerZ = 0;
	float factor = 0;
	for (const auto &vi : vertices)
	{
		if (!vi.empty())
		{
			auto __size = vi.size();
			CHECK(__size % 3, 0);
			for (int i = 0; i != __size; i += 3)
			{
				centerX += vi[i];
				centerY += vi[i + 1];
				centerZ += vi[i + 2];
			}
			factor += __size / 3;
		}
	}
	centerX /= factor;
	centerY /= factor;
	centerZ /= factor;
}

void Object::changeMtl(const Material &m)
{
	if (!m.Ka.empty())
		glMaterialfv(GL_FRONT, GL_AMBIENT, m.Ka.data());
	if (!m.Kd.empty())
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m.Kd.data());
	if (!m.Ks.empty())
		glMaterialfv(GL_FRONT, GL_SPECULAR, m.Ks.data());
	glMaterialf(GL_FRONT, GL_SHININESS, 128);
}

void Object::_drawWithoutMtl()
{
    auto VAOIndex = 0;
    for (auto i = 0; i < vertices.size(); ++i)
    {
        if (!vertices[i].empty())
        {
            glBindVertexArrayCrossOp(vertexArray[VAOIndex++]);
            if (i == 3)
                glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(vertices[i].size() / 3));
            else
            {
                vector<GLsizei> count(vertices[i].size() / i / 3, i);
                vector<GLint> first(count.size());
                std::partial_sum(count.begin(), count.end(), first.begin());
                std::for_each(first.begin(), first.end(), [i](GLint &a) { a -= i; });
                glMultiDrawArrays(GL_TRIANGLE_FAN, first.data(),
                                  count.data(), (GLsizei)count.size());
            }
        }
    }
    glBindVertexArrayCrossOp(0);
}

void Object::draw()
{
    if (mtl.empty())
        _drawWithoutMtl();
    else
        _drawWithMtl();
}

void Object::updateBuffer()
{
    auto count = 0;
    for (auto i = 0; i < vertices.size(); ++i)
    {
        if (!vertices[i].empty())
        {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[count]);
            float *buf = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            if (buf)
            {
                std::memcpy(buf, vertices[i].data(), sizeof(float) * vertices[i].size());
                glUnmapBuffer(GL_ARRAY_BUFFER);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            count++;
        }
    }
}

void Object::reset(const string &obj_path, const string &mtl_path, int max_dim)
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
		std::cout << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;

	if (!vertexBuffer.empty())
	{
		glDeleteBuffers((GLsizei)vertexBuffer.size(), vertexBuffer.data());
		vertexBuffer.clear();
	}

	if (!mtl_path.empty())
		loadMTL(mtl_path);
	is_loaded = loadOBJ(obj_path, max_dim);

	normalize(vertices);
	centerOBJ();

	float diff_x = -centerX, diff_y = -centerY, diff_z = -centerZ;

	stride_add(vertices, diff_x, 0, 3);
	stride_add(vertices, diff_y, 1, 3);
	stride_add(vertices, diff_z, 2, 3);

	centerX = 0;
	centerY = 0;
	centerZ = 0;

	initVAO();

	vector<float> MinMax(findMaxMin(vertices));
	bounding_box = vector<float>
	{
		MinMax[0], MinMax[2], MinMax[4],
		MinMax[0], MinMax[3], MinMax[4],
		MinMax[1], MinMax[3], MinMax[4],
		MinMax[1], MinMax[2], MinMax[4],
		MinMax[0], MinMax[2], MinMax[5],
		MinMax[0], MinMax[3], MinMax[5],
		MinMax[1], MinMax[3], MinMax[5],
		MinMax[1], MinMax[2], MinMax[5],
	};
}

void Object::draw(float x, float y, float z)
{
    float diff_x = x - centerX, diff_y = y - centerY, diff_z = z - centerZ;
    
    stride_add(vertices, diff_x, 0, 3);
    stride_add(vertices, diff_y, 1, 3);
    stride_add(vertices, diff_z, 2, 3);
    
    stride_add(bounding_box, diff_x, 0, 3);
    stride_add(bounding_box, diff_y, 1, 3);
    stride_add(bounding_box, diff_z, 2, 3);
    
    if (diff_x != 0 || diff_y != 0 || diff_z != 0)
    {
        updateBuffer();
    }
    
    draw();
    
    centerX = x;
    centerY = y;
    centerZ = z;
}

void Object::rotate(float theta, float x, float y, float z)
{
    rotate_arbitary_3D(vertices, theta,
                       {centerX, centerY, centerZ}, {x, y, z});
    rotate_arbitary_3D(bounding_box, theta, {centerX, centerY, centerZ}, {x, y, z});
    updateBuffer();
}

void Object::scale(float ratio)
{
    multiply(vertices, ratio);
    updateBuffer();
}

void Object::glBindVertexArrayCrossOp(GLuint index)
{
#ifdef __APPLE__
	glBindVertexArrayAPPLE(index);
#elif defined _WIN32 || defined _WIN64
	glBindVertexArray(index);
#endif
}
