#include <iostream>
#include <limits>
#include <vector>

// Process Id's are just sequential numbers from 0 to n.
int main() {
    using namespace std;
    [[maybe_unused]] int improving{0};

    int size{};
    cout << "Enter the total count of processes: ";

    // handle bad input
    while (true) {
        cin >> size;
        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }

    vector<int> at(size);
    [[maybe_unused]] float av_arival_time{};
    cout << "\nEnter arival time.\n";
    for (int i{}; i < size; ++i) {
        cout << "Pid " << i + 1 << ": ";
        cin >> at[i];
        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            --i;
            continue;
        }
        av_arival_time += at[i];
    }

    vector<int> bt(size);
    [[maybe_unused]] float av_brust_time{};
    cout << "\nEnter burst time.\n";
    for (int i{}; i < size; ++i) {
        cout << "Pid " << i + 1 << ": ";
        cin >> bt[i];
        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            --i;
            continue;
        }
        av_brust_time += bt[i];
    }

    // Sorting at and bt in ascending order based on at
    int smallest{};
    for (int i{}; i < size; ++i) {
        smallest = i;
        for (int j{i}; j < size; ++j) {
            if (at[j] < at[smallest]) {
                smallest = j;
            }
        }
        if (i != smallest) {
            swap(at[i], at[smallest]);
            swap(bt[i], bt[smallest]);
        }
    }

#if improving
    int smallest_at{};
    for (int i{}; i < size; ++i) {
        if (at[i] < smallest_at) {
            smallest_at = i;
        }
    }
    swap(at[smallest_at], at[0]);
    swap(bt[smallest_at], bt[0]);
#endif

    int smallest_bt{};
    int ct_temp{at[0]};
    for (int i{}; i < size; ++i) {
        smallest_bt = i;
        for (int j{i}; j < size; ++j) {
            if (at[j] <= ct_temp) {
                if (bt[j] < bt[smallest_bt]) {
                    smallest_bt = j;
                }
            }
        }
        if (i != smallest_bt) {
            swap(at[i], at[smallest_bt]);
            swap(bt[i], bt[smallest_bt]);
        }

        if (at[i] > ct_temp) {
            ct_temp += at[i] - ct_temp; // Running on ideal
        }
        ct_temp += bt[i]; // running the process
    }

    // for (int i{}; i < size; ++i) {
    //     cout << "at: " << at[i] << "\tbt: " << bt[i] << "\n";
    // }

#if 1
    // Arival times are sorted so we can just run all the processes form top to bottom
    int ct{};
    float av_cycle_time{};
    vector<int> cycle_time(size);
    vector<bool> used_process(size, false);

    for (int i{}; i < size; ++i) {
        if (at[i] > ct) {
            ct += at[i] - ct; // Running on ideal
        }
        ct += bt[i]; // running the process
        cycle_time[i] = ct;
        av_cycle_time += cycle_time[i];
    }

    vector<int> tat(size);
    float av_turn_around_time{};
    for (int i{}; i < size; ++i) {
        tat[i] = cycle_time[i] - at[i];
        av_turn_around_time += tat[i];
    }

    vector<int> wt(size);
    float av_wating_time{};
    for (int i{}; i < size; ++i) {
        wt[i] = tat[i] - bt[i];
        av_wating_time += wt[i];
    }

    cout << "\nResults: \n";
    for (int i{}; i < size; ++i) {
        cout << "at: " << at[i] << "\t\tbt: " << bt[i] << "\t\tct: " << cycle_time[i]
             << "\t\ttat: " << tat[i] << "\t\twt: " << wt[i] << "\n";
    }
    av_wating_time /= size;
    av_cycle_time /= size;
    av_turn_around_time /= size;
    av_arival_time /= size;
    av_brust_time /= size;

    cout << "\nAverage times: " << "\n\t| arival: " << av_arival_time
         << "\n\t| brust: " << av_brust_time << "\n\t| cycle: " << av_cycle_time
         << "\n\t| tat: " << av_turn_around_time << "\n\t| wating: " << av_wating_time << "\n";
#endif
}
