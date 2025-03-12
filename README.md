# 🔐 **Account Management System (C++)**  

## 📌 **Giới thiệu dự án**  
**Account Management System** là một chương trình quản lý tài khoản được viết bằng ngôn ngữ **C++**, cho phép người dùng:  
- Đăng ký và đăng nhập với hệ thống bảo mật.  
- Quản lý tài khoản, thay đổi mật khẩu và xem lịch sử giao dịch.  
- Chuyển điểm giữa các tài khoản.  
- Quản trị viên có thể quản lý người dùng: cấp quyền, xóa tài khoản, xem danh sách tài khoản.  

Hệ thống hỗ trợ cả **người dùng (user)** và **quản trị viên (administrator)**, đảm bảo **bảo mật** với cơ chế **OTP (One-Time Password)** và ghi nhật ký hệ thống (**log file**).  

---

## 👨‍💻 **Thành viên tham gia dự án**  
| Thành viên | Công việc |  
|------------|----------|  
| **Ngô Hoàng Phụng** | Xây dựng hệ thống quản lý tài khoản, xử lý đăng nhập & đăng ký |  
| | Phát triển tính năng OTP, mã hóa mật khẩu & bảo mật |  
| | Quản lý giao dịch điểm, xử lý ghi log & quản lý tài khoản |  
| | Thiết kế hệ thống file CSV, lưu trữ dữ liệu & giao diện CLI |  

---

## 📜 **Đặc tả chức năng**  

### 🟢 **1. Chức năng chính**  
| Chức năng | Mô tả |  
|-----------|-------|  
| **Đăng ký tài khoản** | Người dùng có thể tạo tài khoản mới với vai trò `user` hoặc `administrator`. |  
| **Đăng nhập** | Người dùng nhập tên đăng nhập & mật khẩu (ẩn `*` khi nhập). |  
| **OTP bảo mật** | Khi thay đổi mật khẩu hoặc thực hiện giao dịch, hệ thống yêu cầu nhập mã OTP. |  
| **Thay đổi mật khẩu** | Người dùng có thể thay đổi mật khẩu sau khi nhập đúng OTP. |  
| **Chuyển điểm (Points Transfer)** | Người dùng có thể gửi điểm cho tài khoản khác. |  
| **Xem lịch sử giao dịch** | Hiển thị danh sách các giao dịch đã thực hiện. |  
| **Quản lý người dùng (Admin)** | Admin có thể xem danh sách tài khoản, cấp quyền, xóa tài khoản. |  

### 🔴 **2. Chức năng dành riêng cho quản trị viên (Administrator)**  
| Chức năng | Mô tả |  
|-----------|-------|  
| **Xem danh sách người dùng** | Admin có thể xem tất cả tài khoản và vai trò của họ. |  
| **Cấp quyền** | Thay đổi quyền của một tài khoản giữa `user` và `administrator`. |  
| **Xóa tài khoản** | Admin có thể xóa tài khoản khỏi hệ thống. |  
| **Xóa tất cả tài khoản** | Xóa toàn bộ tài khoản (trừ tài khoản admin hiện tại). |  

---

## 📂 **Cấu trúc dự án**  
```bash
.
├── accounts.csv          # Lưu trữ thông tin tài khoản
├── log.txt               # Ghi log các hoạt động hệ thống
├── main.cpp              # Code chính của hệ thống
├── README.md             # Hướng dẫn sử dụng
└── Makefile              # Hỗ trợ biên dịch chương trình (nếu có)
```

---

## 🔧 **Hướng dẫn tải chương trình & cài đặt**  
### 📌 **1. Yêu cầu hệ thống**  
- **Hệ điều hành**: Windows / Linux / macOS  
- **Trình biên dịch C++**: `g++` (GCC) hoặc **MSVC**  
- **Thư viện cần thiết**:  
  - `<iostream>`: Giao tiếp với người dùng  
  - `<fstream>`: Đọc/ghi file CSV  
  - `<map>`: Quản lý danh sách tài khoản  
  - `<vector>`: Lưu lịch sử giao dịch  
  - `<cstdlib>` & `<ctime>`: Sinh mã OTP  
  - `<termios.h>` & `<conio.h>`: Nhập mật khẩu ẩn  

### 📌 **2. Cách tải chương trình**  
#### ✅ **Cách 1: Clone từ GitHub**  
```sh
git clone https://github.com/phungnhk24dtcn268/wallet-management-system.git
cd wallet-management-system
```

#### ✅ **Cách 2: Tải thủ công**  
- Truy cập [GitHub](https://github.com/phungnhk24dtcn268/wallet-management-system.git) và tải file **main.cpp**.  
- Lưu vào thư mục làm việc của bạn.  

### 📌 **3. Cách biên dịch chương trình**  
#### 🛠️ **Sử dụng g++ (GCC)**
```sh
g++ -o account_management main.cpp
```
#### 🛠️ **Sử dụng Makefile (nếu có)**
```sh
make
```

### 📌 **4. Chạy chương trình**  
Sau khi biên dịch xong, chạy lệnh sau:
```sh
./account_management
```

---

## 🚀 **Hướng dẫn sử dụng chương trình**  
Sau khi chạy chương trình, bạn sẽ thấy giao diện CLI như sau:  

```bash
/----- Welcome to our Program v.1.0 -----/

1. Register
2. Login
3. Exit

>>> Enter your choice:
```

### 🔹 **1. Đăng ký tài khoản**
- Chọn **1. Register**  
- Nhập tên tài khoản  
- Chọn vai trò (`user` hoặc `administrator`)  
- Hệ thống sẽ tự động cấp mật khẩu ban đầu (OTP)  

```bash
Enter username: johndoe
Enter role (user/administrator): user
>>> Generated password (first-time login required): 123456
```

### 🔹 **2. Đăng nhập**
- Chọn **2. Login**  
- Nhập tài khoản & mật khẩu (mật khẩu ẩn `*`)  

```bash
Enter username: johndoe
Enter password: ******
>>> Login successful!
```

### 🔹 **3. Chuyển điểm**
- Chọn **Transfer Points**  
- Nhập tên tài khoản nhận  
- Nhập số điểm muốn gửi  
- Nhập mã OTP để xác thực giao dịch  

```bash
Enter recipient username: janedoe
Enter amount to transfer: 100
OTP: 654321
Enter OTP to confirm transaction: 654321
>>> Transaction successful!
```

### 🔹 **4. Thay đổi mật khẩu**
- Chọn **Change Password**  
- Nhập mã OTP để xác nhận  
- Nhập mật khẩu mới  

```bash
OTP sent: 987654
Enter OTP to proceed: 987654
>>> Enter new password: ******
>>> Confirm new password: ******
>>> Password changed successfully!
```

### 🔹 **5. Quản trị viên (Admin)**
- Admin có quyền:
  - Xem danh sách tài khoản  
  - Xóa tài khoản  
  - Cấp quyền cho user  

```bash
1. View User List
2. Edit Permission
3. Delete User
4. Delete All Users
5. Exit
```

---

## 📚 **Tài liệu tham khảo**
- [C++ Reference](https://cplusplus.com/reference/)  
- [GCC Documentation](https://gcc.gnu.org/onlinedocs/)  
- [GitHub Guide](https://docs.github.com/en/get-started/using-git)  
- [File I/O in C++](https://www.geeksforgeeks.org/file-handling-c/)  

---

## 🤝 **Đóng góp & Phát triển**  
Nếu bạn muốn đóng góp, hãy tạo **Pull Request** hoặc mở **Issue** trên GitHub.  

🚀 **Enjoy coding!**