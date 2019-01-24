#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <math.h>
#include <cmath>
using namespace::std;
int length, width;
const int NS = 1;
const int NESW = 2;
const int WE = 3;
const int NWSE = 4;
const double PI = 3.14159265158979;
const int STRONG = 10;
const int WEAK = 20;
const int NONE = 0;
class Magdir
{
private:
    int magnitude;
    int direction;
    int power;
    double slope;
public:
    Magdir() { magnitude = 0; direction = 0; power = 0; slope = 0.0;};
    Magdir(int mag, int dir, double slop) { magnitude = mag; direction = dir; power = 0; slope = slop;};
    int getMag() { return magnitude; };
    void setDir(int dir) { direction = dir; };
    int getDir() { return direction; };
    void setMag(int mag) { magnitude = mag; };
    int getPow() { return power; };
    void setPow(int pow) { power = pow; };
    double getSlope() {return slope;};
};
class Point
{
private:
    int myX;
    int myY;
public:
    Point() {myX = 0; myY = 0; };
    Point(int a, int b) {myX = a; myY = b; };
    int X() {return myX;};
    void setX(int a) {myX = a;};
    int Y() {return myY;};
    void setY(int b) {myY = b;};
};

vector<vector<int>> togray(string ppm)
{
    string format;
    string w, l, next, one, two, three, mean;
    int first, second, third, average;
    ifstream getter;
    ofstream grayer;
    getter.open(ppm);
    getline(getter, format);
    cout << "" + format << endl;
    getline(getter, w, ' ');
    getline(getter, l);
    cout << "w" + w + "w" << endl;
    cout << "l" + l + "l" << endl;
    getline(getter, next);
    cout << next << endl;
    grayer.open("/Users/austinhuang/Documents/Computer Vision/MoneyReader/MoneyReader/grayscale.ppm");
    grayer << format << endl;
    grayer << w + " " + l << endl;
    grayer << "255" << endl;
    width = std::stoi(w);
    length = std::stoi(l);
    vector<vector<int>> toReturn = vector<vector<int>>();
    toReturn.resize(length);
    for (int down = 0; down < length; down++)
    {
        toReturn[down].resize(width);
        for (int across = 0; across < width; across++)
        {
            getline(getter, one, ' ');
            getline(getter, two, ' ');
            getline(getter, three, ' ');
            first = std::stoi(one);
            second = std::stoi(two);
            third = std::stoi(three);
            average = (first + second + third) / 3;
            mean = to_string(average);
            toReturn[down][across] = average;
            grayer << mean + " " + mean + " " + mean + " ";
            
        }
        grayer << endl;
    }
    getter.close();
    grayer.close();
    return toReturn;
}

int roundedGradientAngle(int Gy, int Gx)
{
    double drad = atan2(double(Gy), double(Gx));
    double rad = drad * 180 / PI;
    if (rad <= -157.5 || rad > 157.5 || (rad <= 22.5 && rad > -22.5))
        return WE;
    else if ((rad >= 112.5 && rad < 157.5) || (rad >= -67.5 && rad < -22.5))
        return NWSE;
    else if ((rad <= 112.5 && rad > 67.5) || (rad <= -67.5 && rad > -112.5))
        return NS;
    else if ((rad >= 22.5 && rad < 67.5) || (rad >= -157.5 && rad < -112.5))
        return NESW;
    cout << "uh-oh" << endl;
    return -1;
}

vector<vector<Magdir>> toedge(string ppm)
{
    ifstream getter;
    getter.open(ppm);
    string next, w, l;
    getline(getter, next);
    getline(getter, w, ' ');
    getline(getter, l);
    getline(getter, next);
    int width = std::stoi(w);
    int length = std::stoi(l);
    vector<vector<int>> grayscale = vector<vector<int>>(length);
    for(int y = 0; y < length; y++)
    {
        grayscale[y].resize(width);
        for(int x = 0; x < width; x++)
        {
            getline(getter, next, ' '); getline(getter, next, ' '); getline(getter, next, ' ');
            grayscale[y][x] = std::stoi(next);
        }
    }
    getter.close();
    cout<<"in"<<endl;
    double Gx, Gy;
    double averaged_third, averaged_two_thirds, standard_dev, sum;
    averaged_third = 0;
    standard_dev = 0;
    sum = 0;
    int total = (width-2) * (length - 2);
    vector<vector<Magdir>> Gmagdir = vector<vector<Magdir>>();
    
    int const xsobel[3][3] = { {-2, 0, 2},
        {-4, 0, 4},
        {-2, 0, 2} };
    int const ysobel[3][3] = { {-2, -4, -2},
        {0, 0, 0},
        {2, 4, 2} };
    
    Gmagdir.resize(length);
    Gmagdir[0].resize(width);
    cout<<"ok"<<endl;
    for (int x = 0; x < width; x++)
    {
        Gmagdir[0][x] = Magdir();
    }
    cout<<"done"<<endl;
    for (int down = 1; down < length - 1; down++)
    {
        //cout<<to_string(down)<<endl;
        Gmagdir[down].resize(width);
        for (int across = 1; across < width - 1; across++)
        {
            Gx = (xsobel[0][0] * grayscale[down - 1][across - 1] + xsobel[0][1] * grayscale[down - 1][across] + xsobel[0][2] * grayscale[down - 1][across + 1]
                  + xsobel[1][0] * grayscale[down][across - 1] + xsobel[1][1] * grayscale[down][across] + xsobel[1][2] * grayscale[down][across + 1]
                  + xsobel[2][0] * grayscale[down + 1][across - 1] + xsobel[2][1] * grayscale[down + 1][across] + xsobel[2][2] * grayscale[down + 1][across + 1]);
            
            Gy = (ysobel[0][0] * grayscale[down - 1][across - 1] + ysobel[0][1] * grayscale[down - 1][across] + ysobel[0][2] * grayscale[down - 1][across + 1]
                  + ysobel[1][0] * grayscale[down][across - 1] + ysobel[1][1] * grayscale[down][across] + ysobel[1][2] * grayscale[down][across + 1]
                  + ysobel[2][0] * grayscale[down + 1][across - 1] + ysobel[2][1] * grayscale[down + 1][across] + ysobel[2][2] * grayscale[down + 1][across + 1]);
            int magn = int(sqrt(pow(Gx, 2) + pow(Gy, 2)));
            sum += magn;
            standard_dev += (magn * magn);
            if(Gx == 0.0)
            {
                Gx = 0.000000001;
            }
            
            Gmagdir[down][across] = Magdir(magn, roundedGradientAngle(Gy, Gx), (double)Gy*10.0/Gx/10.0);
        }
    }
    cout<<"checkpoint"<< endl;
    double mean = sum / total;
    standard_dev += total * mean * mean - sum * 2 * mean;
    standard_dev = standard_dev / total;
    standard_dev = sqrt(standard_dev);
    
    averaged_third = mean  + 2.5*standard_dev;
    averaged_two_thirds = mean + 3.5*standard_dev;
    
    Gmagdir[length - 1].resize(width);
    for (int x = 0; x < width; x++)
    {
        Gmagdir[length - 1][x] = Magdir();
    }
    
    //non-max suppression
    for (int down = 1; down < length - 1; down++)
    {
        for (int across = 1; across < width - 1; across++)
        {
            if (Gmagdir[down][across].getMag() <= averaged_third)
                Gmagdir[down][across].setPow(NONE);
            else if (Gmagdir[down][across].getMag() <= averaged_two_thirds)
                Gmagdir[down][across].setPow(WEAK);
            else
                Gmagdir[down][across].setPow(STRONG);
            
            if (Gmagdir[down][across].getDir() == NS)
            {
                if (Gmagdir[down + 1][across].getMag() > Gmagdir[down][across].getMag() || Gmagdir[down - 1][across].getMag() > Gmagdir[down][across].getMag())
                {
                    Gmagdir[down][across].setMag(-1);
                }
            }
            else if (Gmagdir[down][across].getDir() == WE)
            {
                if (Gmagdir[down][across + 1].getMag() > Gmagdir[down][across].getMag() || Gmagdir[down][across - 1].getMag() > Gmagdir[down][across].getMag())
                {
                    Gmagdir[down][across].setMag(-1);
                }
            }
            else if (Gmagdir[down][across].getDir() == NESW)
            {
                if (Gmagdir[down + 1][across - 1].getMag() > Gmagdir[down][across].getMag() || Gmagdir[down - 1][across + 1].getMag() > Gmagdir[down][across].getMag())
                {
                    Gmagdir[down][across].setMag(-1);
                }
            }
            else if (Gmagdir[down][across].getDir() == NWSE)
            {
                if (Gmagdir[down + 1][across + 1].getMag() > Gmagdir[down][across].getMag() || Gmagdir[down - 1][across - 1].getMag() > Gmagdir[down][across].getMag())
                {
                    Gmagdir[down][across].setMag(-1);
                }
            }
        }
    }
    
    //double threshold//strong weak//hysteresis
    for (int down = 1; down < length - 1; down++)
    {
        for (int across = 1; across < width - 1; across++)
        {
            if (Gmagdir[down][across].getPow() == WEAK)
            {
                if (Gmagdir[down][across].getDir() == NS)
                {
                    if (Gmagdir[down + 1][across].getPow() == STRONG || Gmagdir[down - 1][across].getPow() == STRONG)
                    {
                        Gmagdir[down][across].setPow(STRONG);
                    }
                    else
                        Gmagdir[down][across].setPow(NONE);
                }
                else if (Gmagdir[down][across].getDir() == WE)
                {
                    if (Gmagdir[down][across + 1].getPow() == STRONG || Gmagdir[down][across - 1].getPow() == STRONG)
                    {
                        Gmagdir[down][across].setPow(STRONG);
                    }
                    else
                        Gmagdir[down][across].setPow(NONE);
                }
                else if (Gmagdir[down][across].getDir() == NESW)
                {
                    if (Gmagdir[down + 1][across - 1].getPow() == STRONG || Gmagdir[down - 1][across + 1].getPow() == STRONG)
                    {
                        Gmagdir[down][across].setPow(STRONG);
                    }
                    else
                        Gmagdir[down][across].setPow(NONE);
                }
                else if (Gmagdir[down][across].getDir() == NWSE)
                {
                    if (Gmagdir[down + 1][across + 1].getPow() == STRONG || Gmagdir[down - 1][across - 1].getPow() == STRONG)
                    {
                        Gmagdir[down][across].setPow(STRONG);
                    }
                    else
                        Gmagdir[down][across].setPow(NONE);
                }
            }
        }
    }
    
    
    return Gmagdir;
}

vector<Point> getCentersOfCircles(vector<vector<Magdir>> input)
{
    cout << "Inside getCenterOfCircles" << endl;
    vector<vector<int>> votes;
    int row_size = input[0].size();
    int col_size = input.size();
    int direction, xCurrent, yCurrent;
    int checker, changer;
    double epsilon;
    bool drivingAxisVertical;
    int dependentDelta;
    double m;
    int startX, startY;
    votes.resize(input.size());
    for(int y = 0; y < input.size(); y++)
    {
        votes[y].resize(row_size);
        std::fill(votes[y].begin(), votes[y].end(), 0);
    }
    cout << "reached1" << endl;
    for(int y = 0; y < col_size; y++)
    {
        for(int x = 0; x < row_size; x++)
        {
            if(input[y][x].getPow() == STRONG)
            {
                int max = 4032;
                int counter = 0;
                m = input[y][x].getSlope();
                startX = x;
                startY = y;
                if(abs(m) >= 1)
                    drivingAxisVertical = true;
                else
                    drivingAxisVertical = false;
                if(m <= 0)
                {
                    dependentDelta = -1;
                }
                else
                {
                    dependentDelta = 1;
                }
                if (!drivingAxisVertical) //sets up the cases for if the driving axis is x-axis
                {
                    counter = 0;
                    epsilon = abs(m) - 1.00;
                    int j = startY;
                    int i = startX;
                    changer = 1;
                    checker = dependentDelta;
                    while (i != row_size && j != col_size && j >= 0 && counter <= max)//actually running the Bres algorithm
                    {
                        votes[j][i] = votes[j][i] + 1;
                        if (epsilon >= 0)
                        {
                            epsilon -= 1.0;
                            j += checker;
                        }
                        i += changer;
                        epsilon += abs(m);
                        counter++;
                    }
                    counter = 0;
                    j = startY;
                    i = startX;
                    changer = -1;
                    checker = dependentDelta*-1;
                    while (i != 0 && j != col_size && j >= 0 && counter >= 0)//actually running the Bres algorithm
                    {
                        votes[j][i] = votes[j][i] + 1;
                        if (epsilon >= 0)
                        {
                            epsilon -= 1.0;
                            j += checker;
                        }
                        i += changer;
                        epsilon += abs(m);
                        counter++;
                    }
                }
                else //sets up the cases for if the driving axis is y-axis
                {
                    counter = 0;
                    m = 1.0/m;
                    epsilon = abs(m) - 1.00;
                    int j = startX;
                    int i = startY;
                    changer = 1;
                    checker = dependentDelta;
                    while (i != col_size && j != row_size && j >= 0 && counter <= max)//actually running the Bres algorithm
                    {
                        votes[i][j] = votes[i][j] + 1;
                        if (epsilon >= 0.0)
                        {
                            epsilon -= 1.0;
                            j += checker;
                        }
                        i += changer;
                        epsilon += abs(m);
                        counter++;
                    }
                    counter = 0;
                    j = startX;
                    i = startY;
                    changer = -1;
                    checker = dependentDelta*-1;
                    while (i != 0 && j != row_size && j >= 0 && counter <= max)//actually running the Bres algorithm
                    {
                        votes[i][j] = votes[i][j] + 1;
                        if (epsilon >= 0)
                        {
                            epsilon -= 1.0;
                            j += checker;
                        }
                        i += changer;
                        epsilon += abs(m);
                        counter++;
                    }
                }
            }
        }
    }
    
    double mean, standard_dev, sum;
    sum = 0.0;
    standard_dev = 0.0;
    int total = col_size * row_size;
    int max = 0;
    for(int y = 0; y < col_size; y++)
    {
        for(int x = 0; x < row_size; x++)
        {
            if(votes[y][x] >= max)
            {
                max = votes[y][x];
                cout << "Max: " + to_string(max) << endl;
            }
            sum += votes[y][x];
            standard_dev += (votes[y][x] * votes[y][x]);
        }
    }
    cout << "Sum: " + to_string(sum)<< endl;
    mean = sum / total;
    standard_dev += total * mean * mean - sum * 2 * mean;
    standard_dev = standard_dev / total;
    standard_dev = sqrt(standard_dev);
    
    double threshold = 5*standard_dev + mean;
    cout << "Threshold: " + to_string(threshold) << endl;
    
    vector<Point> toReturn;
    for(int y = 0; y < votes.size(); y++)
    {
        for(int x = 0; x < row_size; x++)
        {
            if((double)votes[y][x] >= threshold)
            {
                toReturn.push_back(Point(x, y));
                cout << "(x, y): (" + to_string(x) + ", " + to_string(y) + ")" << endl;
            }
        }
    }
    return toReturn;
}

void putCirclesInPPM(vector<vector<Magdir>> input)
{
    string format, w, l, next;
    ifstream getter;
    ofstream modifier;
    getter.open("/Users/austinhuang/Documents/Computer Vision/MoneyReader/MoneyReader/grayscale.ppm");
    vector<Point> centers = getCentersOfCircles(input);
    vector<vector<int>> ppm = vector<vector<int>>();
    ppm.resize(input.size());
    getline(getter, format);
    cout << "" + format << endl;
    getline(getter, w, ' ');
    getline(getter, l);
    cout << "w" + w + "w" << endl;
    cout << "l" + l + "l" << endl;
    getline(getter, next);
    cout << next << endl;
    
    width = std::stoi(w);
    length = std::stoi(l);
    
    for(int y = 0; y < length; y++)
    {
        ppm[y].resize(input[0].size());
        for(int x = 0; x < width; x++)
        {
        getline(getter, next, ' '); getline(getter, next, ' '); getline(getter, next, ' ');
        ppm[y][x] = std::stoi(next);
        }
    }
    getter.close();
    
    for(int index = 0; index < centers.size(); index++)
    {
        ppm[centers[index].Y()][centers[index].X()] = -1;
    }
    
    modifier.open("/Users/austinhuang/Documents/Computer Vision/MoneyReader/MoneyReader/graffiti.ppm");
    modifier << format << endl;
    modifier << w + " " + l << endl;
    modifier << "255" << endl;
    int color;
    for(int y = 0; y < ppm.size(); y++)
    {
        for(int x = 0; x < ppm[0].size(); x++)
        {
            color = ppm[y][x];
            if(color == -1)
                modifier << "225 0 0 ";
            else
                modifier << to_string(ppm[y][x]) + " " + to_string(ppm[y][x]) + " " + to_string(ppm[y][x]) + " ";
        }
        modifier << endl;
    }
    modifier.close();
    return;
}

void getOutput(vector<vector<Magdir>> image)
{
    ofstream edger;
    Magdir element;
    edger.open("/Users/austinhuang/Documents/Computer Vision/MoneyReader/MoneyReader/edgeimage.ppm");
    edger << "P3" << endl;
    edger << to_string(image[0].size()) + " " + to_string(image.size()) << endl;
    edger << "1" << endl;
    
    for (int y = 0; y < image.size(); y++)
    {
        for (int x = 0; x < image[0].size(); x++)
        {
            element = image[y][x];
            if (element.getPow() != NONE)
                edger << "1 1 1 ";
            else
                edger << "0 0 0 ";
        }
        edger << endl;
    }
    edger.close();
}

int main()
{
    vector<vector<int>> grayscale;
    vector<vector<Magdir>> edged;
    grayscale = togray("/Users/austinhuang/Documents/Computer Vision/MoneyReader/MoneyReader/image.ppm");
    edged = toedge("/Users/austinhuang/Documents/Computer Vision/MoneyReader/MoneyReader/grayscale.ppm");
    getOutput(edged);
    putCirclesInPPM(edged);
}
