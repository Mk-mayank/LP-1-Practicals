#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

class FCFS{
    public:
    int process_id;
    int burst_time;
    int Arrival_time ;

    FCFS(int p , int b , int a){
        this->process_id = p ;
        this->burst_time = b ;
        this->Arrival_time = a ;
    }
};

class RR{
    public:
    int process_id;
    int burst_time ;
    int arrivalTime ;
    int remainingTime;
    int waitingTime ;
    int TAT;
    int completionTime;

    RR(int p , int b , int a){
        this->process_id = p ;
        this->burst_time = b ;
        this->arrivalTime = a ; 
        remainingTime = b;  // Initial remaining time is the burst time
        waitingTime = 0 ;
        TAT = 0;
        completionTime = 0;
    } 
};

class SJF_pree{
    public:
    int process_id;
    int burst_time ;
    int at;
    int rt ;

    SJF_pree(int p , int b , int a){
        this->process_id = p ;
        this->burst_time =b;
        this->at = a ;
        this->rt = b ; // imp
    }

};
bool compareArrivalSJF(SJF_pree p1, SJF_pree p2) {
    return p1.at < p2.at;  // Sort by arrival time in ascending order
}



bool compareArrival(FCFS p1 , FCFS p2){
    return p1.Arrival_time < p2.Arrival_time;
}

bool compareForRR(RR p1 , RR p2){
    if(p1.arrivalTime == p2.arrivalTime){
        return p1.burst_time < p2.burst_time;
    }
    return p1.arrivalTime < p2.arrivalTime;
}

void FirstComeFirstServe(vector<FCFS>&v){

    sort(v.begin(),v.end(),compareArrival);
    int n = v.size();
    int currTime = 0 ;
    int totalwait = 0 ;
    int totalTurnaround = 0 ;

    for( auto p : v){
        int start_time = max(currTime,p.Arrival_time);

        int finishTime = start_time + p.burst_time ;
        
        int waitTime = start_time - p.Arrival_time ;

        int turnaroundTime = finishTime - p.Arrival_time ;

        totalTurnaround += turnaroundTime;
        totalwait += waitTime ;

        cout << p.process_id <<"\t"<<p.burst_time<<"\t"<<p.Arrival_time<<"\t"<<start_time<<"\t"<<finishTime
                <<"\t" << waitTime <<"\t"<<turnaroundTime <<endl;
    }

    cout <<" Average TurnAround Time :" << (double)totalTurnaround /n<< endl;
    cout <<" Average Wait Time :" << (double)totalwait/n << endl;
}

void RoundRobin( vector<RR>&v , int qunatum){
    sort(v.begin(),v.end() ,compareForRR);
    int currtime = 0 ;
    int totalwait = 0 ;
    int totalTAT = 0 ;
    int completedProcess = 0 ;

    vector<RR*>readyQueue;
    int n = v.size();

    while(completedProcess < n){

        for(int i = 0 ; i < n ; i++){
            if (find(readyQueue.begin(), readyQueue.end(), &v[i]) == readyQueue.end() && v[i].arrivalTime <= currtime && v[i].remainingTime > 0) {
                readyQueue.push_back(&v[i]);
            }
        }
        if(readyQueue.empty()){
            currtime++;
            continue;
        }

        auto first = readyQueue.front();
        readyQueue.erase(readyQueue.begin());

        if( first->remainingTime > qunatum){
            first->remainingTime -= qunatum;
            currtime += qunatum ;
            readyQueue.push_back(first);  // Re-add to the ready queue
        }
        else{
            currtime += first->remainingTime;
            first->completionTime = currtime ;
            first->TAT = first->completionTime - first->arrivalTime ;
            first->waitingTime = first->TAT - first->burst_time;
            first->remainingTime = 0;
            completedProcess++;
        }

    }
        // Output the results
    cout << "Process\tArrival Time\tBurst Time\tCompletion Time\tTAT Time\tWait Time" << endl;
    for (auto& process : v) {
        cout << process.process_id << "\t\t" << process.arrivalTime << "\t\t" << process.burst_time << "\t\t" << process.completionTime
            << "\t\t" << process.TAT << "\t\t" << process.waitingTime << endl;
    }
}

void ShortestJobFirst(vector<SJF_pree>v){
    sort(v.begin(), v.end(), compareArrivalSJF);
    int n = v.size();      // Number of processes
    int currentTime = 0;           // Current time of the CPU
    int completed = 0;             // Count of completed processes
    int totalWaitTime = 0;         // Total wait time for all processes
    int totalTurnaroundTime = 0;   // Total turnaround time for all processes

    vector<bool> isCompleted(n, false);  // Tracks whether each process is completed

    while(completed != n){
        int idx = -1 ;
        int minRemainingTime = INT_MAX;

        for(int i = 0 ; v.size() ; i++){
            if(v[i].at <= currentTime && v[i].rt > 0 && !isCompleted[i] && v[i].rt < minRemainingTime){
                minRemainingTime = v[i].rt;
                idx = i ;
            }
        }

        if(idx != -1){
            v[idx].rt--;
            currentTime++;
             // If the process is completed (remaining time is 0)
            if (v[idx].rt == 0) {
                completed++;                // Increment count of completed processes
                isCompleted[idx] = true;    // Mark process as completed
                int finishTime = currentTime; // Time at which process finished
                int turnaround = finishTime - v[idx].at; // Calculate turnaround time
                int wait = turnaround - v[idx].burst_time; // Calculate wait time

                totalWaitTime += wait;       // Add to total wait time
                totalTurnaroundTime += turnaround; // Add to total turnaround time

                // Output the results for this process
                cout << "PID: " << v[idx].process_id
                     << ", Arrival: " << v[idx].at 
                     << ", Burst: " << v[idx].burst_time 
                     << ", Finish: " << finishTime 
                     << ", Wait: " << wait 
                     << ", Turnaround: " << turnaround << endl;
            }
        }
        else {
            // If no process is available to run (all processes that have arrived are completed)
            // Increment currentTime to move forward and check for new arriving processes
            currentTime++;
        }
    } 
        // Calculate and display average wait time and turnaround time
    cout << "Average wait time: " << (double)totalWaitTime / n << endl;
    cout << "Average turnaround time: " << (double)totalTurnaroundTime / n << endl;      
}

int main(){
    int n ;
    cout << "Enter the Number of Process : ";
    cin >> n ;

    vector<FCFS>v;
    for(int i = 0 ; i < n ; i++){
        int p , b , a ;
        cout<<"Enter the Process Id , Burst time and Arrival Time for Process"<<i+1<<":";
        cin>> p >> b >> a ;
        v.push_back(FCFS(p,b,a));
    }
    FirstComeFirstServe(v);


    vector<RR> processes;
    for (int i = 0; i < n; i++) {
        int p, b, a;
        cout << "Enter Process ID, Burst Time, and Arrival Time for Process " << i + 1 << ": ";
        cin >> p >> b >> a;
        processes.push_back(RR(p, b, a));
    }

    int quantum = 2;  // You can modify this based on your needs
    RoundRobin(processes, quantum);


}