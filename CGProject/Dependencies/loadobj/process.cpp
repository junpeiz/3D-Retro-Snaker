#include "load_obj.h"

bool Object::loadOBJ(const string &path, int max_dim)
{
    using vec3 = std::tuple<float, float, float>;
    using vec2 = std::pair<float, float>;
    
    vertices.resize(max_dim + 1);
    textures.resize(max_dim + 1);
    normals.resize(max_dim + 1);
    mtl_record.resize(max_dim + 1);
    
    vector<vector<int>> vIndices, vtIndices, vnIndices;
    vIndices.resize(max_dim + 1);
    vtIndices.resize(max_dim + 1);
    vnIndices.resize(max_dim + 1);
    
    vector<vec3> v;
    vector<vec2> vt;
    vector<vec3> vn;
    
    std::ifstream fin(path, std::ios::in);
    if (!fin.is_open())
    {
        std::cout << "invalid file path" << std::endl;
        return false;
    }
    
    string str, current_mtl;
    vector<string> isSameMTL(max_dim + 1);
    while (!fin.eof())
    {
        std::getline(fin, str);
        
        string op(str.substr(0, str.find_first_of(' ')));
        if (op.empty())
            continue;
        
        std::istringstream iss(str);
        vector<string> element{std::istream_iterator<string>{iss},
            std::istream_iterator<string>{}};
        if (op == "v")
        {
            CHECK(element.size(), 4);
            v.emplace_back(std::make_tuple(std::stod(element[1]),
                                           std::stod(element[2]),
                                           std::stod(element[3])));
        }
        else if (op == "vt")
        {
            //CHECK(element.size(), 3);
            vt.emplace_back(std::make_pair(std::stod(element[1]),
                                           std::stod(element[2])));
        }
        else if (op == "vn")
        {
            //CHECK(element.size(), 4);
            vn.emplace_back(std::make_tuple(std::stod(element[1]),
                                            std::stod(element[2]),
                                            std::stod(element[3])));
        }
        else if (op == "usemtl")
        {
            CHECK(element.size(), 2);
            current_mtl = element[1];
        }
        else if (op == "f")
        {
            auto index = element.size() - 1;
            CHECK_GREAT(element.size(), 3);
            CHECK_LESS(index, max_dim);
            
            if (str.find("//") != std::string::npos)
            {
                for (auto i = 1; i != element.size(); ++i)
                {
                    string &e = element[i];
                    vIndices[index].push_back(std::stoi(e.substr(0, e.find_first_of('/'))));
                    vnIndices[index].push_back(std::stoi(e.substr(e.find_last_of('/') + 1)));
                    vtIndices[index].push_back(-1);
                }
            }
            else if (str.find_first_of('/') == str.find_last_of('/'))
            {
                for (auto i = 1; i != element.size(); ++i)
                {
                    string &e = element[i];
                    vIndices[index].push_back(std::stoi(e.substr(0, e.find_first_of('/'))));
                    vnIndices[index].push_back(-1);
                    vtIndices[index].push_back(std::stoi(e.substr(e.find_last_of('/') + 1)));
                }
            }
            else
            {
                for (auto i = 1; i != element.size(); ++i)
                {
                    string &e = element[i];
                    vIndices[index].push_back(std::stoi(e.substr(0, e.find_first_of('/'))));
                    vnIndices[index].push_back(std::stoi(e.substr(e.find_last_of('/') + 1)));
                    vtIndices[index].push_back(std::stoi(e.substr(e.find_first_of('/') + 1,
                                                                  e.find_last_of('/'))));
                }
            }
            if (current_mtl == isSameMTL[index])
                mtl_record[index].push_back(string());
            else
            {
                mtl_record[index].push_back(current_mtl);
                isSameMTL[index] = current_mtl;
            }
        }
    }
    
    for (auto i = 0; i != vIndices.size(); ++i)
    {
        for (auto j = 0; j != vIndices[i].size(); ++j)
        {
            auto vIndex = vIndices[i][j], vtIndex = vtIndices[i][j], vnIndex = vnIndices[i][j];
            
            vec3 vertice = v[vIndex - 1];
            vec2 texture = vtIndex < 0 ? vec2(0, 0) : vt[vtIndex - 1];
            //vec3 normal  = vnIndex < 0 ? vec3(0, 0, 0) : vn[vnIndex - 1];
            
            pushTupleVector<float>(vertices[i], vertice);
            //pushTupleVector<float>(normals[i], normal);
            pushPairVector<float>(textures[i], texture);
        }
    }
    return true;
}

bool Object::loadMTL(const string &path)
{
    std::ifstream fin(path, std::ios::in);
    
    if (!fin.is_open())
    {
        std::cout << "Faild Open .mtl" << std::endl;
        return false;
    }
    
    string str;
    string current_mtl_name;
    while (!fin.eof())
    {
        std::getline(fin, str);
        
        if (str.empty())
            continue;
        
        if (str.front() == '#' || str.find_first_of(' ') == string::npos)
            continue;
        
        std::istringstream iss(str);
        vector<string> element{std::istream_iterator<string>{iss},
            std::istream_iterator<string>{}};
        string &op = element.front();
        
        if (op == "newmtl")
        {
            CHECK(element.size(), 2);
            mtl.insert(std::make_pair(element[1], Material()));
            current_mtl_name = element[1];
        }
        else if (op == "Ns")
        {
            CHECK(element.size(), 2);
            mtl[current_mtl_name].Ns = std::stof(element[1]);
        }
        else if (op == "Ka")
        {
            CHECK(element.size(), 4);
            mtl[current_mtl_name].Ka = vector<float>{   std::stof(element[1]),
                std::stof(element[2]),
                std::stof(element[3])};
        }
        else if (op == "Kd")
        {
            CHECK(element.size(), 4);
            mtl[current_mtl_name].Kd = vector<float>{   std::stof(element[1]),
                std::stof(element[2]),
                std::stof(element[3])};
        }
        else if (op == "Ks")
        {
            CHECK(element.size(), 4);
            mtl[current_mtl_name].Ks = vector<float>{   std::stof(element[1]),
                std::stof(element[2]),
                std::stof(element[3])};
        }
        else if (op == "Ni")
        {
            CHECK(element.size(), 2);
            mtl[current_mtl_name].Ni = std::stof(element[1]);
        }
        else if (op == "d")
        {
            CHECK(element.size(), 2);
            mtl[current_mtl_name].d = std::stof(element[1]);
        }
        else if (op == "illum")
        {
            CHECK(element.size(), 2);
            mtl[current_mtl_name].illum = std::stof(element[1]);
        }
    }
    return true;
}