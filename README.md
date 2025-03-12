# 🔐 Account Management System (C++)

## 📌 Giới thiệu
Dự án **Account Management System** được xây dựng bằng ngôn ngữ **C++**, cho phép người dùng:
- Đăng ký tài khoản với vai trò **User** hoặc **Administrator**.
- Đăng nhập với hệ thống **OTP bảo mật**.
- Quản lý tài khoản, bao gồm thay đổi mật khẩu và xem lịch sử giao dịch.
- Chuyển điểm giữa các tài khoản (**Point Transfer System**).
- Quản lý danh sách người dùng (**Administrator Features**).

## ⚙️ Tính năng chính
### 🎯 Người dùng (User)
- 🔹 **Đăng ký tài khoản**: Người dùng có thể đăng ký với vai trò `user` hoặc `administrator`.
- 🔹 **Đăng nhập**: Sử dụng mật khẩu ẩn (`*`) để bảo mật.
- 🔹 **Thay đổi mật khẩu**: Hệ thống yêu cầu OTP để xác thực trước khi thay đổi.
- 🔹 **Chuyển điểm (Points Transfer)**: Người dùng có thể chuyển điểm đến tài khoản khác sau khi xác thực OTP.
- 🔹 **Xem lịch sử giao dịch**: Hiển thị danh sách các giao dịch đã thực hiện.

### 🛠️ Quản trị viên (Administrator)
- ✅ **Xem danh sách người dùng**.
- ✅ **Cấp quyền cho người dùng** (chuyển đổi giữa `user` và `administrator`).
- ✅ **Xóa tài khoản**.
- ✅ **Xóa tất cả tài khoản** (ngoại trừ tài khoản của chính quản trị viên).

## 🛠️ Cài đặt và chạy chương trình
### 📌 Yêu cầu hệ thống
- **Hệ điều hành**: Windows / Linux / macOS
- **Trình biên dịch C++**: `g++` (GCC) hoặc **MSVC**

### 🔧 Cách biên dịch và chạy chương trình
1. **Biên dịch chương trình**:
   ```sh
   g++ -o account_management main.cpp
   ```
2. **Chạy chương trình**:
   ```sh
   ./account_management
   ```

## 📂 Cấu trúc dự án
```bash
.
├── accounts.csv          # Lưu trữ thông tin tài khoản
├── log.txt               # Ghi log các hoạt động hệ thống
├── main.cpp              # Code chính của hệ thống
└── README.md             # Hướng dẫn sử dụng
```

## 🔐 Cơ chế bảo mật
- **Mật khẩu ẩn**: Người dùng nhập mật khẩu được ẩn (`*`).
- **OTP**: Hệ thống gửi mã OTP 6 chữ số để xác thực trước khi thực hiện các thao tác quan trọng.
- **Nhật ký hệ thống**: Ghi lại các sự kiện quan trọng như đăng nhập, giao dịch, thay đổi mật khẩu.

## 📜 Hướng dẫn sử dụng
1. **Đăng ký tài khoản**.
2. **Đăng nhập vào hệ thống**.
3. **Nếu là lần đầu tiên đăng nhập, hệ thống yêu cầu đổi mật khẩu**.
4. **Chuyển điểm hoặc xem lịch sử giao dịch**.
5. **Administrator có thể quản lý người dùng**.

## 🏆 Đóng góp & Phát triển
Mọi đóng góp và ý tưởng mới đều được chào đón! Nếu bạn muốn cải thiện dự án, hãy tạo **Pull Request** hoặc **Issue**.

---

🚀 **Enjoy coding!**