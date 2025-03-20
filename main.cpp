#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <limits>
#include <algorithm> // Để sử dụng std::sort
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
    string createdAt;
    string firstLoginAt; // Ghi lại thời gian đăng nhập đầu tiên
    vector<string> transactionHistory;
};
string getCurrentTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    stringstream ss;
    ss << localTime->tm_year + 1900 << "-"
       << localTime->tm_mon + 1 << "-"
       << localTime->tm_mday << " "
       << localTime->tm_hour << ":"
       << localTime->tm_min << ":"
       << localTime->tm_sec;
    return ss.str();
}

// Khai báo biến toàn cục
map<string, Account> accounts;
string currentUser;

//Hàm nhập mật khẩu ẩn (hiển thị *)
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
                cout << "\b \b"; // Xóa dấu *
                password.pop_back();
            }
        } else {
            password += ch;
            cout << '*'; // Hiển thị dấu * khi nhập mật khẩukhẩu
        }
    }

#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    cout << endl;
    return password;
}

// Ví tổng với số điểm ban đầu
int totalWalletPoints = 1000000; // 1,000,000 điểm cho ví tổng

// Cập nhật vào CSV cho ví tổng
void saveTotalWalletToCSV() {
    ofstream file("total_wallet.csv");
    if (file.is_open()) {
        file << "totalPoints\n";
        file << totalWalletPoints << endl;
        file.close();
    } else {
        cout << "Unable to open file for saving total wallet data!" << endl;
    }
}

// Đọc thông tin ví tổng từ file CSV
void loadTotalWalletFromCSV() {
    ifstream file("total_wallet.csv");
    if (file.is_open()) {
        string line;
        if (getline(file, line)) {  // Đọc tiêu đề (nếu có)
            if (getline(file, line)) {  // Đọc dữ liệu ví tổng
                try {
                    totalWalletPoints = stoi(line);  // Chuyển đổi giá trị từ chuỗi sang int
                } catch (const std::invalid_argument& e) {
                    cout << ">>> Invalid data in total_wallet.csv. Using default value.\n";
                    totalWalletPoints = 1000000;  // Thiết lập giá trị mặc định là 1,000,000 điểm
                }
            } else {
                cout << ">>> total_wallet.csv is empty. Using default value.\n";
                totalWalletPoints = 1000000;  // Nếu không có dữ liệu, thiết lập giá trị mặc định
            }
        } else {
            cout << ">>> Unable to read total_wallet.csv. Using default value.\n";
            totalWalletPoints = 1000000;  // Nếu file không có tiêu đề, thiết lập giá trị mặc định
        }
        file.close();
    } else {
        cout << ">>> Unable to open total_wallet.csv. Using default value.\n";
        totalWalletPoints = 1000000;  // Nếu không thể mở file, thiết lập giá trị mặc định
    }
}

// Lưu giao dịch vào file cho ví tổng
void saveTotalWalletTransactionToCSV(const string& transactionDetails, int debit, int credit, int balance) {
    bool fileExists = ifstream("total_wallet_transactions.csv").good(); // Kiểm tra file tồn tại

    ofstream file("total_wallet_transactions.csv", ios::app);
    if (file.is_open()) {
        // Ghi tiêu đề nếu file chưa có
        if (!fileExists) {
            file << "Date,Time,Transaction Details,Debit,Credit,Balance\n";
        }

        // Lấy thời gian hiện tại
        time_t now = time(0);
        tm* localTime = localtime(&now);

        file << localTime->tm_year + 1900 << "-" 
             << localTime->tm_mon + 1 << "-" 
             << localTime->tm_mday << "," 
             << localTime->tm_hour << ":" 
             << localTime->tm_min << ":" 
             << localTime->tm_sec << "," 
             << transactionDetails << "," 
             << (debit > 0 ? to_string(debit) : "") << ","  // Ghi giá trị Debit nếu có
             << (credit > 0 ? to_string(credit) : "") << "," // Ghi giá trị Credit nếu có
             << balance 
             << endl;

        file.close();
    } else {
        cout << "Error: Unable to save total wallet transaction history!" << endl;
    }
}

void sortTotalWalletTransactionsCSV() {
    ifstream file("total_wallet_transactions.csv");
    vector<vector<string>> rows;
    string line;

    if (file.is_open()) {
        getline(file, line); // Đọc tiêu đề
        string header = line;

        while (getline(file, line)) {
            stringstream ss(line);
            vector<string> row;
            string cell;

            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            rows.push_back(row);
        }
        file.close();

        // Sắp xếp theo cột `Date` (cột thứ 1, chỉ số 0) và `Time` (cột thứ 2, chỉ số 1)
        sort(rows.begin(), rows.end(), [](const vector<string>& a, const vector<string>& b) {
            if (a[0] == b[0]) {
                return a[1] < b[1]; // Nếu ngày giống nhau, so sánh thời gian
            }
            return a[0] < b[0]; // So sánh ngày
        });

        // Ghi lại dữ liệu đã sắp xếp vào file
        ofstream outFile("total_wallet_transactions.csv");
        if (outFile.is_open()) {
            outFile << header << endl; // Ghi tiêu đề
            for (const auto& row : rows) {
                for (size_t i = 0; i < row.size(); ++i) {
                    outFile << row[i];
                    if (i < row.size() - 1) outFile << ",";
                }
                outFile << endl;
            }
            outFile.close();
        }
    } else {
        cout << ">>> Unable to open total_wallet_transactions.csv for sorting!" << endl;
    }
}

// Hàm tạo mã OTP ngẫu nhiên
string generateOTP() {
    srand(time(0) + rand()); // Thêm biến đổi thời gian để tránh trùng OTP
    int otp = 100000 + rand() % 900000; // Luôn đảm bảo số có 6 chữ số
    return to_string(otp);
}

// Hàm kiểm tra OTP nhập vào có đúng không
bool isValidOTP(string inputOTP, string realOTP) {
    return inputOTP == realOTP;
}

// Ghi log vào file
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

// Lưu thông tin tài khoản vào file CSV
void saveAccountsToCSV() {
    ofstream file("accounts.csv");
    if (file.is_open()) {
        file << "username,password,role,points,isFirstLogin,createdAt,firstLoginAt\n";
        for (auto& account : accounts) {
            file << account.second.username << ","
                 << account.second.password << ","
                 << account.second.role << ","
                 << account.second.points << ","
                 << account.second.isFirstLogin << ","
                 << account.second.createdAt << ","
                 << (account.second.firstLoginAt.empty() ? "N/A" : account.second.firstLoginAt)
                 << endl;
        }
        file.close();
    } else {
        cout << "Unable to open file for saving accounts!" << endl;
    }
}

void createRootAccountIfNeeded() {
    bool rootAccountExists = false;

    // Kiểm tra xem tài khoản root có tồn tại không
    for (const auto& account : accounts) {
        if (account.second.username == "root") {
            rootAccountExists = true;
            break;
        }
    }

    // Nếu không có tài khoản root, tạo tài khoản root
    if (!rootAccountExists) {
        Account rootAccount;
        rootAccount.username = "root";
        rootAccount.password = "root";
        rootAccount.role = "administrator";  // root luôn luôn là administrator
        rootAccount.points = 1000000;  // Cấp cho root số điểm lớn cho ví tổng
        rootAccount.isFirstLogin = false;
        rootAccount.createdAt = getCurrentTime();
        rootAccount.firstLoginAt = getCurrentTime(); // Root không cần thay đổi mật khẩu

        accounts["root"] = rootAccount;
        saveAccountsToCSV();  // Lưu tài khoản root vào file CSV
        cout << ">>> Root account created successfully!\n";
    }
}
// Lưu thông tin giao dịch vào file CSV
void saveTransactionToCSV(const string& username, const string& transactionDetails, int debit, int credit, int balance) {
    bool fileExists = ifstream("transactions.csv").good(); // Kiểm tra file tồn tại

    ofstream file("transactions.csv", ios::app);
    if (file.is_open()) {
        // Ghi tiêu đề nếu file chưa có
        if (!fileExists) {
            file << "Date,Time,Username,Transaction Details,Debit,Credit,Balance\n";
        }

        // Lấy thời gian hiện tại
        time_t now = time(0);
        tm* localTime = localtime(&now);

        file << localTime->tm_year + 1900 << "-" 
             << localTime->tm_mon + 1 << "-" 
             << localTime->tm_mday << "," 
             << localTime->tm_hour << ":" 
             << localTime->tm_min << ":" 
             << localTime->tm_sec << "," 
             << username << "," 
             << transactionDetails << "," 
             << (debit > 0 ? to_string(debit) : "") << ","  // Ghi giá trị Debit nếu có
             << (credit > 0 ? to_string(credit) : "") << "," // Ghi giá trị Credit nếu có
             << balance 
             << endl;

        file.close();
    } else {
        cout << "Error: Unable to save transaction history!" << endl;
    }
}

// Đọc thông tin tài khoản từ file CSV
void loadAccountsFromCSV() {
    ifstream file("accounts.csv");
    string line;
    
    if (file.is_open()) {
        getline(file, line); // Bỏ qua dòng tiêu đề
        while (getline(file, line)) {
            stringstream ss(line);
            Account acc;
            string points, isFirstLogin, createdAt, firstLoginAt;

            getline(ss, acc.username, ',');
            getline(ss, acc.password, ',');
            getline(ss, acc.role, ',');
            getline(ss, points, ',');
            getline(ss, isFirstLogin, ',');
            getline(ss, createdAt, ',');
            getline(ss, firstLoginAt, ',');

            acc.points = stoi(points);
            acc.isFirstLogin = stoi(isFirstLogin);
            acc.createdAt = createdAt;
            acc.firstLoginAt = (firstLoginAt == "N/A" ? "" : firstLoginAt);

            accounts[acc.username] = acc;
        }
        file.close();
    } else {
        cout << ">>> Unable to open file for loading accounts!" << endl;
    }
    createRootAccountIfNeeded();  // Tạo tài khoản root nếu chưa tồn tại
}
#include <algorithm> // Để sử dụng std::sort

void sortAccountsCSV() {
    ifstream file("accounts.csv");
    vector<vector<string>> rows;
    string line;

    if (file.is_open()) {
        getline(file, line); // Đọc tiêu đề
        string header = line;

        while (getline(file, line)) {
            stringstream ss(line);
            vector<string> row;
            string cell;

            while (getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            rows.push_back(row);
        }
        file.close();

        // Sắp xếp theo cột `createdAt` (cột thứ 6, chỉ số 5)
        sort(rows.begin(), rows.end(), [](const vector<string>& a, const vector<string>& b) {
            return a[5] < b[5]; // So sánh cột `createdAt`
        });

        // Ghi lại dữ liệu đã sắp xếp vào file
        ofstream outFile("accounts.csv");
        if (outFile.is_open()) {
            outFile << header << endl; // Ghi tiêu đề
            for (const auto& row : rows) {
                for (size_t i = 0; i < row.size(); ++i) {
                    outFile << row[i];
                    if (i < row.size() - 1) outFile << ",";
                }
                outFile << endl;
            }
            outFile.close();
        }
    } else {
        cout << ">>> Unable to open accounts.csv for sorting!" << endl;
    }
}
// Đăng ký tài khoản
void registerAccount() {
    string username, password, role;
    int initialPoints = 0;
    string roleInput;

    // Yêu cầu nhập username, không cho phép để trống hoặc trùng lặp
    while (true) {
        cout << "-- Information registration --\nEnter username: ";
        getline(cin, username);

        if (username.empty()) {
            cout << ">>> Username cannot be empty! Please enter again.\n";
        } else if (accounts.find(username) != accounts.end()) {
            cout << ">>> Username already exists! Try another one.\n";
        } else {
            break; // ✅ Hợp lệ, thoát vòng lặp
        }
    }

    // Yêu cầu nhập role (1 hoặc 2)
    while (true) {
        cout << "Enter role (1: User, 2: Administrator): ";
        getline(cin, roleInput);

        if (roleInput == "1") {
            role = "user";
            break;
        } else if (roleInput == "2") {
            role = "administrator";
            break;
        } else {
            cout << ">>> Invalid role! Please enter '1' for User or '2' for Administrator.\n";
        }
    }

    initialPoints = (role == "user") ? 100000 : 500000;
    
    // Kiểm tra nếu có đủ điểm trong ví tổng để cấp cho người dùng
    if (totalWalletPoints < initialPoints) {
        cout << ">>> Not enough points in the total wallet to register this account.\n";
        return;
    }

    // Giảm điểm từ ví tổng
    totalWalletPoints -= initialPoints;
    
    // 🔹 Cập nhật số điểm của tài khoản root
    if (accounts.find("root") != accounts.end()) {
        accounts["root"].points = totalWalletPoints;
    }
    // Ensure total wallet points do not go below 0
    if (totalWalletPoints < 0) {
        cout << ">>> Error: Total wallet points cannot be negative! Registration failed.\n";
        totalWalletPoints += initialPoints;  // Rollback the deduction
        return;
    }

    saveTotalWalletToCSV();  // Cập nhật lại thông tin ví tổng
    
    password = generateOTP();
    cout << ">>> Generated password (first-time login required): " << password << endl;

    Account newAccount;
    newAccount.username = username;
    newAccount.password = password;
    newAccount.role = role;
    newAccount.points = initialPoints;
    newAccount.isFirstLogin = true;
    newAccount.createdAt = getCurrentTime();
    newAccount.firstLoginAt = "";

    accounts[username] = newAccount;
    
    // Save the account data to CSV
    saveAccountsToCSV();  // This will now update the accounts.csv file with the new account

    writeLog("New account created: " + username + " (Role: " + role + ")");

    // Ghi lại giao dịch của ví tổng
    saveTotalWalletTransactionToCSV("Points allocated to " + username, initialPoints, 0, totalWalletPoints);

    cout << ">>> Account registered successfully!\n";
}

// Chức năng thay đổi mật khẩu
void changePassword() {
    string newPassword, confirmPassword;
    int attempts = 0;  // Số lần nhập OTP sai
    
    // Xác thực bằng OTP
    string otp = generateOTP();
    cout << ">>> OTP sent to your email/phone: " << otp << endl;

    string inputOTP;
    
    while (attempts < 3) {  // Giới hạn tối đa 3 lần nhập OTP
        cout << ">>> Enter OTP to proceed: ";
        getline(cin, inputOTP);

        if (inputOTP.empty()) {
            attempts++;  // Đếm số lần nhập rỗng
            cout << ">>> OTP cannot be empty! You have " << (3 - attempts) << " attempts left.\n";
            continue;
        }

        if (isValidOTP(inputOTP, otp)) {
            break;  // Thoát vòng lặp nếu OTP hợp lệ
        } else {
            attempts++;
            cout << ">>> Invalid OTP! You have " << (3 - attempts) << " attempts left.\n";
        }
    }

    if (attempts == 3) {
        cout << ">>> Too many failed OTP attempts! You have been logged out.\n";
        currentUser = "";  // 🔹 Đăng xuất user để bảo mật
        return;
    }

    while (true) {
        cout << ">>> Enter new password: ";
        getline(cin, newPassword);

        if (newPassword.empty()) {
            cout << ">>> Password cannot be empty! Please enter again.\n";
            continue;
        }

        cout << ">>> Confirm new password: ";
        getline(cin, confirmPassword);

        if (newPassword == confirmPassword) {
            accounts[currentUser].password = newPassword;
            accounts[currentUser].isFirstLogin = false;
            saveAccountsToCSV();
            cout << "\n>>> Password changed successfully!\n";

            // Đăng xuất để buộc user đăng nhập lại với mật khẩu mới
            cout << ">>> You have been logged out. Please login again.\n";
            currentUser = "";
            return;
        } else {
            cout << ">>> Passwords do not match! Try again.\n";
        }
    }
}

// Xem thông tin tài khoản
void viewAccountInfo() {
    Account& acc = accounts[currentUser];
    cout << "\n-- Account Info --" << endl;
    cout << "Username: " << acc.username << endl;
    cout << "Role: " << acc.role << endl;
    cout << "Points: " << acc.points << endl;

    // Hiển thị lịch sử giao dịch
    cout << "\n--Transaction History --\n";
    if (acc.transactionHistory.empty()) {
        cout << ">>> No transactions yet.\n";
    } else {
        for (const string& history : acc.transactionHistory) {
            cout << "- " << history << endl;
        }
    }
}

// Chức năng chuyển điểm
void transferPoints() {
    string toUsername;
    int amount = 0;
    
    // Xóa bộ đệm trước khi sử dụng getline
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        cout << "Enter recipient username: ";
        getline(cin, toUsername);

        // Check if the username is empty first
        if (toUsername.empty()) {
            cout << ">>> Username cannot be empty! Please enter again.\n";
        } 
        // Check if the user is trying to transfer to themselves
        else if (toUsername == currentUser) {
            cout << ">>> You cannot transfer points to yourself!\n";
        } 
        // Check if the user is trying to transfer to the root account
        else if (toUsername == "root") {
            cout << ">>> You cannot transfer points to the root account!\n";
        } 
        // Check if the recipient exists in the accounts map
        else if (accounts.find(toUsername) == accounts.end()) {
            cout << ">>> Recipient not found! Please enter a valid username.\n";
        } 
        // If everything is valid, exit the loop
        else {
            break;
        }
    }

    // Proceed with amount validation
    while (true) {
        cout << "Enter amount to transfer: ";
        string amountInput;
        getline(cin, amountInput);

        if (amountInput.empty() || amountInput.find_first_not_of("0123456789") != string::npos) {
            cout << ">>> Invalid amount! Please enter a valid number.\n";
            continue;
        }

        amount = stoi(amountInput);
        if (amount <= 0) {
            cout << ">>> Amount must be greater than zero! Please try again.\n";
            continue;
        }

        if (accounts[currentUser].points < amount) {
            cout << ">>> Insufficient balance! You only have " << accounts[currentUser].points << " points.\n";
            continue;
        }

        break;
    }

    string otp = generateOTP();
    cout << "OTP: " << otp << endl;

    string inputOTP;
    cout << "Enter OTP to confirm transaction: ";
    getline(cin, inputOTP);

    if (!isValidOTP(inputOTP, otp)) {
        cout << ">>> Invalid OTP! Transaction failed.\n";
        return;
    }

    // Perform the transaction
    accounts[currentUser].points -= amount;
    accounts[toUsername].points += amount;

    // Save transaction to CSV
    saveTransactionToCSV(currentUser, "Transferred to " + toUsername, amount, 0, accounts[currentUser].points);
    saveTransactionToCSV(toUsername, "Received from " + currentUser, 0, amount, accounts[toUsername].points);

    saveAccountsToCSV();
    writeLog("Transaction: " + currentUser + " transferred " + to_string(amount) + " points to " + toUsername);
    cout << ">>> Transaction successful!" << endl;
}

// Xóa tài khoản
void deleteUser() {
    string username;

    while (true) {
        cout << "Enter username to delete: ";
        getline(cin, username);

        if (username.empty()) {
            cout << ">>> Username cannot be empty! Please enter again.\n";
            continue;
        }

        if (accounts.find(username) == accounts.end()) {
            cout << ">>> User not found! Please enter a valid username.\n";
            continue;
        }

        if (username == currentUser) {
            cout << ">>> You cannot delete your own account!\n";
            return;
        }

        break;
    }

    // Xác nhận OTP trước khi xóa
    string otp = generateOTP();
    cout << "OTP sent: " << otp << "\nEnter OTP to confirm deletion: ";
    string inputOTP;
    getline(cin, inputOTP);

    if (!isValidOTP(inputOTP, otp)) {
        cout << ">>> Invalid OTP! Deletion aborted.\n";
        return;
    }

    accounts.erase(username);

    // Ghi lịch sử giao dịch
    string history = "Deleted user: " + username;
    accounts[currentUser].transactionHistory.push_back(history);

    saveAccountsToCSV();
    cout << ">>> User deleted successfully!\n";
}

// Chỉnh sửa quyền người dùng
void editUserRole() {
    string username;

    while (true) {
        cout << "Enter username to edit role: ";
        getline(cin, username);

        if (username.empty()) {
            cout << ">>> Username cannot be empty! Please enter again.\n";
            continue;
        }

        if (accounts.find(username) == accounts.end()) {
            cout << ">>> User not found! Please enter a valid username.\n";
            continue;
        }

        if (username == currentUser) {
            cout << ">>> You cannot change your own role!\n";
            return;
        }

        break;
    }

    string newRole;
    while (true) {
        cout << "Enter new role (user/administrator): ";
        getline(cin, newRole);

        if (newRole != "user" && newRole != "administrator") {
            cout << ">>> Invalid role! Role must be 'user' or 'administrator'. Please enter again.\n";
            continue;
        }

        break;
    }

    string otp = generateOTP();
    cout << "OTP sent: " << otp << "\nEnter OTP to confirm: ";
    string inputOTP;
    getline(cin, inputOTP);

    if (!isValidOTP(inputOTP, otp)) {
        cout << ">>> Invalid OTP! Role change aborted.\n";
        return;
    }

    accounts[username].role = newRole;

    string history = "Changed role of " + username + " to " + newRole;
    accounts[currentUser].transactionHistory.push_back(history);

    saveAccountsToCSV();
    writeLog("User " + currentUser + " changed role of " + username + " to " + newRole);
    cout << ">>> Role updated successfully!\n";
}

// Xóa tất cả người dùng (trừ admin)
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

    // Ghi lịch sử giao dịch
    string history = ">>> Deleted all users (except self)";
    accounts[currentUser].transactionHistory.push_back(history);

    saveAccountsToCSV();
    cout << ">>> Successfully deleted " << deletedCount << " users!\n";
}

// Quản lý người dùng (chỉ dành cho admin)
void manageUsers() {
    while (true) {
        cout << "\n-- User List --\n";
        for (const auto& account : accounts) {
            cout << "Username: " << account.second.username << " | Role: " << account.second.role << "\n";
        }

        string choice;
        cout << "\n1. Edit Permission\n2. Delete User\n3. Delete All Users\n4. Exit\n\n>>> Enter your choice: ";
        
        getline(cin, choice);

        // Kiểm tra nhập rỗng
        if (choice.empty()) {
            cout << ">>> Choice cannot be empty! Please enter a valid option.\n";
            continue;
        }

        // Kiểm tra nếu nhập không phải số hợp lệ
        if (choice.find_first_not_of("1234") != string::npos) {
            cout << ">>> Invalid choice! Please enter a number between 1-4.\n";
            continue;
        }

        int option = stoi(choice);

        switch (option) {
            case 1:
                editUserRole();
                break;
            case 2:
                deleteUser();
                break;
            case 3:
                deleteAllUsers();
                break;
            case 4:
                return;
            default:
                cout << ">>> Invalid choice! Please enter a number between 1-4.\n";
        }
    }
}

// Menu chức năng cho admin
void adminMenu() {
    while (!currentUser.empty()) {
        string choice;

        cout << "\n/----- Administrator Menu -----/\n";
        cout << "1. View My Account\n2. View User List\n3. Change Password\n4. Transfer Points\n5. Logout\n\n>>> Enter your choice: ";
        
        getline(cin, choice);

        // Kiểm tra nếu nhập rỗng
        if (choice.empty()) {
            cout << ">>> Choice cannot be empty! Please enter a valid option.\n";
            continue;
        }

        // Kiểm tra nếu nhập không phải số
        if (choice.find_first_not_of("12345") != string::npos) {
            cout << ">>> Invalid choice! Please enter a number between 1-5.\n";
            continue;
        }

        // Chuyển đổi choice sang số nguyên
        int option = stoi(choice);

        switch (option) {
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

// Menu chức năng cho user
void userMenu() {
    while (!currentUser.empty()) {
        if (accounts[currentUser].role == "administrator") {
            adminMenu();  // Nếu là admin, vào adminMenu
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

// Đăng nhập
bool login() {
    string username, password;
    int attempts = 0;

    cout << "\n/----- LOGIN -----/\n";

    cout << "Enter username: ";
    getline(cin, username);

    // Kiểm tra username có tồn tại hay không
    if (accounts.find(username) == accounts.end()) {
        cout << ">>> Account not found!" << endl;
        return false;
    }

    while (attempts < 5) {
        cout << "Enter password (Attempts left: " << (5 - attempts) << "): ";
        password = getPassword(); // Hàm nhập mật khẩu ẩn

        if (accounts[username].password == password) {
            currentUser = username;

            // Nếu đây là lần đầu đăng nhập, ghi nhận thời gian
            if (accounts[username].firstLoginAt.empty()) {
                accounts[username].firstLoginAt = getCurrentTime();
                saveAccountsToCSV();
            }

            // Nếu lần đầu đăng nhập, bắt buộc đổi mật khẩu
            if (accounts[username].isFirstLogin) {
                cout << ">>> First-time login detected. You must change your password!" << endl;
                changePassword();

                if (currentUser.empty()) return false;
            }

            writeLog("User " + username + " logged in.");
            cout << ">>> Login successful!" << endl;
            return true;
        } else {
            attempts++;
            if (attempts < 5) {
                cout << ">>> Incorrect password! Attempts left: " << (5 - attempts) << endl;
            }
        }
    }

    // Hết số lần thử
    cout << ">>> Too many incorrect attempts. Access denied!" << endl;
    return false;
}
// Kiểm tra xem chuỗi có phải là một số hợp lệ hay không

// Hàm chính
int main() {
    loadAccountsFromCSV();
    loadTotalWalletFromCSV();
    sortAccountsCSV();
    sortTotalWalletTransactionsCSV();
    string choice;
    while (true) {
        cout << "\n* * * * * * * * * * * * * * * * * * * * * * " << endl;
        cout << "*                                         *" << endl;
        cout << "*      Welcome to our Program v.1.1       *" << endl;
        cout << "*         Updated: March 19, 2025         *" << endl;
        cout << "*                                         *" << endl;
        cout << "* * * * * * * * * * * * * * * * * * * * * *" << endl;
        cout << "\nPlease choose an option below:\n";
        cout << "-------------------------------------------\n";
        cout << "\n1. Register\n2. Login\n3. Total wallet (for root)\n4. Exit program\n\n>>> Enter your choice: ";
        
        getline(cin, choice); // Sử dụng getline để tránh lỗi nhập
        
        if (choice == "1") {
            registerAccount();
        } else if (choice == "2") {
            if (login()) {
                // cout << ">>> Login successful!" << endl;
                userMenu();
            } else {
                cout << ">>> Login failed!" << endl;
            }
        } else if (choice == "4") {
            cout << ">>> Exiting program...\n";
            break;
        }
        else if (choice == "3") {
            string username, password;
            cout << "Enter username to view total wallet: ";
            getline(cin, username);
        
            // Check if the entered username exists
            if (accounts.find(username) == accounts.end()) {
                cout << ">>> User not found! Please enter a valid username.\n";
            } else {
                // Check if the entered username is an administrator (root)
                if (accounts[username].role == "administrator") {
                    // Prompt for password to confirm access
                    cout << "Enter password for " << username << ": ";
                    password = getPassword(); // Use the getPassword function to hide password input
        
                    if (accounts[username].password == password) {
                        // Only allow root/admin to view total wallet if the password matches
                        cout << "\n>>> Total wallet points: " << totalWalletPoints << endl;
                    } else {
                        cout << ">>> Incorrect password! Access denied.\n";
                    }
                } else {
                    cout << ">>> You do not have permission to view the total wallet. Only root (administrator) can access it.\n";
                }
            }
        }
        else {
            cout << ">>> Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
