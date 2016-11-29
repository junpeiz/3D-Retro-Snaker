#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <vector>
#include <cfloat>
#include <cmath>
#include <algorithm>
#include <exception>
#include "../../Const_def.h"

using namespace std;

vector<float> findMaxMin(const vector<vector<float>> &v);
void normalize(vector<vector<float>> &v, float upper = 1, float lower = -1);
void stride_add(vector<vector<float>> &v, float addend, unsigned int start, unsigned int stride);
void stride_add(vector<float> &v, float addend, unsigned int start, unsigned int stride);
void rotate_arbitary_3D(vector<vector<float>> &v, float theta, vector<float> p1, vector<float> p2);
void rotate_arbitary_3D(vector<float> &v, float theta, vector<float> p1, vector<float> p2);
void multiply(vector<vector<float>> &v, float ratio);

#endif
