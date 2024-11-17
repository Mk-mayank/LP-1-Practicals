#include <iostream>
#include <vector>
using namespace std;

class Process {
    bool active;
public:
    int pId;

    Process(int id) : pId(id), active(true) {}

    void down() {
        active = false;
    }

    void up() {
        active = true;
    }

    bool isActive() const {
        return active;
    }
};

class BullyElection {
    int coordinatorId;
public:
    vector<Process> Processes;

    BullyElection(vector<Process>& processes) {
        this->Processes = processes;
    }

    void startElection(int initiatorId) {
        cout << "Election initiated by process: " << initiatorId << endl;
        for (int i = initiatorId + 1; i < Processes.size(); i++) {
            if (Processes[i].isActive()) {
                cout << "Process " << i << " gets an election message from " << initiatorId << endl;
            }
        }
        coordinateElection(initiatorId);
    }

    void coordinateElection(int initiatorId) {
        for (int i = Processes.size() - 1; i >= initiatorId; i--) {
            if (Processes[i].isActive()) {
                cout << "Process " << i << " becomes coordinator" << endl;
                coordinatorId = i;
                announceResult();
                return;
            }
        }
    }

    void announceResult() {
        for (const Process& pro : Processes) {
            if (pro.isActive() && pro.pId != coordinatorId) { // Skip self-acknowledgment
                cout << "Process " << pro.pId << " acknowledges process " << coordinatorId << " as coordinator" << endl;
            }
        }
    }
};

class RingElection {
    int coordinatorId;
public:
    vector<Process> process;

    RingElection(vector<Process> pro) : process(pro) {}

    void startElection(int initiatorId) {
        cout << "Election initiated by process " << initiatorId << endl;
        coordinatorId = initiatorId;

        // Wrap around the ring in a circular manner
        int i = (initiatorId + 1) % process.size();
        while (i != initiatorId) {
            if (process[i].isActive()) {
                cout << "Process " << i << " gets an election message from " << initiatorId << endl;
                coordinatorId = max(coordinatorId, i);
            }
            i = (i + 1) % process.size();  // Move to the next process in a circular manner
        }
        announceCoordinator();
    }

    void announceCoordinator() {
        cout << "Process " << coordinatorId << " becomes coordinator" << endl;
        for (const Process& pro : process) {
            if (pro.isActive() && pro.pId != coordinatorId) { // Skip self-acknowledgment
                cout << "Process " << pro.pId << " acknowledges process " << coordinatorId << " as coordinator" << endl;
            }
        }
    }
};

int main() {
    vector<Process> Processes;
    int num = 5;
    for (int i = 0; i < num; i++) {
        Processes.push_back(Process(i));
    }

    cout << "Number of Processes: " << Processes.size() << endl;

    BullyElection bully(Processes);
    bully.Processes[2].down();  // Simulate a failure of Process 2
    bully.startElection(1);     // Start Bully Election from Process 1

    cout << "\n--- Ring Election ---\n";
    RingElection ring(Processes);
    ring.process[2].down();     // Simulate a failure of Process 2
    ring.startElection(1);      // Start Ring Election from Process 1

    return 0;
}
