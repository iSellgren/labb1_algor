//
//  main.cpp
//  labb1_algor
//
//  Created by Fredrik Sellgren on 2019-03-29.
//  Copyright © 2019 Fredrik Sellgren. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <ctype.h>
#include <stack>
#include <queue>

struct vertex{
    
    int index;
    std::string name;
    bool visited = false;
};

using vertices = std::vector<vertex>;

struct edge{
    
    vertices::iterator start,dest;
    float length;
    std::string name;
    
    bool operator<(edge rhs)
    {
        return length < rhs.length;
    }
    
};
struct Djik
{
    float length = std::numeric_limits<float>::infinity();
    int prev = std::numeric_limits<float>::min();
};

using edges =  std::vector<edge>;


using spot = edges::iterator;

using matrix = std::vector<std::vector<spot>>;

class tree
{
public:
    
    void make()
    {
        std::string line;
        std::ifstream ifs("export.txt");
        if(!ifs.is_open())
            perror("error when opening file");
        while(getline(ifs,line, ' '))
        {
            
            if(line[0] =='#')
                ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if(line[0] == 'M')
            {
                vertex v;
                getline(ifs, line,' ');
                v.index = stoi(line);
                getline(ifs, line);
                v.name = line;
                _vertices.push_back(v);
                
            }
            if(isdigit(line[0]))
            {
                int start = stoi(line);
                getline(ifs, line,' ');
                int dest = stoi(line);
                getline(ifs, line,' ');
                float length = stof(line);
                getline(ifs, line);
                line.erase(0,1);
                line.erase(line.end()-1);
                
                auto pointA = std::find_if(_vertices.begin(), _vertices.end(),[&](const vertex v)
                                           {
                                               return v.index == start;
                                           });
                auto pointB = std::find_if(_vertices.begin(), _vertices.end(),[&](const vertex v)
                                           {
                                               return v.index == dest;
                                           });
                
                _edges.push_back(edge{pointA,pointB,length,line});
                
                
            }
            
        }
        if(ifs.bad())
            perror("error while reading file");
        
        spot s{_edges.end()};
        _matrix.resize(_vertices.size(), std::vector<spot>(_vertices.size(), s ) );
        
        for(auto _edge = _edges.begin(); _edge != _edges.end(); _edge++)
        {
            
            _matrix[_edge->start->index][_edge->dest->index] = _edge;
        }
        
    }
    
    void display()
    {
        
        
        std::cout << "   ";
        
        for(int i = 0; i < _vertices.size(); i++)
        {
            std::string str = std::to_string(i);
            
            while(str.size() < 4) str.push_back(' ');
            
            std::cout << str;
        }
        
        std::cout << '\n';
        
        
        for(int i = 0; i < _matrix.size(); i++)
        {
            std::string str = std::to_string(i);
            
            while(str.size() < 3) str.push_back(' ');
            
            std::cout << str;
            
            for(int j = 0; j < _matrix[i].size(); j++)
            {
                if(_matrix[i][j]->length == 0.f)
                {
                    std::cout << "    ";
                    continue;
                }
                std::string str = std::to_string(_matrix[i][j]->length);
                while(str.size() > 3) str.pop_back();
                
                std::cout << str << ' ';
            }
            
            std::cout << '\n';
        }
    }
    
    bool DFS()
    {
        std::vector<vertices::iterator> grannar;
        std::stack<vertices::iterator> curpos;
        curpos.push(_vertices.begin());
        
        for(auto &v : _vertices) v.visited = false;
        
        while (!curpos.empty()) {
            curpos.top()->visited = true;
            grannar.clear();
            for(auto &i : _matrix[curpos.top()->index])
            {
                if(i != _edges.end() && !i->dest->visited)
                {
                    grannar.push_back(i->dest);
                }
            }
            
            std::cout << curpos.top()->name << "\n";
            
            if(!grannar.empty())
            {
                curpos.push(grannar.front());
            }
            else curpos.pop();
            
            
        }
        std::cout << '{';
        for(auto &v : _vertices) { std::cout << v.visited << ' '; }
        std::cout << "}\n";
        
        return std::find_if(_vertices.begin(), _vertices.end(), [&](const vertex &v)
                            {
                                if(!v.visited) std::cout << v.index <<" index " << v.name << " was not visited\n";
                                return !v.visited;
                            }) == _vertices.end();
    }
    
    bool BFS()
    {
        std::vector<vertices::iterator> grannar;
        std::queue<vertices::iterator> qurpos;
        for(auto &v : _vertices) v.visited = false;
        qurpos.push(_vertices.begin());
        qurpos.front()->visited = true;
        
        while (!qurpos.empty()) {
            grannar.clear();
            for(auto &i : _matrix[qurpos.front()->index])
            {
                if(i != _edges.end()  && !i->dest->visited)
                {
                    grannar.push_back(i->dest);
                }
            }
            
            std::cout << qurpos.front()->name << "\n";
            
            
            if(!grannar.empty())
            {
                grannar.front()->visited = true;
                qurpos.push(grannar.front() );
                
                
            }
            else qurpos.pop();
            
        }
        std::cout << '{';
        for(auto &v : _vertices) { std::cout << v.visited << ' '; }
        std::cout << "}\n";
        
        return std::find_if(_vertices.begin(), _vertices.end(), [&](const vertex &v)
                            {
                                if(!v.visited) std::cout << v.index <<" index " << v.name << " was not visited\n";
                                return !v.visited;
                            }) == _vertices.end();
        
        
        
        return 0;
    }
    void scv()
    {
        auto save = [&]()
        {
            for(auto &c: _matrix)
                for(auto &r : c)
                    if(r == _edges.end() ) ++r;
        };
        
        for(int i = 0; i < _matrix.size(); i++)
        {
            for (int j = i; j < _matrix[i].size(); j++)
            {
                if(_matrix[i][j] == _edges.end() && _matrix[j][i] != _edges.end())
                {
                    
                    save();
                    _edges.push_back(edge{_matrix[j][i]->dest,_matrix[j][i]->start,_matrix[j][i]->length,_matrix[j][i]->name});
                    _matrix[i][j] = _edges.end() -1;
                }
                else if(_matrix[j][i] == _edges.end() && _matrix[i][j] != _edges.end())
                {
                    
                    save();
                    _edges.push_back(edge{_matrix[i][j]->start,_matrix[i][j]->dest,_matrix[i][j]->length,_matrix[i][j]->name});
                    _matrix[j][i] = _edges.end() -1;
                }
            }
        }
        
        std::cout << " Scv good to go sir" << std::endl;
    }
    
    int shortest(matrix& _matrix, vertices& _vertices, int e, std::vector<Djik>& shortPath) {
        for (int i = 0; i < _matrix.size(); i++)
        {
            if (_matrix[e][i]!=_edges.end() && !_vertices[i].visited && shortPath[i].length > (_matrix[e][i]->length + shortPath[e].length))
            {
                shortPath[i].length = _matrix[e][i]->length + shortPath[e].length;
                shortPath[i].prev = e;
            }
        }
        float min = std::numeric_limits<float>::max();
        int index = 0;
        
        for (int i = 0; i < _matrix.size(); i++)
        {
            if (!_vertices[i].visited && shortPath[i].length < min)
            {
                min = shortPath[i].length;
                index = i;
            }
        }
        _vertices[index].visited = true;
        return index;
    }
    
    
    
    void dijk(int start, int end)
    {
        for(auto &v : _vertices) v.visited = false;
        std::vector<Djik> shortPath(_vertices.size());
        shortPath[start].length = 0;
        shortPath[start].prev = start;
        _vertices[start].visited = true;
        
        int at = shortest(_matrix, _vertices, start, shortPath);
        int tempo = 0;
        while (tempo != _vertices.size())
        {
            tempo = 0;
            at = shortest(_matrix, _vertices, at, shortPath);
            for (int i = 0; i < _vertices.size(); i++)
            {
                if(_vertices[i].visited)
                    tempo++;
            }
            
        }
        
        for(int i = 0; i < shortPath.size(); i++)
        {
            std::cout << i << "\t" << shortPath[i].length << '\n';
            
        }
        
        while(end != start)
        {
            std::cout << _vertices[end].name << "<-";
            end = shortPath[end].prev;
        }
        std::cout << std::endl;
    }
    
    
    
private:
    vertices _vertices;
    edges _edges;
    matrix _matrix;
};



int main() {
    
    tree make;
    
    make.make();
    
    //make.display();
    //make.BFS();
    make.scv();
    //make.display();
    std::cout << "--------------------------------------------" << std::endl;
    //make.DFS();
    make.dijk(24,37);
    //make.display();
    
    
    
    return 0;
}
