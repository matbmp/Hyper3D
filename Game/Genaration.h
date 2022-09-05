#ifndef GENARATION_H
#define GENARATION_H

#include <queue>
#include "Math/Vector3.h"
#include "Graphics/Color.h"
#include "gameobject.h"

class Schunk{
public:
    std::vector<GyroVector> tiles;
    std::vector<Color> tileColors;
    std::vector<std::vector<bool> > walls;

    Schunk(){
        float tileWidth = O4SquareTiling::TileWidth();
        GyroVector w = GyroVector(Vector3(0, 0, tileWidth));
        GyroVector a = GyroVector(Vector3(tileWidth, 0, 0));

        tiles.push_back(GyroVector());
        tiles.push_back(w);
        tiles.push_back(-w);
        tiles.push_back(a);
        tiles.push_back(-a);

        //left up corner
        tiles.push_back(tiles[1] - a); //5
        tiles.push_back(tiles[4] + w);

        //right up corner
        tiles.push_back(tiles[1] + a); //7
        tiles.push_back(tiles[3] + w);

        //left down corner
        tiles.push_back(tiles[2] - a); //9
        tiles.push_back(tiles[4] - w);

        //right down corner

        tiles.push_back(tiles[2] + a); //11
        tiles.push_back(tiles[3] - w);


        for(uint i = 0; i < tiles.size(); i++){
            walls.push_back(std::vector<bool>());
            for(int j = 0; j < 4; j++){
                walls[i].push_back(false);
            }
        }


        walls[1][2] = true;
        walls[2][0] = true;
        walls[0][2] = true;
        walls[3][3] = true;
        walls[4][1] = true;

        walls[5][2] = true;
        walls[5][1] = true;
        walls[6][1] = true;
        walls[6][2] = true;

        walls[7][2] = true;
        walls[7][3] = true;
        walls[8][2] = true;
        walls[8][3] = true;

        walls[9][0] = true;
        walls[9][1] = true;
        walls[10][0] = true;
        walls[10][1] = true;

        walls[11][0] = true;
        walls[11][3] = true;
        walls[12][0] = true;
        walls[12][3] = true;


        Color colors[] = {Color(26, 26, 189)*(1/255.0F), Color(204, 52, 14)*(1/255.0F), Color(220, 186, 227)*(1/255.0F)};
        Color c, c2;
        float alpha;
        for(uint i = 0; i < tiles.size(); i++)
        {
            //float cc = (rand()%1000)%1000;
            c = colors[0];
            for (int j = 1; j < 3; j++)
            {
                c2 = colors[j];

                alpha = (j % 2 == 0) ? tiles[i].vec.Dot(tiles[i].vec, tiles[i].vec) : (tiles[i].gyr.z);
                c.r = (c.r * alpha + (1 - alpha) * c2.r);
                c.g = (c.g * alpha + (1 - alpha) * c2.g);
                c.b = (c.b * alpha + (1 - alpha) * c2.b);
            }
            tileColors.push_back(c);
        }
    }
};


#endif // GENARATION_H
