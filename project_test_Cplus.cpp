#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>

#ifdef _WIN32
#include <conio.h>  // Dùng cho Windows
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

struct Account {
    string username;
    string password;
    string role;
    int points;
    bool isFirstLogin;
    vector<string> transactionHistory;
};

map<string, Account> accounts;
string currentUser;

// 🔹 Hàm nhập mật khẩu ẩn (hiển thị `*`)
string getPassword() {
    string password = "";
    char ch;

#ifdef _WIN32
    while ((ch = _getch()) != '\r') { // Windows: Enter là '\r'
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((ch = getchar()) != '\n') { // Linux/macOS: Enter là '\n'
#endif
        if (ch == 8 || ch == 127) { // Xóa ký tự (Backspace)
            if (!password.empty()) {
                cout << "\b \b"; // Xóa dấu `*`
                password.pop_back();
            }
        } else {
            password += ch;
            cout << '*'; // Hiển thị dấu `*`
        }
    }

#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    cout << endl;
    return password;
}

string generateOTP() {
    srand(time(0) + rand()); // Thêm biến đổi thời gian để tránh trùng OTP
    int otp = 100000 + rand() % 900000; // Luôn đảm bảo số có 6 chữ số
    return to_string(otp);
}


bool isValidOTP(string inputOTP, string realOTP) {
    return inputOTP == realOTP;
}
void writeLog(const string& logMessage) {
    ofstream logFile("log.txt", ios::app); // Mở file ở chế độ ghi tiếp (append)
    if (logFile.is_open()) {
        time_t now = time(0);
        tm* localTime = localtime(&now);
        logFile << "[" << localTime->tm_year + 1900 << "-" 
                << localTime->tm_mon + 1 << "-"
                << localTime->tm_mday << " "
                << localTime->tm_hour << ":"
                << localTime->tm_min << ":"
                << localTime->tm_sec << "] "
                << logMessage << endl;
        logFile.close();
    } else {
        cout << "Error: Unable to write log file!" << endl;
    }
}
void saveAccountsToCSV() {
    ofstream file("accounts.csv");
    if (file.is_open()) {
        file << "username,password,role,points,isFirstLogin,history\n";
        for (auto& account : accounts) {
            file << account.second.username << ","
                 << account.second.password << ","
                 << account.second.role << ","
                 << account.second.points << ","
                 << account.second.isFirstLogin << ",";

            // Ghi lịch sử giao dịch vào file (cách nhau bởi `|`)
            for (const string& history : account.second.transactionHistory) {
                file << history << "|";
            }
            file << endl;
        }
        file.close();
    } else {
        cout << "Unable to open file for saving accounts!" << endl;
    }
}


void loadAccountsFromCSV() {
    ifstream file("accounts.csv");
    string line;
    if (file.is_open()) {
        getline(file, line); // Bỏ qua tiêu đề
        while (getline(file, line)) {
            stringstream ss(line);
            Account acc;
            string points, isFirstLogin, history;
            getline(ss, acc.username, ',');
            getline(ss, acc.password, ',');
            getline(ss, acc.role, ',');
            getline(ss, points, ',');
            getline(ss, isFirstLogin, ',');
            getline(ss, history, ','); // Lịch sử giao dịch

            acc.points = stoi(points);
            acc.isFirstLogin = stoi(isFirstLogin);

            // Tách các giao dịch từ chuỗi `history`
            stringstream historyStream(history);
            string transaction;
            while (getline(historyStream, transaction, '|')) {
                acc.transactionHistory.push_back(transaction);
            }

            accounts[acc.username] = acc;
        }
        file.close();
    } else {
        cout << ">>> Unable to open file for loading accounts!" << endl;
    }
}


void registerAccount() {
    string username, password, role;
    int initialPoints = 0;

    cout << "\nEnter username: ";
    cin >> username;

    if (accounts.find(username) != accounts.end()) {
        cout << ">>> Username already exists!" << endl;
        return;
    }

    cout << "Enter role (user/administrator): ";
    cin >> role;

    if (role != "user" && role != "administrator") {
        cout << ">>> Invalid role! Role must be 'user' or 'administrator'.\n";
        return;
    }

    // Nếu là admin, cho phép nhập số điểm khởi tạo
    if (currentUser != "" && accounts[currentUser].role == "administrator") {
        cout << "Enter initial points for this user: ";
        cin >> initialPoints;
    } else {
        // Nếu là user tự đăng ký, đặt điểm mặc định
        initialPoints = (role == "user") ? 100 : 500;
    }

    password = generateOTP();
    cout << ">>> Generated password (first-time login required): " << password << endl;

    Account newAccount;
    newAccount.username = username;
    newAccount.password = password;
    newAccount.role = role;
    newAccount.points = initialPoints;
    newAccount.isFirstLogin = true;

    accounts[username] = newAccount;
    saveAccountsToCSV();
    writeLog("New account created: " + username + " (Role: " + role + ")");
    cout << "/----- Account registered successfully! -----/\n";
}

void changePassword() {
    string newPassword, confirmPassword;

    // Xác thực bằng OTP
    string otp = generateOTP();
    cout << ">>> OTP sent to your email/phone: " << otp << endl;

    string inputOTP;
    cout << ">>> Enter OTP to proceed: ";
    cin >> inputOTP;

    if (!isValidOTP(inputOTP, otp)) {
        cout << ">>> Invalid OTP! Password change aborted." << endl;
        currentUser = "";  // 🛑 Nếu OTP sai, logout ngay lập tức!
        return;
    }

    while (true) {
        cout << ">>> Enter new password: ";
        newPassword = getPassword();
        cout << ">>> Confirm new password: ";
        confirmPassword = getPassword();

        if (newPassword == confirmPassword) {
            accounts[currentUser].password = newPassword;
            accounts[currentUser].isFirstLogin = false;
            saveAccountsToCSV();
            cout << "\n>>> /----- Password changed successfully! -----/\n";

            // 🛑 Không tự động login lại, bắt buộc quay về màn hình chính
            cout << ">>> You have been logged out. Please login again.\n";
            currentUser = "";  // 🛑 Logout ngay
            return;
        } else {
            cout << ">>> Passwords do not match! Try again." << endl;
        }
    }
}


void viewAccountInfo() {
    Account& acc = accounts[currentUser];
    cout << "\n/----- Account Info -----/" << endl;
    cout << "Username: " << acc.username << endl;
    cout << "Role: " << acc.role << endl;
    cout << "Points: " << acc.points << endl;

    // 🛑 Hiển thị lịch sử giao dịch
    cout << "\n/----- Transaction History -----/\n";
    if (acc.transactionHistory.empty()) {
        cout << ">>> No transactions yet.\n";
    } else {
        for (const string& history : acc.transactionHistory) {
            cout << "- " << history << endl;
        }
    }
}


void transferPoints() {
    string toUsername;
    int amount;
    cout << "Enter recipient username: ";
    cin >> toUsername;

    if (accounts.find(toUsername) == accounts.end()) {
        cout << ">>> Recipient not found!" << endl;
        return;
    }

    cout << "Enter amount to transfer: ";
    cin >> amount;

    if (accounts[currentUser].points >= amount) {
        string otp = generateOTP();
        cout << "OTP: " << otp << endl;

        string inputOTP;
        cout << "Enter OTP to confirm transaction: ";
        cin >> inputOTP;

        if (isValidOTP(inputOTP, otp)) {
            accounts[currentUser].points -= amount;
            accounts[toUsername].points += amount;

            // 🛑 Ghi lịch sử giao dịch cho người gửi
            string senderHistory = "Transferred " + to_string(amount) + " points to " + toUsername;
            accounts[currentUser].transactionHistory.push_back(senderHistory);

            // 🛑 Ghi lịch sử giao dịch cho người nhận
            string receiverHistory = "Received " + to_string(amount) + " points from " + currentUser;
            accounts[toUsername].transactionHistory.push_back(receiverHistory);

            saveAccountsToCSV();

            writeLog("Transaction: " + currentUser + " transferred " + to_string(amount) + " points to " + toUsername);
            cout << ">>> Transaction successful!" << endl;
        } else {
            cout << ">>> Invalid OTP! Transaction failed." << endl;
        }
    } else {
        cout << ">>> Insufficient balance!" << endl;
    }
}

void deleteUser() {
    string username;
    cout << "Enter username to delete: ";
    cin >> username;

    if (accounts.find(username) == accounts.end()) {
        cout << ">>> User not found!\n";
        return;
    }

    if (username == currentUser) {
        cout << ">>> You cannot delete your own account!\n";
        return;
    }

    cout << ">>> Do you want to remove this user? Yes [y] or No [n]: ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        accounts.erase(username);

        // 🛑 Ghi lịch sử giao dịch
        string history = ">>> Deleted user: " + username;
        accounts[currentUser].transactionHistory.push_back(history);

        saveAccountsToCSV();
        cout << ">>> User deleted successfully!\n";
    } else {
        cout << ">>> User deletion canceled.\n";
    }
}


void editUserRole() {
    string username;
    cout << "Enter username to edit role: ";
    cin >> username;

    if (accounts.find(username) == accounts.end()) {
        cout << ">>> User not found!\n";
        return;
    }

    if (username == currentUser) {
        cout << ">>> You cannot change your own role!\n";
        return;
    }

    string newRole;
    cout << "Enter new role (user/administrator): ";
    cin >> newRole;

    if (newRole != "user" && newRole != "administrator") {
        cout << ">>> Invalid role! Role must be 'user' or 'administrator'.\n";
        return;
    }

    string otp = generateOTP();
    cout << "OTP sent: " << otp << "\nEnter OTP to confirm: ";
    string inputOTP;
    cin >> inputOTP;

    if (!isValidOTP(inputOTP, otp)) {
        cout << ">>> Invalid OTP! Role change aborted.\n";
        return;
    }

    accounts[username].role = newRole;

    // 🛑 Ghi lịch sử giao dịch
    string history = "Changed role of " + username + " to " + newRole;
    accounts[currentUser].transactionHistory.push_back(history);

    saveAccountsToCSV();

    writeLog("User " + currentUser + " changed role of " + username + " to " + newRole);
    cout << ">>> Role updated successfully!\n";
}

void deleteAllUsers() {
    cout << ">>> WARNING: This will delete all users except the currently logged-in administrator.\n";
    cout << ">>> Do you want to continue? Yes [y] or No [n]: ";
    char confirm;
    cin >> confirm;

    if (confirm != 'y' && confirm != 'Y') {
        cout << ">>> Delete All Users canceled.\n";
        return;
    }

    int deletedCount = 0;
    for (auto it = accounts.begin(); it != accounts.end(); ) {
        if (it->first != currentUser) { // Không xóa chính mình
            it = accounts.erase(it);
            deletedCount++;
        } else {
            ++it;
        }
    }

    // 🛑 Ghi lịch sử giao dịch
    string history = ">>> Deleted all users (except self)";
    accounts[currentUser].transactionHistory.push_back(history);

    saveAccountsToCSV();
    cout << ">>> Successfully deleted " << deletedCount << " users!\n";
}

void manageUsers() {
    while (true) {
        cout << "\n/----- User List -----/\n";
        for (const auto& account : accounts) {
            cout << "Username: " << account.second.username << " | Role: " << account.second.role << "\n";
        }

        cout << "\n1. Edit Permission\n2. Delete User\n3. Delete All Users\n4. Exit\n\n>>> Enter your choice: ";
        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << ">>> Invalid input! Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            editUserRole();
        } else if (choice == 2) {
            deleteUser();
        } else if (choice == 3) {
            deleteAllUsers();
        } else if (choice == 4) {
            return;
        } else {
            cout << ">>> Invalid choice! Please enter a number between 1-4.\n";
        }
    }
}


void adminMenu() {
    while (!currentUser.empty()) {
        int choice;
        cout << "\n/----- Administrator Menu -----/\n";
        cout << "1. View My Account\n2. View User List\n3. Change Password\n4. Transfer Points\n5. Logout\n\n>>> Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << ">>> Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                viewAccountInfo();
                break;
            case 2:
                manageUsers();
                break;
            case 3:
                changePassword();
                if (currentUser.empty()) return; // Nếu admin đổi mật khẩu và bị logout, thoát ngay
                break;
            case 4:
                transferPoints();
                break;
            case 5:
                cout << ">>> Logging out...\n";
                currentUser = "";
                return;
            default:
                cout << ">>> Invalid choice! Please enter a number between 1-5.\n";
        }
    }
}

void userMenu() {
    while (!currentUser.empty()) {
        if (accounts[currentUser].role == "administrator") {
            adminMenu();  // 🛑 Nếu là admin, vào adminMenu
            return;
        }

        int choice;
        cout << "\n1. View Account\n2. Change Password\n3. Transfer Points\n4. Logout\n\n>>> Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                viewAccountInfo();
                break;
            case 2:
                changePassword();
                if (currentUser.empty()) return;
                break;
            case 3:
                transferPoints();
                break;
            case 4:
                cout << ">>> Logging out...\n";
                currentUser = "";
                return;
            default:
                cout << ">>> Invalid choice! Please enter a number between 1-4.\n";
        }
    }
}
bool login() {
    string username, password;
    int attempts = 0;

    while (attempts < 3) {
        cout << "\nEnter username: ";
        cin >> username;

        if (accounts.find(username) == accounts.end()) {
            cout << "Account not found!" << endl;
            return false;
        }

        cout << "Enter password: ";
        password = getPassword();

        if (accounts[username].password == password) {
            currentUser = username;

            // 🔹 Kiểm tra nếu là lần đầu đăng nhập, bắt buộc đổi mật khẩu
            if (accounts[username].isFirstLogin) {
                cout << ">>> First-time login detected. You must change your password!" << endl;
                changePassword();

                // 🛑 Nếu người dùng đã logout sau khi đổi mật khẩu, thoát ngay lập tức
                if (currentUser.empty()) {
                    return false;  // Không in "Login successful!"
                }
            }
            writeLog("User " + username + " logged in.");
            return true; // 🟢 Chỉ in "Login successful!" nếu vẫn đăng nhập
        } else {
            cout << ">>> Incorrect password!" << endl;
            attempts++;
        }
    }

    cout << ">>> Too many incorrect attempts. Exiting program." << endl;
    exit(0);
}
int main() {
    loadAccountsFromCSV();

    string choice;
    while (true) {
        cout << "\n/----- Welcome to our Program v.1.0  -----/" << endl;
        cout << "\n1. Register\n2. Login\n3. Exit\n\n>>> Enter your choice: ";
        cin >> choice;

        if (choice == "1") {
            registerAccount();
        } else if (choice == "2") {
            if (login()) {
                cout << ">>> Login successful!" << endl;
                userMenu();
            } else {
                cout << ">>> Login failed!" << endl;
            }
        } else if (choice == "3") {
            cout << ">>> Exiting program...\n";
            break;
        } else {
            cout << ">>> Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
