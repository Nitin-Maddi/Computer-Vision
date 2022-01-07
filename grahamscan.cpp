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

class Point{
    double myx, myy;
    public:
        Point(){

            myx = ((double) rand() / (RAND_MAX));
            myy = ((double) rand() / (RAND_MAX));
        }
        Point(double x1, double y1){
            myx = x1;
            myy = y1;
        }
        double x(){
            return myx;
        }
        double y(){
            return myy;
        }
        void setX(double x1){
            myx = x1;
        }
        void setY(double y1){
            myy = y1;
        }
};

vector<Point> hull;
Point p0;

int makeLine(int screen[800][800], int x1, int y1, int x2, int y2){
    // make it so that x2 is always greater than x1 (this might do something im not sure)
    int dx = x2 - x1;
    int dy = y2 - y1;
    int temp = 0;
    if(abs(dx) >= abs(dy)){
        if(x1 > x2){
            temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
    }
    else{
        if(y1 > y2){
            temp = y1;
            y1 = y2;
            y2 = temp;
            temp = x1;
            x1 = x2;
            x2 = temp;
        }
    }
    dx = x2-x1;
    dy = y2-y1;
    int j = 0;
    int epsilon = 0;

    if(abs(dx) >= abs(dy)){
        epsilon = dy - dx;
        j = y1;
        for(int i = x1; i < x2; i++)
        {
            screen[i][j] = 1;
            if(epsilon >= 0){
                if(dy > 0){
                    j++;
                }
                else{
                    j--;
                }

                epsilon -= dx;
            }
            if(dy > 0){
                epsilon += dy;
            }
            else{
                epsilon -= dy;
            }
        }
    }
    else{
        epsilon = dx - dy;
        j = x1;
        for(int i = y1; i < y2; i++)
        {
            screen[j][i] = 1;
            if(epsilon >= 0){
                if(dx > 0){
                    j++;
                }
                else{
                    j--;
                }
                epsilon -= dy;
            }
            if(dx > 0){
                epsilon += dx;
            }
            else{
                epsilon -= dx;
            }
        }
    }
    return 0;
}
int drawCircle(int screen[800][800], int xc, int yc, int radius){
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
        if(x + xc < 800 && y + yc < 800 && x + xc > 0 && y + yc > 0){
            screen[x + xc][y + yc] = 1;
        }
        if(x + xc < 800 && -y + yc < 800 && x + xc > 0 && -y + yc > 0){
            screen[x + xc][-y + yc] = 1;
        }
        if(-x + xc < 800 && y + yc < 800 && -x + xc > 0 && y + yc > 0){
            screen[-x + xc][y + yc] = 1;
        }
        if(-x + xc < 800 && -y + yc < 800 && -x + xc > 0 && -y + yc > 0){
            screen[-x + xc][-y + yc] = 1;
        }
        if(y + xc < 800 && x + yc < 800 && y + xc > 0 && x + yc > 0){
            screen[y + xc][x + yc] = 1;
        }
        if(y + xc < 800 && -x + yc < 800 && y + xc > 0 && -x + yc > 0){
            screen[y + xc][-x + yc] = 1;
        }
        if(-y + xc < 800 && x + yc < 800 && -y + xc > 0 && x + yc > 0){
            screen[-y + xc][x + yc] =  1;
        }
        if(-y + xc < 800 && -x + yc < 800 && -y + xc > 0 && -x + yc > 0){
            screen[-y + xc][-x + yc] = 1;
        }
        y2_new -= (2 * x) - 3;
    }
    return 0;
}

double distanceFromLine(Point first, Point second, Point p){
    return abs((p.y() - first.y()) * (second.x() - first.x()) -
              (second.y() - first.y()) * (p.x() - first.x()));


}
int findSide(Point first, Point second, Point p){
    double val = (p.y() - first.y()) * (second.x() - first.x()) -
              (second.y() - first.y()) * (p.x() - first.x());

    if (val > 0)
        return 1;
    if (val < 0)
        return -1;
    return 0;
}
void recurhull(vector<Point> points, Point firstPoint, Point secondPoint){
    int index = -1;
    double maxDist = 0;
    for(int i = 0; i < points.size(); i++){
        double tempDist = distanceFromLine(firstPoint, secondPoint, points[i]);
        if(tempDist > maxDist){
            index = i;
            maxDist = tempDist;

        }
    }
    if(index == -1){
        return;
    }
    int insertIndex = 0;
    for(int i = 0; i < hull.size(); i++){
        if(firstPoint.x() == hull[i].x() && firstPoint.y() == hull[i].y()){
            insertIndex = i;
        }
    }
    hull.insert(hull.begin() + insertIndex + 1, points[index]);
    vector<Point> points1;
    vector<Point> points2;
    int outside1 = -findSide(points[index], firstPoint, secondPoint);
    int outside2 = -findSide(points[index], secondPoint, firstPoint);

    for(int i = 0; i < points.size(); i++){

        if(findSide(points[index], firstPoint, points[i]) == outside1){
            points1.push_back(points[i]);
        }
        if(findSide(points[index], secondPoint, points[i]) == outside2){
            points2.push_back(points[i]);
        }
    }
    recurhull(points1, firstPoint, points[index]);
    recurhull(points2, points[index], secondPoint);

}

void quickhull(vector<Point> points){
    Point maxPoint = points[0];
    Point minPoint = points[0];
    for(int i = 0; i < points.size(); i++){
        Point p = points[i];
        if(p.x() > maxPoint.x()){
            maxPoint = p;
        }
        if(p.x() < minPoint.x()){
            minPoint = p;
        }
    }

    hull.push_back(minPoint);
    hull.push_back(maxPoint);
    vector<Point> top;
    vector<Point> bottom;
    for(int i = 0; i < points.size(); i++){
        Point p = points[i];
        int side = findSide(minPoint, maxPoint, p);
        if(side == 1){
            top.push_back(p);

        }
        else if(side == -1){
            bottom.push_back(p);
        }
    }
    recurhull(top, minPoint, maxPoint);
    recurhull(bottom, maxPoint, minPoint);
}
bool polar_angle(Point p1, Point p2){
    return -(p1.x() - p0.x())/(p1.y() - p0.y()) < -(p2.x() - p0.x())/(p2.y() - p0.y());

}
Point secondTop(stack<Point> stk){
    Point atTop = stk.top();
    stk.pop();
    Point secondT = stk.top();
    stk.push(atTop);
    return secondT;

}
void grahamscan(int screen[800][800], vector<Point> points){
    int minYIndex = 0;
    for(int i = 0; i < points.size(); i++){
        if(points[i].y() < points[minYIndex].y()){
            minYIndex = i;

        }
    }
    Point temp = points[0];
    points[0] = points[minYIndex];
    points[minYIndex] = temp;
    p0 = points[0];
    sort(points.begin()+1, points.end(), polar_angle);

    stack<Point> stk;
    stk.push(points[0]);
    stk.push(points[1]);
    stk.push(points[2]);
    for(int i = 3; i < points.size(); i++){
        while(findSide(secondTop(stk), stk.top(), points[i]) != 1){
            stk.pop();
        }
        stk.push(points[i]);
    }

    int stksize = stk.size();
    for(int i = 0; i < stksize; i++){
        Point p = stk.top();
        if(i < stksize-1) {
            Point p2 = secondTop(stk);
            stk.pop();
            makeLine(screen, round(p.x() * 800), round(p.y() * 800), round(p2.x() * 800), round(p2.y() * 800));
        }
        if(i == 0){
            makeLine(screen, round(p.x() * 800), round(p.y() * 800), round(points[0].x() * 800), round(points[0].y() * 800));
        }
    }
}

int main() {
    srand(time(0));
    int displayScreen[800][800] = {{0}};
    vector<Point> points;
    for (int i = 0; i < 50; i++) {
        Point p = Point();
        points.push_back(p);
        drawCircle(displayScreen, round(points[i].x() * 800), round(points[i].y() * 800), 2);

    }
    //quickhull(points);
    grahamscan(displayScreen, points);

    /*
    for(int i = 0; i < hull.size(); i++){
        if(i < hull.size() -1){
            makeLine(displayScreen, round(hull[i].x() * 800), round(hull[i].y() * 800), round(hull[i+1].x() * 800), round(hull[i+1].y() * 800));
        }
        else{
            makeLine(displayScreen, round(hull[0].x() * 800), round(hull[0].y() * 800), round(hull[i].x() * 800), round(hull[i].y() * 800));
        }
    }*/

    std::ofstream outfile("cv.ppm");
    outfile <<"P3 800 800 1\n"<<endl;
    for(int i=0; i<800; i++)
    {
        for(int j=0; j<800; j++)
        {
            if(displayScreen[i][j] == 0){
                outfile << "1 1 1" << " ";
            }
            else{
                outfile << "0 0 0" << " ";
            }
        }
        outfile << endl;
    }
    outfile.close();



    return 0;

}