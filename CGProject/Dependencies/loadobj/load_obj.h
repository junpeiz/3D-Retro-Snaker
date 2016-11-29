#ifndef LOAD_OBJ_H
#define LOAD_OBJ_H

#include "..\..\Const_def.h"
#include "analysis.h"
#include "..\glew\glew.h"
#include "..\freeglut\freeglut.h"
#include <map>
#include <iostream>
#include <tuple>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <memory>
#include <string>
#include <numeric>
#include <vector>

using namespace std;

struct Material
{
	vector<float> Ka, Kd, Ks;
	float d, Tr, Ns, Ni, illum;
	string map_Ka;
};

class Object
{
public:
	Object() {}
	/* from the start to look at, positive means anti-clockwise */
	void rotate(float theta, float x, float y, float z);
	void scale(float ratio);
	void draw();
	vector<float> getCenter() { return{ centerX, centerY, centerZ }; }
	vector<float> getBoundingBox() { return bounding_box; }
	void draw(float x, float y, float z);
	void reset(const string &path, const string &mtl_path, int max_dim = 10);
private:
    void centerOBJ();
    bool initVAO();
    void updateBuffer();
	bool loadOBJ(const string &path, int max_dim);
	bool loadMTL(const string &path);
	void changeMtl(const Material &m);
    template <typename T>
    void pushTupleVector(vector<T> &v, std::tuple<T, T, T> t);
    template <typename T>
    void pushPairVector(vector<T> &v, std::pair<T, T> p);
    void _drawWithMtl();
    void _drawWithoutMtl();
    void glBindVertexArrayCrossOp(GLuint);
    vector<float> bounding_box;
    vector<vector<string>> mtl_record;
    map<string, Material> mtl;
    bool is_loaded = false;
	vector<vector<float>> vertices;
	vector<vector<float>> textures;
	vector<vector<float>> normals;
    vector<GLuint> vertexArray;
    vector<GLuint> vertexBuffer;
    float centerX;
    float centerY;
    float centerZ;
};

template <typename T>
void Object::pushTupleVector(vector<T> &v, std::tuple<T, T, T> t)
{
    v.push_back(std::get<0>(t));
    v.push_back(std::get<1>(t));
    v.push_back(std::get<2>(t));
}

template <typename T>
void Object::pushPairVector(vector<T> &v, std::pair<T, T> p)
{
    v.push_back(p.first);
    v.push_back(p.second);
}

#endif /* load_obj.h */
