#include "analysis.h"

void rotate_arbitary_3D(vector<vector<float>> &v, float theta, vector<float> p1, vector<float> p2)
{
	CHECK(p1.size(), 3);
	CHECK(p2.size(), 3);
    vector<float> offset = p1;
    for (auto i = 0; i != 3; ++i){
        p2[i] -= p1[i];
    }
    float len = sqrtf(p2[0] * p2[0] + p2[1] * p2[1] + p2[2] * p2[2]);
    for (auto &e: p2){
        e /= len;
    }
    stride_add(v, -p1[0], 0, 3);
    stride_add(v, -p1[1], 1, 3);
    stride_add(v, -p1[2], 2, 3);
    float &nx = p2[0], &ny = p2[1], &nz = p2[2];
    for (auto &vi: v){
        if (!vi.empty()){
            for (auto i = 0; i < vi.size(); i += 3){
                float _x =  vi[i] * (cosf(theta) + nx * nx * (1 - cosf(theta))) +
                            vi[i + 1] * (nx  * ny * (1 - cosf(theta)) - nz * sinf(theta)) +
                            vi[i + 2] * (nx * nz * (1 - cosf(theta)) + ny * sinf(theta));
                float _y =  vi[i] * (nx * ny * (1 - cosf(theta)) + nz * sinf(theta)) +
                            vi[i + 1] * (ny * ny * (1 - cosf(theta)) + cosf(theta)) +
                            vi[i + 2] * (ny * nz * (1 - cosf(theta)) - nx * sinf(theta));
                float _z =  vi[i] * (nx * nz * (1 - cosf(theta)) - ny * sinf(theta)) +
                            vi[i + 1] * (ny * nz * (1 - cosf(theta)) + nx * sinf(theta)) +
                            vi[i + 2] * (nz * nz * (1 - cosf(theta)) + cosf(theta));
                vi[i] = _x;
                vi[i + 1] = _y;
                vi[i + 2] = _z;
            }
        }
    }
    stride_add(v, p1[0], 0, 3);
    stride_add(v, p1[1], 1, 3);
    stride_add(v, p1[2], 2, 3);
}

void rotate_arbitary_3D(vector<float> &v, float theta,
                        vector<float> p1, vector<float> p2)
{
	CHECK(p1.size(), 3);
	CHECK(p2.size(), 3);
    vector<float> offset = p1;
    for (auto i = 0; i != 3; ++i)
    {
        p2[i] -= p1[i];
    }
    float len = sqrtf(p2[0] * p2[0] + p2[1] * p2[1] + p2[2] * p2[2]);
    for (auto &e: p2)
    {
        e /= len;
    }
    stride_add(v, -p1[0], 0, 3);
    stride_add(v, -p1[1], 1, 3);
    stride_add(v, -p1[2], 2, 3);
    float &nx = p2[0], &ny = p2[1], &nz = p2[2];
    if (!v.empty())
    {
        for (auto i = 0; i < v.size(); i += 3)
        {
            float _x =  v[i] * (cosf(theta) + nx * nx * (1 - cosf(theta))) +
                        v[i + 1] * (nx  * ny * (1 - cosf(theta)) - nz * sinf(theta)) +
                        v[i + 2] * (nx * nz * (1 - cosf(theta)) + ny * sinf(theta));
            float _y =  v[i] * (nx * ny * (1 - cosf(theta)) + nz * sinf(theta)) +
                        v[i + 1] * (ny * ny * (1 - cosf(theta)) + cosf(theta)) +
                        v[i + 2] * (ny * nz * (1 - cosf(theta)) - nx * sinf(theta));
            float _z =  v[i] * (nx * nz * (1 - cosf(theta)) - ny * sinf(theta)) +
                        v[i + 1] * (ny * nz * (1 - cosf(theta)) + nx * sinf(theta)) +
                        v[i + 2] * (nz * nz * (1 - cosf(theta)) + cosf(theta));
            
            v[i]     = _x;
            v[i + 1] = _y;
            v[i + 2] = _z;
        }
    }
}

void multiply(vector<vector<float>> &v, float ratio)
{
    for (auto &vi: v){
        if (!vi.empty()){
            std::for_each(vi.begin(), vi.end(), [ratio](float &e){
				e *= ratio;
            });
        }
    }
}

vector<float> findMaxMin(const vector<vector<float>> &v)
{
    float _minX = FLT_MAX, _minY = FLT_MAX, _minZ = FLT_MAX;
    float _maxX = FLT_MIN, _maxY = FLT_MIN, _maxZ = FLT_MIN;
    for (const auto &vi: v){
        if (!v.empty()){
            for (auto i = 0; i < vi.size(); i += 3){
                _minX = std::min(vi[i], _minX);
                _maxX = std::max(vi[i], _maxX);
                
                _minY = std::min(vi[i + 1], _minY);
                _maxY = std::max(vi[i + 1], _maxY);
                
                _minZ = std::min(vi[i + 2], _minZ);
                _maxZ = std::max(vi[i + 2], _maxZ);
            }
        }
    }
    return {_minX, _maxX, _minY, _maxY, _minZ, _maxZ};
}

void normalize(vector<vector<float>> &v, float upper, float lower)
{
	float min = FLT_MAX, max = FLT_MIN;
	for (const auto &vi : v) {
		if (!vi.empty()) {
			min = std::min(*(std::min_element(vi.begin(), vi.end())), min);
			max = std::max(*(std::max_element(vi.begin(), vi.end())), max);
		}
	}
	if (max == min) {
		float mid = (upper + lower) / 2;
		for (auto &vi : v) {
			vi = vector<float>(vi.size(), mid);
		}
	}
	else {
		for (auto &vi : v) {
			if (!vi.empty()) {
				std::for_each(vi.begin(), vi.end(), [min, max, upper, lower](float &a) {
					a = (upper - lower) * (a - min) / (max - min) + lower;
				});
			}
		}
	}
}

void stride_add(vector<vector<float>> &v, float addend, unsigned int start, unsigned int stride)
{
	if (addend == 0){
		return;
	}
	for (auto &vi : v){
		if (!vi.empty() && vi.size() > start){
			auto p = start;
			while (p < vi.size()){
				vi[p] += addend;
				p += stride;
			}
		}
	}
}

void stride_add(vector<float> &v, float addend, unsigned int start, unsigned int stride)
{
	if (!v.empty() && v.size() > start){
		auto p = start;
		while (p < v.size()){
			v[p] += addend;
			p += stride;
		}
	}
}