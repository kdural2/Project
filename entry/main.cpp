#include "algorithms.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


int main(){
    Map newFlights("routes_clean.csv", "airports.dat");
    //input file
    ifstream ifs;
    //output file
    ofstream ofs;
    ifs.open("input.txt");
    ofs.open("output.txt");

    //Get location to start BFS from
    string startBFSlocation;
    //Initialize starting BFS index as invalid.
    int startBFS = -1;
    while (startBFS < 0) {
        getline(ifs, startBFSlocation);
        //converts index to airport ID.
        startBFS = newFlights.toIATA(startBFSlocation);
    }

    //Find BFS airport and search.
    cout << ">>>>>>>>>>> BFS Start Location <<<<<<<<<<<" << endl << "                  " << newFlights.convertToName(startBFSlocation) << endl << endl;
    ofs << ">>>>>>>>>>> BFS Start Location <<<<<<<<<<<" << endl << "                  " << startBFSlocation << endl << endl;
    vector<string> bfs = newFlights.BFS(startBFS);
    int count = 0;
    for (size_t i = 0; i < bfs.size() - 1; i++) {
        //Just in case IATA column is empty (avoids possible errors)
        if (bfs[i] != "") {
            ofs << bfs[i] << ", ";
            count++;
            if (count == 20) {
                ofs << endl;
                count = 0;
            }
        }
    }
    ofs << bfs[bfs.size() - 1] << endl;
    ofs << endl;
    cout << "BFS Success!" << endl << endl;

    //Continues to find optimal paths until end of input.txt
    while (ifs.peek() != EOF) {

        //Find starting airport
        string startingAirport;
        int startingAirportIndex = -1; 

        while (startingAirportIndex < 0) {
            //Check to see if input.txt still has valid airports.
            if (ifs.peek() == EOF) {
                cout<<"Please add valid USA airports."<<endl;
                return 0;
            }
            getline(ifs, startingAirport);
            //Convert index to ID.
            startingAirportIndex = newFlights.toIATA(startingAirport);
        }
    
        cout << ">>>>>>>>>>> Source Airport <<<<<<<<<<<" << endl << "                  " << newFlights.convertToName(startingAirport) << endl << endl;
        ofs << ">>>>>>>>>>> Source Airport <<<<<<<<<<<" << endl << "                  " << startingAirport << endl << endl;

        //Find final airport.
        string finalAirport;
        //Initialize as invalid index.
        int finalAirportIndex = -1; 
        while (finalAirportIndex < 0) {
            //make sure valid airport exists in input.txt
            if (ifs.peek() == EOF) { 
                cout<<"Please add valid destination US airport to input.txt"<<endl;
                return 0;
            }
            getline(ifs, finalAirport);
            //Convert index to airport ID.
            finalAirportIndex = newFlights.toIATA(finalAirport);
        }
        cout << ">>>>>>>>>>> Destination Airport <<<<<<<<<<<" << endl << "                  " << newFlights.convertToName(finalAirport) << endl << endl;
        ofs << ">>>>>>>>>>> Destination Airport <<<<<<<<<<<" << endl << "                  " << finalAirport << endl << endl;


        //Use BFS again from start airport to final airport.
        vector<string> bfsPath = newFlights.BFS(startingAirportIndex, finalAirportIndex);
        //No route found from starting airport to destination.
        if (bfsPath.empty() == true) {
            cout << "There are no possible commercial flight routes that connect " << startingAirport << " and " << finalAirport << endl;
            ofs << "There are no possible commercial flight routes that connect " << startingAirport <<" and " << finalAirport << endl;
            cout << endl;
            ofs << endl;
            //Stops search.
            continue;
        }
        cout << "To minimize total number of layovers, the path you should take is: " << endl;
        ofs << "To minimize total number of layovers, the path you should take is: " << endl;
        for (size_t i = 0; i < bfsPath.size() - 1; i++) {
            string s = bfsPath[i];
            cout << newFlights.convertToName(s) << " to ";
            ofs << s << " to ";
        }
        cout << bfsPath[bfsPath.size() - 1] << endl;
        ofs << bfsPath[bfsPath.size() - 1] << endl;

        //Dijskstras algorithm
        tuple<vector<string>,double> dijkstra = newFlights.dijkstra(startingAirportIndex, finalAirportIndex);
        cout << endl << "To minimize total distance flown, the ideal path is: " << endl;
        ofs << endl << "To minimize total distance flown, the ideal path is: " << endl;
        for (size_t i = 0; i < get<0>(dijkstra).size() - 1; i++) {
            cout << newFlights.convertToName(get<0>(dijkstra)[i]) << " to ";
            ofs << get<0>(dijkstra)[i] << " to ";
        }
        cout << get<0>(dijkstra)[get<0>(dijkstra).size() - 1] << endl;
        ofs << get<0>(dijkstra)[get<0>(dijkstra).size() - 1] << endl;
        cout << endl << ">>>>>>>>>>> Total Distance Flown <<<<<<<<<<<" << endl << "                  " << get<1>(dijkstra) << " miles" << endl << endl;
        ofs << endl << "Total distance flown: " << get<1>(dijkstra) << " miles" << endl;
    }

    ifs.close();
    ofs.close();

    return 0;
}
