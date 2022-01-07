//Nitin Maddi

#include <iostream>
#include<cstdlib>
#include<fstream>
#include<math.h>
#include<random>
#include<string>
#include<list>
#include<vector>
#include<iterator>
#include<set>
#include<iomanip>
#include<stack>
#include <unordered_map>

using namespace std;
int main() {
    int row = 0;
    int col = 0;
    int maximum = 0;
    int ysobeloperator[3][3] = {{-1,0,1},
                                {-2,0,2},
                                {-1,0,1}};
    int xsobeloperator[3][3] = {{-1,-2,-1},
                                {0,  0, 0},
                                {1,  2, 1}};
    int direction[5][3][2] = {{{0,1}, {0,0}, {0,-1}},  //-90
                              {{-1,-1}, {0,0}, {1,1}}, //-45
                              {{-1,0}, {0,0}, {1,0}}, //0
                              {{1,-1}, {0,0}, {-1,1}}, //45
                              {{0,1}, {0,0}, {0,-1}}}; //90
    ifstream infile;
    infile.open("image.ppm");
    string pSix;
    infile >> pSix;
    infile >> col >> row >> maximum;

    int** greyscale = new int*[row];
    int** edge = new int*[row];
    int** edge1 = new int*[row];
    int** angle = new int*[row];
    for(int i = 0; i < row; ++i){
        greyscale[i] = new int[col];
        edge[i] = new int[col];
        edge1[i] = new int[col];
        angle[i] = new int[col];
    }

    int raux;
    int gaux;
    int baux;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            infile >> raux >> gaux >> baux;
            int grey = (raux + gaux + baux)/3;
            greyscale[i][j] = grey;

        }
    }
    std::ofstream outfile("imageg.ppm");
    outfile <<"P3 "<<col<< " "<<row<< " 255\n"<<endl;
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            int val = greyscale[i][j];
            outfile <<val<<" "<<val<< " "<<val<<" ";

        }
        outfile << endl;
    }
    outfile.close();
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if( i > 0 && i < row-1 && j > 0 && j < col-1){
                double smoothx = 0;
                double smoothy = 0;
                for(int row = -1; row < 2; row++){
                    for(int col = -1; col < 2; col++){
                        smoothx += greyscale[i + row][j + col] * xsobeloperator[row + 1][col + 1];
                        smoothy += greyscale[i + row][j + col] * ysobeloperator[row + 1][col + 1];
                    }
                }

                double mag = sqrt(pow(smoothx,2) + pow(smoothy,2));
                double theta = atan(smoothx/smoothy) * (180/3.14159265);
                int dir = lround(theta/45);
                double maxgrey = greyscale[i][j];
                for(int r = 0; r < 3; r++){
                    int xindex = direction[dir+2][r][0];
                    int yindex = direction[dir+2][r][1];
                    if(maxgrey < greyscale[i+xindex][j+yindex])
                        maxgrey = greyscale[i+xindex][j+yindex];
                }
                if(mag > 150){
                    edge1[i][j] = 1;
                }
                if(mag > 250)
                    edge[i][j] = 2;
                else if(mag > 100)
                    edge[i][j] = 1;
                else
                    edge[i][j] = 0;

                if(maxgrey != greyscale[i][j])
                    angle[i][j] = 1;
                else
                    angle[i][j] = 0;

            }
            else {

                edge[i][j] = 0;
                edge1[i][j] = 0;

            }
        }
    }
    int count = 1;
    int count1 = 0;
    while(count > 0){
        //cout<<count1<<endl;
        count1++;
        count = 0;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (i > 0 && i < row - 1 && j > 0 && j < col - 1) {
                    if (edge[i][j] == 1) {
                        for(int r = -1; r < 2; r++){
                            for(int c = -1; c < 2; c++){
                                if(edge[i+r][j+c] == 2){
                                    edge[i][j] = 2;
                                    count++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (edge[i][j] == 2 && angle[i][j] == 1) {
                edge[i][j] = 1;

            } else {
                edge[i][j] = 0;
            }

        }
    }
    std::ofstream outedge("image1.ppm");
    outedge <<"P3 "<<col<< " "<<row<< " 1\n"<<endl;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            int val = angle[i][j];
            outedge <<val<<" "<<val<< " "<<val<<" ";
        }
        outedge << endl;
    }
    outedge.close();

    std::ofstream outerfile("imagef.ppm");
    outerfile <<"P3 "<<col<< " "<<row<< " 1\n"<<endl;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            int val = edge[i][j];
            outerfile <<val<<" "<<val<< " "<<val<<" ";
        }
        outerfile << endl;
    }
    outerfile.close();

    return 0;

}