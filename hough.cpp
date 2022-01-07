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

int drawCircle(int **screen,int row, int col, int xc, int yc, int radius){
    int xmax = (int) (radius * 0.72710678); // maximum x at radius/sqrt(2)
    int y = radius;
    int y2 = y * y;
    int ty = (2 * y) - 1;
    int y2_new = y2;
    for (int x = 0; x <= xmax; x++) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }
        if(x + xc < row && y + yc < col && x + xc > 0 && y + yc > 0){
            screen[x + xc][y + yc] = 1;
        }
        if(x + xc < row && -y + yc < col && x + xc > 0 && -y + yc > 0){
            screen[x + xc][-y + yc] = 1;
        }
        if(-x + xc < row && y + yc < col && -x + xc > 0 && y + yc > 0){
            screen[-x + xc][y + yc] = 1;
        }
        if(-x + xc < row && -y + yc < col && -x + xc > 0 && -y + yc > 0){
            screen[-x + xc][-y + yc] = 1;
        }
        if(y + xc < row && x + yc < col && y + xc > 0 && x + yc > 0){
            screen[y + xc][x + yc] = 1;
        }
        if(y + xc < row && -x + yc < col && y + xc > 0 && -x + yc > 0){
            screen[y + xc][-x + yc] = 1;
        }
        if(-y + xc < row && x + yc < col && -y + xc > 0 && x + yc > 0){
            screen[-y + xc][x + yc] =  1;
        }
        if(-y + xc < row && -x + yc < col && -y + xc > 0 && -x + yc > 0){
            screen[-y + xc][-x + yc] = 1;
        }
        y2_new -= (2 * x) - 3;
    }
    return 0;
}

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
    int** originalImage = new int*[row * 3];
    int** edge = new int*[row];
    int** edge1 = new int*[row];
    int** angle = new int*[row];
    int** htrans = new int*[row];
    for(int i = 0; i < row; ++i){
        greyscale[i] = new int[col];
        originalImage[i*3] = new int[col];
        originalImage[i*3+1] = new int[col];
        originalImage[i*3+2] = new int[col];
        edge[i] = new int[col];
        edge1[i] = new int[col];
        angle[i] = new int[col];
        htrans[i] = new int[col];
    }

    int raux;
    int gaux;
    int baux;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){

            infile >> raux >> gaux >> baux;
            int grey = (raux + gaux + baux)/3;
            greyscale[i][j] = grey;
            originalImage[i*3][j] = raux;
            originalImage[(i*3)+1][j] = gaux;
            originalImage[(i*3)+2][j] = baux;


        }
    }

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
                double theta = atan(smoothy/smoothx) * (180/3.14159265);
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
            htrans[i][j] = 0;
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
    int maxRadius = 14;

    vector<std::vector<std::vector<double>>> votes(row, vector<vector<double>>(col, std::vector<double>(maxRadius, 0))); //this allows instant access to x, y , radius, votes
    vector<int> radii;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(edge[i][j] == 1) {

                for (int r = -maxRadius; r < maxRadius; r++) {
                    for (int c = -maxRadius; c < maxRadius; c++) {
                        int x = i + r;
                        int y = j + c;
                        if (x > 0 && x < row - 1 && y > 0 && y < col - 1) {
                            int radius = round(sqrt(pow(r, 2) + pow(c, 2)));

                            if (radius < maxRadius) {
                                /*bool prevVote = votes.find(x) != votes.end() &&
                                                votes[x].find(y) != votes[x].end() &&
                                                votes[x][y].find(radius) != votes[x][y].end();*/
                                votes[x][y][radius] = votes[x][y][radius] + 1;
                            }
                        }
                    }
                }
            }
        }
    }

    int circleThres = 45;
    int minRadius = 500;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            for(int rad = 0; rad < maxRadius; rad++){
                int maxVotes = 0;
                int maxIndex[3] =  {0,0,0};
                if(votes[i][j][rad] > circleThres* (1 + (abs(rad-11)/6))){ //* (1 + (abs(rad-11)/6)) could add this to account for the difference in points that will come
                    //due to larger sized coins
                    cout<<"x val: "<< i <<" y val: "<<j<<" radius: "<<rad<<" votes: "<< votes[i][j][rad]<<endl;
                    for(int r = -20; r < 20; r++){
                        for(int c = -10; c < 10; c++){
                            for(int radc = -8; radc < 8; radc++){
                                if(i + r > 0 && i + r < row - 1 && j + c > 0 && j + c < col - 1){
                                    int posVotes = votes[i + r][j + c][rad + radc];
                                    if(posVotes > maxVotes){
                                        maxVotes = posVotes;
                                        maxIndex[0] = i + r;
                                        maxIndex[1] = j + c;
                                        maxIndex[2] = rad + radc;
                                    }
                                    votes[i+r][j+c][rad+radc] = 0;
                                }
                            }

                        }
                    }

                    drawCircle(htrans, row, col, maxIndex[0], maxIndex[1], maxIndex[2]);
                    drawCircle(htrans, row, col, maxIndex[0], maxIndex[1], maxIndex[2]+1);
                    drawCircle(htrans, row, col, maxIndex[0], maxIndex[1], maxIndex[2]+2);
                    if(maxIndex[2] < minRadius) {
                        minRadius = maxIndex[2];
                    }
                    radii.push_back(maxIndex[2]);
                }
            }
        }
    }
    int pennies = 0;
    int dimes = 0;
    int nickels = 0;
    int quarters = 0;

    for(int i = 0; i < radii.size(); i++){

        double rad = (double) radii[i];
        double min = (double) minRadius;
        double ratio =rad  / min;

        if(ratio > 1.324) {
            quarters++;
        }
        else if(ratio > 1.1184){
            nickels++;
        }
        else if(ratio > 1.0638){
            pennies++;
        }
        else{
            dimes++;
        }
    }


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

    std::ofstream outfile1("imagec.ppm");
    outfile1 <<"P3 "<<col<< " "<<row<< " 255\n"<<endl;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            int val = htrans[i][j];
            if(val == 1){
                outfile1 <<255<<" "<<0<< " "<<0<<" ";
            }
            else{

                outfile1 <<originalImage[i*3][j]<<" "<<originalImage[i*3 + 1][j]<< " "<<originalImage[i*3 + 2][j]<<" ";
            }



        }
        outfile1 << endl;
    }
    outfile1.close();

    std::ofstream outresult("result.txt");
    outresult << "quarters: "<< quarters<< " dimes: "<<dimes<< " nickels: "<< nickels<< " pennies: "<<pennies<<endl;
    outresult << "Total Sum: "<<(quarters * .25 + dimes * .1 + nickels * .05 + pennies* .01);
    outresult.close();

    cout << "quarters: "<< quarters<< " dimes: "<<dimes<< " nickels: "<< nickels<< " pennies: "<<pennies<<endl;
    cout << "Total Sum: "<<(quarters * .25 + dimes * .1 + nickels * .05 + pennies* .01);
    return 0;

}