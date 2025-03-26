#include <iostream>// Để sử dụng cin, cout
#include <fstream>// Để sử dụng ifstream, ofstream
#include <string>
#include <map>
#include <vector>//
#include <cstdlib>// Để sử dụng hàm srand, rand
#include <ctime>
#include <sstream>
#include <limits>// Để sử dụng numeric_limits
#include <algorithm> // Để sử dụng std::sort
#ifdef _WIN32// Điều kiện biên dịch cho Windows
#include <conio.h>  // Dùng cho Windows
#else
#include <termios.h>// Dùng cho Linux/macOS
#include <unistd.h>// Dùng cho Linux/macOS
#endif
#include <filesystem> // Thư viện để thao tác file và thư mục (C++17 trở lên)

using namespace std;

/**
 * @brief Cấu trúc dữ liệu đại diện cho một tài khoản người dùng.
 * 
 * Các thuộc tính:
 * - `username`: Tên đăng nhập của tài khoản.
 * - `password`: Mật khẩu của tài khoản.
 * - `role`: Vai trò của tài khoản (user/administrator).
 * - `points`: Số điểm hiện tại của tài khoản.
 * - `isFirstLogin`: Cờ đánh dấu lần đầu đăng nhập (true nếu là lần đầu).
 * - `createdAt`: Thời gian tạo tài khoản.
 * - `firstLoginAt`: Thời gian đăng nhập đầu tiên.
 * - `transactionHistory`: Lịch sử giao dịch của tài khoản (danh sách các chuỗi mô tả giao dịch).
 */
struct Account {
    string username;
    string password;
    string role;
    int points;
    bool isFirstLogin;
    string createdAt;
    string firstLoginAt;
    vector<string> transactionHistory;
};

/**
 * @brief Lấy thời gian hiện tại của hệ thống.
 * 
 * Input:
 * - Không có.
 * 
 * Output:
 * - Chuỗi thời gian định dạng: "YYYY-MM-DD HH:MM:SS".
 */
string getCurrentTime() {
    time_t now = time(0); // Lấy thời gian hiện tại (giây từ 01/01/1970)
    tm* localTime = localtime(&now); // Chuyển đổi sang thời gian địa phương
    stringstream ss; // Sử dụng stringstream để định dạng chuỗi
    ss << localTime->tm_year + 1900 << "-" // Năm (cộng thêm 1900 vì tm_year là số năm từ 1900)
       << localTime->tm_mon + 1 << "-"    // Tháng (tm_mon bắt đầu từ 0, nên cần +1)
       << localTime->tm_mday << " "       // Ngày
       << localTime->tm_hour << ":"       // Giờ
       << localTime->tm_min << ":"        // Phút
       << localTime->tm_sec;             // Giây
    return ss.str(); // Trả về chuỗi thời gian đã định dạng
}

// Khai báo biến toàn cục
map<string, Account> accounts;
string currentUser;

/**
 * @brief Xóa màn hình console.
 * 
 * Input:
 * - Không có.
 * 
 * Output:
 * - Màn hình console được xóa.
 */
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // Lệnh xóa màn hình trên Windows
    #else
        system("clear"); // Lệnh xóa màn hình trên Linux/macOS
    #endif
    }

/**
 * @brief Nhập mật khẩu từ người dùng với ký tự ẩn (hiển thị `*`).
 * 
 * Input:
 * - Người dùng nhập mật khẩu từ bàn phím.
 * 
 * Output:
 * - Chuỗi mật khẩu đã nhập.
 */
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
            cout << '*'; // Hiển thị dấu * khi nhập mật khẩu
        }
    }

#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    cout << endl;
    return password;
}

/**
 * @brief Lưu số điểm hiện tại của ví tổng vào file `total_wallet.csv`.
 * 
 * Input:
 * - Không có.
 * 
 * Output:
 * - File `total_wallet.csv` được cập nhật với số điểm hiện tại.
 * - Hiển thị thông báo lỗi nếu không thể mở file.
 */
int totalWalletPoints = 1000000; // 1,000,000 điểm cho ví tổng

/**
 * @brief Lưu số điểm hiện tại của ví tổng vào file `total_wallet.csv`.
 * 
 * Input:
 * - Không có.
 * 
 * Output:
 * - File `total_wallet.csv` được cập nhật với số điểm hiện tại.
 * - Hiển thị thông báo lỗi nếu không thể mở file.
 */
void saveTotalWalletToCSV() {
    ofstream file("total_wallet.csv");
    if (file.is_open()) {
        file << "totalPoints\n"; // Ghi tiêu đề cột
        file << totalWalletPoints << endl; // Ghi giá trị số điểm hiện tại của ví tổng
        file.close(); // Đóng file
    } else {
        cout << "Unable to open file for saving total wallet data!" << endl;
    }
}

/**
 * @brief Đọc số điểm hiện tại của ví tổng từ file `total_wallet.csv`.
 * 
 * Input:
 * - File `total_wallet.csv` chứa số điểm hiện tại của ví tổng.
 * 
 * Output:
 * - Biến `totalWalletPoints` được cập nhật với giá trị từ file.
 * - Hiển thị thông báo lỗi nếu không thể mở file hoặc dữ liệu không hợp lệ.
 */
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

/**
 * @brief Lưu số điểm hiện tại của ví tổng vào file `total_wallet.csv`.
 * 
 * Input:
 * - Không có.
 * 
 * Output:
 * - File `total_wallet.csv` được cập nhật với số điểm hiện tại.
 * - Hiển thị thông báo lỗi nếu không thể mở file.
 */
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

/**
 * @brief Sắp xếp dữ liệu trong file `total_wallet_transactions.csv` theo ngày và thời gian.
 * 
 * Input:
 * - File `total_wallet_transactions.csv` chứa danh sách giao dịch với các cột:
 *   - `Date`, `Time`, `Transaction Details`, `Debit`, `Credit`, `Balance`.
 * 
 * Output:
 * - File `total_wallet_transactions.csv` được sắp xếp lại theo cột `Date` và `Time`.
 * - Hiển thị thông báo lỗi nếu không thể mở file.
 */
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

/**
 * @brief Tạo mã OTP ngẫu nhiên gồm 6 chữ số.
 * 
 * Input:
 * - Không có.
 * 
 * Output:
 * - Chuỗi ký tự gồm 6 chữ số (OTP).
 */
string generateOTP() {
    srand(time(0) + rand()); // Thêm biến đổi thời gian để tránh trùng OTP
    int otp = 100000 + rand() % 900000; // Luôn đảm bảo số có 6 chữ số
    return to_string(otp);
}

/**
 * @brief Kiểm tra tính hợp lệ của mã OTP.
 * 
 * Input:
 * - `inputOTP`: Mã OTP do người dùng nhập.
 * - `realOTP`: Mã OTP thực tế được tạo.
 * 
 * Output:
 * - `true` nếu mã OTP hợp lệ, `false` nếu không hợp lệ.
 */
bool isValidOTP(string inputOTP, string realOTP) {
    return inputOTP == realOTP;
}

/**
 * @brief Ghi thông tin log vào file `log.txt`.
 * 
 * Input:
 * - `logMessage`: Chuỗi chứa thông tin log cần ghi.
 * 
 * Output:
 * - File `log.txt` được cập nhật với thông tin log mới.
 * - Hiển thị thông báo lỗi nếu không thể mở file.
 */
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

/**
 * @brief Lưu thông tin tài khoản vào file `accounts.csv`.
 * 
 * Input:
 * - Không có.
 * 
 * Output:
 * - File `accounts.csv` được cập nhật với danh sách tài khoản.
 * - Hiển thị thông báo lỗi nếu không thể mở file.
 */
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

/**
 * @brief Tạo tài khoản root nếu chưa tồn tại.
 * 
 * Input:
 * - Không có.
 * 
 * Output:
 * - Tài khoản root được tạo nếu chưa tồn tại.
 * - Lưu thông tin tài khoản root vào file `accounts.csv`.
 */
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

/**
 * @brief Ghi thông tin giao dịch của một tài khoản vào file `transactions.csv`.
 * 
 * Input:
 * - `username`: Tên tài khoản thực hiện giao dịch.
 * - `transactionDetails`: Mô tả giao dịch (ví dụ: "Transferred to user1").
 * - `debit`: Số điểm bị trừ (nếu có).
 * - `credit`: Số điểm được cộng (nếu có).
 * - `balance`: Số dư hiện tại của tài khoản sau giao dịch.
 * 
 * Output:
 * - File `transactions.csv` được cập nhật với giao dịch mới.
 * - Hiển thị thông báo lỗi nếu không thể mở file.
 */
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

/**
 * @brief Đọc thông tin tài khoản từ file `accounts.csv` và lưu vào biến toàn cục `accounts`.
 * 
 * Input:
 * - File `accounts.csv` chứa danh sách tài khoản với các cột:
 *   - `username`, `password`, `role`, `points`, `isFirstLogin`, `createdAt`, `firstLoginAt`.
 * 
 * Output:
 * - Biến toàn cục `accounts` được cập nhật với thông tin từ file.
 * - Hiển thị thông báo lỗi nếu không thể mở file.
 */
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

/**
 * @brief Sắp xếp danh sách tài khoản trong file `accounts.csv` theo ngày tạo.
 * 
 * Input:
 * - File `accounts.csv` chứa danh sách tài khoản với các cột:
 *   - `username`, `password`, `role`, `points`, `isFirstLogin`, `createdAt`, `firstLoginAt`.
 * 
 * Output:
 * - File `accounts.csv` được sắp xếp lại theo cột `createdAt` (cột thứ 6).
 * - Hiển thị thông báo lỗi nếu không thể mở file.
 */
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

/**
 * @brief Đăng ký tài khoản mới.
 * 
 * Input:
 * - Người dùng nhập `username` (không được để trống hoặc trùng lặp).
 * - Chọn vai trò (`role`): 1 (User) hoặc 2 (Administrator).
 * - Nếu chọn Administrator, yêu cầu xác thực tài khoản root.
 * 
 * Output:
 * - Tạo tài khoản mới với thông tin:
 *   - `username`, `password` (OTP), `role`, `points`, `createdAt`.
 * - Cập nhật số điểm trong ví tổng.
 * - Ghi thông tin tài khoản vào file CSV.
 * - Hiển thị thông báo thành công hoặc lỗi.
 */
void registerAccount() {
    string username, password, role;
    int initialPoints = 0;
    string roleInput;
    clearScreen();
    // Yêu cầu nhập username, không cho phép để trống hoặc trùng lặp
    cout << "\n* * * * * * * * * * * * * * * * * * * * * * " << endl;
    cout << "*                                         *" << endl;
    cout << "*          INFORMATION REGISTRATION       *" << endl;
    cout << "*                                         *" << endl;
    cout << "* * * * * * * * * * * * * * * * * * * * * *" << endl;
    while (true) {
        cout << "\nEnter username: ";
        getline(cin, username);

        if (username.empty()) {
            cout << ">>> Username cannot be empty! Please enter again.\n";
        } else if (accounts.find(username) != accounts.end()) {
            cout << ">>> Username already exists! Try another one.\n";
        } else {
            break; // ✅ Hợp lệ, thoát vòng lặp
        }
    }

    // Nhập vai trò (1: User, 2: Administrator)
    while (true) {
        cout << "Enter role (1: User, 2: Administrator): ";
        getline(cin, roleInput);

        if (roleInput == "1") {
            role = "user";
            break;
        } else if (roleInput == "2") {
            // Xác thực tài khoản root
            string rootUsername, rootPassword;
            cout << ">>> To register as an Administrator, please authenticate with the root account.\n";
            cout << "Enter root username: ";
            getline(cin, rootUsername);
            cout << "Enter root password: ";
            rootPassword = getPassword();

            if (accounts.find(rootUsername) != accounts.end() &&
                accounts[rootUsername].password == rootPassword &&
                accounts[rootUsername].role == "administrator") {
                role = "administrator";
                break; // ✅ Xác thực thành công
            } else {
                cout << ">>> Authentication failed! Only the root account can authorize Administrator registration.\n";
            }
        } else {
            cout << ">>> Invalid role! Please enter '1' for User or '2' for Administrator.\n";
        }
    }

    // Cấp điểm khởi tạo dựa trên vai trò
    initialPoints = (role == "user") ? 100000 : 500000;

    // Kiểm tra nếu có đủ điểm trong ví tổng để cấp cho người dùng
    if (totalWalletPoints < initialPoints) {
        cout << ">>> Not enough points in the total wallet to register this account.\n";
        system("pause");
        clearScreen();
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

    // Lưu thông tin tài khoản vào file CSV
    saveAccountsToCSV();

    writeLog("New account created: " + username + " (Role: " + role + ")");

    // Ghi lại giao dịch của ví tổng
    saveTotalWalletTransactionToCSV("Points allocated to " + username, initialPoints, 0, totalWalletPoints);

    cout << "\n>>> Account registered successfully!\n";
    cout << "\n>>> WARNING!!! PLEASE BACKUP UP THE 6-DIGIT OTP CODE ABOVE! THANK YOU.\n\n";
    system("pause"); // Tạm dừng để người dùng đọc thông báo
    clearScreen();   // Xóa màn hình trước khi quay lại menu chính
}

/**
 * @brief Thay đổi mật khẩu của tài khoản hiện tại.
 * 
 * Input:
 * - Người dùng nhập mật khẩu mới và xác nhận mật khẩu.
 * - Xác thực giao dịch bằng OTP.
 * 
 * Output:
 * - Mật khẩu được cập nhật.
 * - Lưu thông tin tài khoản vào file CSV.
 * - Hiển thị thông báo thành công hoặc lỗi.
 */
void changePassword() {
    string newPassword, confirmPassword;
    int attempts = 0;  // Số lần nhập OTP sai
    clearScreen();    

    // Nhập username, không cho phép để trống hoặc trùng lặp
    cout << "\n* * * * * * * * * * * * * * * * * * * * * * " << endl;
    cout << "*                                         *" << endl;
    cout << "*          CHANGE PASSWORD PAGE           *" << endl;
    cout << "*                                         *" << endl;
    cout << "* * * * * * * * * * * * * * * * * * * * * *" << endl;
    // Xác thực bằng OTP
    string otp = generateOTP();
    cout << ">>> OTP sent to your email/phone: " << otp << endl;
    cin.ignore(1000, '\n'); // Xóa bộ đệm trước khi sử dụng getline
    string inputOTP;
    
    while (attempts < 3) {  // Giới hạn tối đa 3 lần nhập OTP
        cout << ">>> Enter OTP to proceed: ";
        getline(cin, inputOTP);
        
        // Kiểm tra nếu người dùng nhập chuỗi rỗng
        if (inputOTP.empty()) {
            cout << ">>> OTP cannot be empty! Please enter again.\n";
            continue;  // Không tăng attempts, yêu cầu nhập lại
        }
    
        // Kiểm tra OTP hợp lệ
        if (isValidOTP(inputOTP, otp)) {
            cout << ">>> OTP verified successfully!\n";
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
            cout << ">>> Password changed successfully!\n";

            // Đăng xuất để buộc user đăng nhập lại với mật khẩu mới
            cout << ">>> You have been logged out. Please login again.\n";
            currentUser = "";
            return;
        } else {
            cout << ">>> Passwords do not match! Try again.\n";
        }
    }
}

/**
 * @brief Hiển thị thông tin tài khoản hiện tại.
 * 
 * Input:
 * - Không có.
 * 
 * Output:
 * - Hiển thị thông tin tài khoản và lịch sử giao dịch.
 */
void viewAccountInfo() {
    Account& acc = accounts[currentUser];
    clearScreen();
    cout << "\n* * * * * * * * * * * * * * * * * * * * * * " << endl;
    cout << "*                                         *" << endl;
    cout << "*            Wallet information           *" << endl;
    cout << "*                                         *" << endl;
    cout << "* * * * * * * * * * * * * * * * * * * * * *" << endl;
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
    system("pause"); // Tạm dừng để người dùng đọc thông báo
    clearScreen();   // Xóa màn hình
}

/**
 * @brief Thực hiện chuyển điểm giữa hai tài khoản.
 * 
 * Input:
 * - Người dùng nhập tên tài khoản người nhận (`toUsername`).
 * - Nhập số điểm cần chuyển (`amount`).
 * - Xác thực giao dịch bằng OTP.
 * 
 * Output:
 * - Cập nhật số điểm của người gửi và người nhận.
 * - Ghi lịch sử giao dịch vào bộ nhớ và file `transactions.csv`.
 * - Hiển thị thông báo thành công hoặc lỗi.
 * 
 * Quy trình:
 * 1. Người dùng nhập tên tài khoản người nhận.
 * 2. Kiểm tra tính hợp lệ của tài khoản người nhận (không được là chính mình, không được là root, phải tồn tại).
 * 3. Người dùng nhập số điểm cần chuyển và xác thực số dư.
 * 4. Xác thực giao dịch bằng OTP.
 * 5. Nếu OTP hợp lệ:
 *    - Trừ điểm từ tài khoản người gửi.
 *    - Cộng điểm vào tài khoản người nhận.
 *    - Ghi lịch sử giao dịch vào bộ nhớ và file CSV.
 *    - Hiển thị thông báo thành công.
 * 6. Nếu OTP không hợp lệ:
 *    - Hiển thị thông báo lỗi và hủy giao dịch.
 */
void transferPoints() {
    string toUsername;
    int amount = 0;

    // Xóa bộ đệm trước khi sử dụng getline
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clearScreen();
    cout << "\n* * * * * * * * * * * * * * * * * * * * * * " << endl;
    cout << "*                                         *" << endl;
    cout << "*           TRANSFER POINT PAGE           *" << endl;
    cout << "*                                         *" << endl;
    cout << "* * * * * * * * * * * * * * * * * * * * * *" << endl;
    while (true) {
        cout << "Enter recipient username: ";
        getline(cin, toUsername);

        if (toUsername.empty()) {
            cout << ">>> Username cannot be empty! Please enter again.\n";
        } else if (toUsername == currentUser) {
            cout << ">>> You cannot transfer points to yourself!\n";
        } else if (toUsername == "root") {
            cout << ">>> You cannot transfer points to the root account!\n";
        } else if (accounts.find(toUsername) == accounts.end()) {
            cout << ">>> Recipient not found! Please enter a valid username.\n";
        } else {
            break; // ✅ Tên tài khoản hợp lệ
        }
    }

    // Nhập số điểm cần chuyển
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

        break; //✅ Số điểm hợp lệ
    }

    // Xác thực OTP trước khi thực hiện giao dịch
    string otp = generateOTP();
    cout << "OTP: " << otp << endl;

    string inputOTP;
    cout << "Enter OTP to confirm transaction: ";
    getline(cin, inputOTP);

    if (!isValidOTP(inputOTP, otp)) {
        cout << ">>> Invalid OTP! Transaction failed.\n";
        return;
    }

    // Thực hiện giao dịch
    accounts[currentUser].points -= amount;
    accounts[toUsername].points += amount;

    // Ghi lịch sử giao dịch vào bộ nhớ
    string senderHistory = "Transferred to " + toUsername + ": -" + to_string(amount) + " points";
    string recipientHistory = "Received from " + currentUser + ": +" + to_string(amount) + " points";

    accounts[currentUser].transactionHistory.push_back(senderHistory);
    accounts[toUsername].transactionHistory.push_back(recipientHistory);

    // Ghi lịch sử giao dịch vào file CSV
    saveTransactionToCSV(currentUser, "Transferred to " + toUsername, amount, 0, accounts[currentUser].points);
    saveTransactionToCSV(toUsername, "Received from " + currentUser, 0, amount, accounts[toUsername].points);

    // Lưu thông tin tài khoản vào file CSV
    saveAccountsToCSV();

    // Ghi log
    writeLog("Transaction: " + currentUser + " transferred " + to_string(amount) + " points to " + toUsername);

    cout << ">>> Transaction successful!" << endl;
    system("pause");
    clearScreen();
}

/**
 * @brief Xóa một tài khoản người dùng.
 * 
 * Input:
 * - Người dùng nhập tên tài khoản cần xóa.
 * - Xác nhận hành động bằng OTP.
 * 
 * Output:
 * - Tài khoản bị xóa khỏi hệ thống.
 * - Cập nhật danh sách tài khoản trong file `accounts.csv`.
 * - Hiển thị thông báo thành công hoặc lỗi.
 */
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

/**
 * @brief Chỉnh sửa quyền của một tài khoản người dùng.
 * 
 * Input:
 * - Người dùng nhập tên tài khoản cần chỉnh sửa quyền.
 * - Nhập vai trò mới (`user` hoặc `administrator`).
 * - Xác nhận hành động bằng OTP.
 * 
 * Output:
 * - Quyền của tài khoản được cập nhật.
 * - Cập nhật danh sách tài khoản trong file `accounts.csv`.
 * - Hiển thị thông báo thành công hoặc lỗi.
 */
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
    system("pause");
    clearScreen();
}

/**
 * @brief Xóa tất cả tài khoản người dùng (trừ admin hiện tại).
 * 
 * Input:
 * - Người dùng xác nhận hành động xóa bằng cách nhập `y` hoặc `n`.
 * 
 * Output:
 * - Tất cả tài khoản người dùng (trừ admin hiện tại) bị xóa khỏi hệ thống.
 * - Cập nhật danh sách tài khoản trong file `accounts.csv`.
 * - Hiển thị thông báo thành công hoặc hủy bỏ hành động.
 */
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
    cin.ignore(1000, '\n'); // Xóa bộ đệm trước khi tiếp tục
    system("pause");
    clearScreen();
}

/**
 * @brief Menu chức năng dành cho quản trị viên (administrator).
 * 
 * Input:
 * - Người dùng chọn một trong các tùy chọn sau:
 *   - `1`: Xem thông tin tài khoản của chính mình.
 *   - `2`: Quản lý danh sách người dùng (xem, chỉnh sửa quyền, xóa tài khoản).
 *   - `3`: Thay đổi mật khẩu.
 *   - `4`: Chuyển điểm.
 *   - `5`: Đăng xuất.
 * 
 * Output:
 * - Thực hiện các chức năng tương ứng với lựa chọn của quản trị viên.
 * - Hiển thị thông báo thành công hoặc lỗi.
 */
void manageUsers() {
    clearScreen();
    while (true) {
        cout << "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;
        cout << "*                                                                               *" << endl;
        cout << "*                                USER INFORMATION LIST                          *" << endl;
        cout << "*                                                                               *" << endl;
        cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;

        // Hiển thị tiêu đề bảng
        cout << left << setw(15) << "Username" 
             << setw(15) << "Role" 
             << setw(10) << "Points" 
             << setw(20) << "Created At" 
             << setw(20) << "First Login At" << endl;
        cout << string(80, '-') << endl; // Dòng kẻ ngang

        // Hiển thị danh sách người dùng
        for (const auto& account : accounts) {
            cout << left << setw(15) << account.second.username
                 << setw(15) << account.second.role
                 << setw(10) << account.second.points
                 << setw(20) << account.second.createdAt
                 << setw(20) << (account.second.firstLoginAt.empty() ? "N/A" : account.second.firstLoginAt)
                 << endl;
        }

        string choice;
        cout << "\n1. Edit Permission\n2. Delete User\n3. Delete All Users\n4. Exit\n\n>>> Enter your choice: ";
        getline(cin, choice);

        if (choice == "1") {
            editUserRole();
        } else if (choice == "2") {
            deleteUser();
        } else if (choice == "3") {
            deleteAllUsers();
        } else if (choice == "4") {
            clearScreen();
            return;
        } else {
            cout << ">>> Invalid choice! Please enter a valid option.\n";
            system("pause");
            clearScreen();
        }
    }
}

/**
 * @brief Menu chức năng dành cho quản trị viên (administrator).
 * 
 * Input:
 * - Người dùng chọn một trong các tùy chọn sau:
 *   - `1`: Xem thông tin tài khoản của chính mình.
 *   - `2`: Quản lý danh sách người dùng (xem, chỉnh sửa quyền, xóa tài khoản).
 *   - `3`: Thay đổi mật khẩu.
 *   - `4`: Chuyển điểm.
 *   - `5`: Đăng xuất.
 * 
 * Output:
 * - Thực hiện các chức năng tương ứng với lựa chọn của quản trị viên.
 * - Hiển thị thông báo thành công hoặc lỗi.
 */
void adminMenu() {
    while (!currentUser.empty()) {
        string choice;
        cout << "\n* * * * * * * * * * * * * * * * * * * * * * " << endl;
        cout << "*                                         *" << endl;
        cout << "*            ADMINISTRATOR PAGE           *" << endl;
        cout << "*                                         *" << endl;
        cout << "* * * * * * * * * * * * * * * * * * * * * *" << endl;
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
            system("pause");
            clearScreen();
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
                system("pause");
                clearScreen();
        }
    }
}

/**
 * @brief Menu chức năng cho người dùng.
 * 
 * Input:
 * - Người dùng chọn một trong các tùy chọn sau:
 *   - `1`: Xem thông tin tài khoản.
 *   - `2`: Thay đổi mật khẩu.
 *   - `3`: Chuyển điểm.
 *   - `4`: Đăng xuất.
 * 
 * Output:
 * - Thực hiện các chức năng tương ứng với lựa chọn của người dùng.
 * - Hiển thị thông báo thành công hoặc lỗi.
 * - Nếu người dùng là administrator, chuyển sang menu quản trị (`adminMenu()`).
 */
void userMenu() {
    clearScreen();   // Xóa màn hình trước khi quay lại menu chính
    while (!currentUser.empty()) {
        if (accounts[currentUser].role == "administrator") {
            adminMenu();  // Nếu là admin, vào adminMenu
            return;
        }
        cout << "\n* * * * * * * * * * * * * * * * * * * * * * " << endl;
        cout << "*                                         *" << endl;
        cout << "*              YOUR PROFILE               *" << endl;
        cout << "*                                         *" << endl;
        cout << "* * * * * * * * * * * * * * * * * * * * * *" << endl;
        cout << "\nPlease choose an option below:\n";
        int choice;
        cout << "\n1. View Account\n2. Change Password\n3. Transfer Points\n4. Logout\n\n>>> Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
            system("pause");
            clearScreen();
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
                currentUser = "";
                cin.ignore(1000, '\n');
                return;
                
            default:
                cout << ">>> Invalid choice! Please enter a number between 1-4.\n";
                system("pause");
                clearScreen();
        }
    }
}

/**
 * @brief Đăng nhập vào hệ thống.
 * 
 * Input:
 * - Người dùng nhập `username` và `password`.
 * 
 * Output:
 * - Xác thực thông tin đăng nhập.
 * - Hiển thị thông báo thành công hoặc lỗi.
 */
bool login() {
    string username, password;
    int attempts = 0;
    clearScreen();   // Xóa màn hình trước khi quay lại menu chính
    cout << "\n* * * * * * * * * * * * * * * * * * * * * * " << endl;
    cout << "*                                         *" << endl;
    cout << "*                LOGIN PAGE               *" << endl;
    cout << "*                                         *" << endl;
    cout << "* * * * * * * * * * * * * * * * * * * * * *" << endl;

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

/**
 * @brief Sao lưu dữ liệu hiện tại vào thư mục `backup/`.
 * 
 * Input:
 * - Không có.
 * 
 * Output:
 * - Các file CSV được sao lưu vào thư mục `backup/` với tên chứa dấu thời gian.
 */
void backupData() {
    // Kiểm tra quyền truy cập
    if (accounts[currentUser].role != "administrator") {
        cout << ">>> Access denied! Only administrators can perform backups.\n";
        return;
    }

    // Tạo thư mục backup nếu chưa tồn tại
    std::filesystem::create_directory("backup");

    // Lấy thời gian hiện tại để đặt tên file
    string timestamp = getCurrentTime(); // Hàm getCurrentTime() đã có trong mã
    std::replace(timestamp.begin(), timestamp.end(), ' ', '_'); // Thay khoảng trắng bằng dấu gạch dưới
    std::replace(timestamp.begin(), timestamp.end(), ':', '-'); // Thay dấu ":" bằng "-"

    // Danh sách các file cần sao lưu
    vector<string> filesToBackup = {
        "accounts.csv",
        "total_wallet.csv",
        "transactions.csv",
        "total_wallet_transactions.csv"
    };

    // Sao lưu từng file
    for (const string& file : filesToBackup) {
        string backupFileName = "backup/" + file.substr(0, file.find('.')) + "_" + timestamp + ".csv";
        try {
            std::filesystem::copy(file, backupFileName, std::filesystem::copy_options::overwrite_existing);
            cout << ">>> Backup successful for file: " << file << " -> " << backupFileName << endl;
        } catch (const std::exception& e) {
            cout << ">>> Backup failed for file: " << file << ". Error: " << e.what() << endl;
        }
    }
}

/**
 * @brief Phục hồi dữ liệu từ một file sao lưu.
 * 
 * Input:
 * - Người dùng chọn file sao lưu từ danh sách.
 * 
 * Output:
 * - Dữ liệu được phục hồi từ file sao lưu.
 * - Hiển thị thông báo thành công hoặc lỗi.
 */
void restoreData() {
    // Kiểm tra quyền truy cập
    if (accounts[currentUser].role != "administrator") {
        cout << ">>> Access denied! Only administrators can restore data.\n";
        return;
    }

    // Kiểm tra xem thư mục backup có tồn tại không
    if (!std::filesystem::exists("backup")) {
        cout << ">>> Backup folder does not exist! No backups available.\n";
        return;
    }

    // Hiển thị danh sách các file sao lưu
    cout << ">>> Available backup files:\n";
    vector<string> backupFiles;
    for (const auto& entry : std::filesystem::directory_iterator("backup")) {
        if (entry.is_regular_file()) {
            backupFiles.push_back(entry.path().string());
            cout << backupFiles.size() << ". " << entry.path().filename().string() << endl;
        }
    }

    if (backupFiles.empty()) {
        cout << ">>> No backup files found!\n";
        return;
    }

    // Yêu cầu người dùng chọn file sao lưu
    int choice;
    cout << ">>> Enter the number of the backup file to restore: ";
    cin >> choice;

    // Kiểm tra tính hợp lệ của lựa chọn
    if (cin.fail() || choice < 1 || choice > static_cast<int>(backupFiles.size())) {
        cin.clear(); // Xóa trạng thái lỗi của cin
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm
        cout << ">>> Invalid choice! Restore canceled.\n";
        return;
    }

    // Phục hồi từng file
    string selectedBackup = backupFiles[choice - 1];
    string originalFile = selectedBackup.substr(selectedBackup.find_last_of('/') + 1);
    originalFile = originalFile.substr(0, originalFile.find_last_of('_')) + ".csv";

    try {
        std::filesystem::copy(selectedBackup, originalFile, std::filesystem::copy_options::overwrite_existing);
        cout << ">>> Restore successful for file: " << originalFile << endl;
    } catch (const std::exception& e) {
        cout << ">>> Restore failed for file: " << originalFile << ". Error: " << e.what() << endl;
    }
}

/**
 * @brief Điểm bắt đầu của chương trình.
 * 
 * Input:
 * - Người dùng chọn các tùy chọn từ menu chính.
 * 
 * Output:
 * - Điều hướng người dùng đến các chức năng tương ứng.
 * - Hiển thị thông báo và kết quả của từng chức năng.
 */
int main() {
    loadAccountsFromCSV();
    loadTotalWalletFromCSV();
    sortAccountsCSV();
    sortTotalWalletTransactionsCSV();
    string choice; 
    clearScreen();   // Xóa màn hình trước khi quay lại menu chính
    while (true) {
        cout << "\n* * * * * * * * * * * * * * * * * * * * * * " << endl;
        cout << "*                                         *" << endl;
        cout << "*      Welcome to our Program v.1.1       *" << endl;
        cout << "*         Updated: March 19, 2025         *" << endl;
        cout << "*                                         *" << endl;
        cout << "* * * * * * * * * * * * * * * * * * * * * *" << endl;
        cout << "\nPlease choose an option below:\n";
        cout << "-------------------------------------------\n";
        cout << "\n1. Register\n2. Login\n3. Total wallet (for root)\n4. Exit program\n5. Backup Data\n6. Restore Data\n\n>>> Enter your choice: ";
        getline(cin, choice); // Sử dụng getline để tránh lỗi nhập từ bộ đệm
        if (choice == "1") {
            registerAccount();
        } else if (choice == "2") {
            if (login()) {
                userMenu();
                clearScreen();   // Xóa màn hình trước khi quay lại menu chính
            } else {
                cout << ">>> Login failed!" << endl;
                system("pause");
                clearScreen();
            }
        } else if (choice == "4") {
            cout << ">>> Exiting program...\n";
            clearScreen();
            break;
        }
        else if (choice == "3") {
            string username, password;
            cout << "Enter username to view total wallet: ";
            getline(cin, username);
        
            // Kiểm tra xem tên người dùng đã nhập có tồn tại không
            if (accounts.find(username) == accounts.end()) {
                cout << ">>> User not found! Please enter a valid username.\n";
                system("pause");
                clearScreen();
            } else {
                // Kiểm tra xem tên người dùng đã nhập có phải là quản trị viên (root) hay không
                if (accounts[username].role == "administrator") {
                    // Nhắc nhập mật khẩu để xác nhận quyền truy cập
                    cout << "Enter password for " << username << ": ";
                    password = getPassword(); // Sử dụng hàm getPassword() để ẩn đầu vào mật khẩu
        
                    if (accounts[username].password == password) {
                        // Chỉ cho phép root/admin xem tổng số ví nếu mật khẩu khớp
                        clearScreen();   // Xóa màn hình trước khi hiển thị tổng số ví
                        cout << "\n* * * * * * * * * * * * * * * * * * * * * * " << endl;
                        cout << "*                                         *" << endl;
                        cout << "*        TOTAL WALLET INFORMATION         *" << endl;
                        cout << "*                                         *" << endl;
                        cout << "* * * * * * * * * * * * * * * * * * * * * *" << endl;
                        cout << "\n>>> Balance: " << totalWalletPoints << "\n" << endl;
                        system("pause");
                        clearScreen();
                    } else {
                        cout << ">>> Incorrect password! Access denied.\n";
                        system("pause");
                        clearScreen();
                    }
                } else {
                    cout << ">>> You do not have permission to view the total wallet. Only root (administrator) can access it.\n";
                    system("pause");
                    clearScreen();
                }
            }
        }
        else if (choice == "5") { // Backup Data
            cout << ">>> Please log in as an administrator to perform a backup.\n";
            if (login() && accounts[currentUser].role == "administrator") { // Yêu cầu đăng nhập và kiểm tra quyền
                backupData();
            } else {
                cout << ">>> Access denied! Only administrators can perform backups.\n";
            }
            system("pause");
            clearScreen();
        } else if (choice == "6") { // Restore Data
            cout << ">>> Please log in as an administrator to restore data.\n";
            if (login() && accounts[currentUser].role == "administrator") { // Yêu cầu đăng nhập và kiểm tra quyền
                restoreData();
            } else {
                cout << ">>> Access denied! Only administrators can restore data.\n";
            }
            system("pause");
            clearScreen();
        }
        else {
            cout << ">>> Invalid choice. Please try again." << endl;
            system("pause");
            clearScreen();
        }
    }

    return 0;
    system("pause"); // Tạm dừng để người dùng đọc thông báo
    clearScreen();   // Xóa màn hình trước khi quay lại menu chính
}