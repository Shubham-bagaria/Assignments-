#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <sstream>
using namespace std;

class RangeTree {
private:
    vector<int> arr;
    vector<int> segSum, segMin, segMax;
    int n;

    void buildSum(int idx, int l, int r) {
        if (l == r)
            segSum[idx] = arr[l];
        else {
            int mid = (l + r) / 2;
            buildSum(2 * idx, l, mid);
            buildSum(2 * idx + 1, mid + 1, r);
            segSum[idx] = segSum[2 * idx] + segSum[2 * idx + 1];
        }
    }

    void buildMin(int idx, int l, int r) {
        if (l == r)
            segMin[idx] = arr[l];
        else {
            int mid = (l + r) / 2;
            buildMin(2 * idx, l, mid);
            buildMin(2 * idx + 1, mid + 1, r);
            segMin[idx] = min(segMin[2 * idx], segMin[2 * idx + 1]);
        }
    }

    void buildMax(int idx, int l, int r) {
        if (l == r)
            segMax[idx] = arr[l];
        else {
            int mid = (l + r) / 2;
            buildMax(2 * idx, l, mid);
            buildMax(2 * idx + 1, mid + 1, r);
            segMax[idx] = max(segMax[2 * idx], segMax[2 * idx + 1]);
        }
    }

    void updateSum(int idx, int l, int r, int pos, int val) {
        if (l == r)
            segSum[idx] = val;
        else {
            int mid = (l + r) / 2;
            if (pos <= mid)
                updateSum(2 * idx, l, mid, pos, val);
            else
                updateSum(2 * idx + 1, mid + 1, r, pos, val);
            segSum[idx] = segSum[2 * idx] + segSum[2 * idx + 1];
        }
    }

    void updateMin(int idx, int l, int r, int pos, int val) {
        if (l == r)
            segMin[idx] = val;
        else {
            int mid = (l + r) / 2;
            if (pos <= mid)
                updateMin(2 * idx, l, mid, pos, val);
            else
                updateMin(2 * idx + 1, mid + 1, r, pos, val);
            segMin[idx] = min(segMin[2 * idx], segMin[2 * idx + 1]);
        }
    }

    void updateMax(int idx, int l, int r, int pos, int val) {
        if (l == r)
            segMax[idx] = val;
        else {
            int mid = (l + r) / 2;
            if (pos <= mid)
                updateMax(2 * idx, l, mid, pos, val);
            else
                updateMax(2 * idx + 1, mid + 1, r, pos, val);
            segMax[idx] = max(segMax[2 * idx], segMax[2 * idx + 1]);
        }
    }

    int querySum(int idx, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) return 0;
        if (ql <= l && r <= qr) return segSum[idx];
        int mid = (l + r) / 2;
        return querySum(2 * idx, l, mid, ql, qr) + querySum(2 * idx + 1, mid + 1, r, ql, qr);
    }

    int queryMin(int idx, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) return INT_MAX;
        if (ql <= l && r <= qr) return segMin[idx];
        int mid = (l + r) / 2;
        return min(queryMin(2 * idx, l, mid, ql, qr), queryMin(2 * idx + 1, mid + 1, r, ql, qr));
    }

    int queryMax(int idx, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) return INT_MIN;
        if (ql <= l && r <= qr) return segMax[idx];
        int mid = (l + r) / 2;
        return max(queryMax(2 * idx, l, mid, ql, qr), queryMax(2 * idx + 1, mid + 1, r, ql, qr));
    }

public:
    RangeTree(vector<int> input) {
        n = input.size();
        arr = vector<int>(n + 1);
        for (int i = 1; i <= n; ++i)
            arr[i] = input[i - 1];
        segSum.assign(4 * n, 0);
        segMin.assign(4 * n, 0);
        segMax.assign(4 * n, 0);
        buildSum(1, 1, n);
        buildMin(1, 1, n);
        buildMax(1, 1, n);
    }

    void modify(int idx, int val) {
        if (idx < 1 || idx > n) {
            cout << "!! Index out of range.\n";
            return;
        }
        arr[idx] = val;
        updateSum(1, 1, n, idx, val);
        updateMin(1, 1, n, idx, val);
        updateMax(1, 1, n, idx, val);
    }

    int getSum(int l, int r) { return querySum(1, 1, n, l, r); }
    int getMin(int l, int r) { return queryMin(1, 1, n, l, r); }
    int getMax(int l, int r) { return queryMax(1, 1, n, l, r); }

    void displayArray() {
        cout << "Current Array: ";
        for (int i = 1; i <= n; ++i) cout << arr[i] << " ";
        cout << endl;
    }
};

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> input(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; ++i) cin >> input[i];

    RangeTree seg(input);

    cout << "\nCommands: query <l> <r>, update <idx> <val>, exit\n";

    string command;
    cin.ignore();
    while (true) {
        cout << "\n> ";
        getline(cin, command);
        if (command.empty()) continue;
        stringstream ss(command);
        string cmd;
        ss >> cmd;

        if (cmd == "query") {
            int l, r;
            ss >> l >> r;
            cout << "Range [" << l << ", " << r << "]\n";
            cout << "Sum: " << seg.getSum(l, r) << endl;
            cout << "Min: " << seg.getMin(l, r) << endl;
            cout << "Max: " << seg.getMax(l, r) << endl;
        } 
        else if (cmd == "update") {
            int idx, val;
            ss >> idx >> val;
            seg.modify(idx, val);
            cout << "Updated value at index " << idx << " to " << val << endl;
            seg.displayArray();
        } 
        else if (cmd == "exit") {
            break;
        } 
        else {
            cout << "!! Invalid command. Use 'query', 'update', or 'exit'.\n";
        }
    }

    return 0;
}
