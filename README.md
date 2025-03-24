# 🚀 Point Management System (PMS) v1.1

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Last Updated](https://img.shields.io/badge/Last%20Updated-March%2024%2C%202025-orange.svg)

## 📝 Giới thiệu
Chương trình **Point Management System (PMS)** là một hệ thống quản lý điểm đơn giản được viết bằng ngôn ngữ C++. Hệ thống cho phép người dùng đăng ký, đăng nhập, quản lý điểm, chuyển điểm giữa các tài khoản, và thực hiện các giao dịch khác. Hệ thống cũng hỗ trợ phân quyền người dùng (User và Administrator) và quản lý ví tổng.

Hệ thống hỗ trợ cả **người dùng (user)** và **quản trị viên (administrator)**, đảm bảo **bảo mật** với cơ chế **OTP (One-Time Password)** và ghi nhật ký hệ thống (**log file**).

---

## 🌟 Tính năng chính

- **Đăng ký tài khoản mới** 🆕
  - Tạo tài khoản với vai trò User hoặc Administrator (yêu cầu xác thực từ root).
  - Mật khẩu được tạo tự động bằng OTP (One-Time Password) và yêu cầu thay đổi sau lần đăng nhập đầu tiên.

- **Đăng nhập hệ thống** 🔑
  - Xác thực người dùng bằng username và password.
  - Bắt buộc thay đổi mật khẩu sau lần đăng nhập đầu tiên.

- **Quản lý điểm** 💰
  - Chuyển điểm giữa các tài khoản.
  - Xem lịch sử giao dịch.

- **Quản lý người dùng (Admin)** 👨‍💻
  - Xem danh sách người dùng.
  - Chỉnh sửa quyền người dùng.
  - Xóa người dùng hoặc xóa tất cả người dùng (trừ admin hiện tại).

- **Ví tổng (Root)** 🏦
  - Quản lý tổng số điểm trong hệ thống.
  - Xem lịch sử giao dịch của ví tổng.

- **Ghi log hệ thống** 📜
  - Ghi lại các hoạt động quan trọng như đăng nhập, đăng ký, chuyển điểm, v.v.

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
├── .vscode/
  ├── c_cpp_properties.json
  ├── launch.json
  ├── settings.json
├── output/                           # Thư mục chứa các file dữ liệu đầu ra
  ├── accounts.csv                    # Lưu trữ thông tin tài khoản
  ├── log.txt                         # Ghi log các hoạt động hệ thống
  ├── main.exe                        # File thực thi (sinh ra sau khi biên dịch)
  ├── total_wallet.csv                # Lưu thông tin ví tổng
  ├── total_wallet_transactions.csv   # Lưu lịch sử giao dịch ví tổng
  ├── transactions.csv
├── main.cpp                          # Code chính của hệ thống
└── README.md                         # Tài liệu mô tả dự án
```
## 💡 **Cấu hình môi trường**
1. **Cài đặt**:
   - Mở dự án trong Visual Studio Code.
   - Đảm bảo đã cài đặt trình biên dịch C++ (ví dụ: MinGW hoặc GCC).
   - Cấu hình file `.vscode/c_cpp_properties.json` và `launch.json` nếu cần.

2. **Chạy chương trình**:
   - Biên dịch `main.cpp` bằng lệnh:
     ```bash
     g++ -o main.exe main.cpp
     ```
   - Chạy chương trình:
     ```bash
     ./main.exe
     ```

## 🌟 Hướng dẫn sử dụng

### 1. Đăng ký tài khoản
**Mô tả:** Tạo tài khoản mới với vai trò user hoặc administrator.  
**Hướng dẫn:**
1. Chọn tùy chọn **1. Register** từ menu chính.
2. Nhập tên tài khoản (username).
3. Chọn vai trò:
   - 1: **User**.
   - 2: **Administrator** (yêu cầu xác thực từ tài khoản root).
4. Nhập số điểm khởi tạo (nếu là tài khoản user).
5. OTP sẽ được tạo tự động và hiển thị trên màn hình.
6. Sau khi đăng ký thành công, hệ thống sẽ yêu cầu bạn đăng nhập.

### 2. Đăng nhập
**Mô tả:** Xác thực người dùng bằng username và password.  
**Hướng dẫn:**
1. Chọn tùy chọn **2. Login** từ menu chính.
2. Nhập tên tài khoản (username).
3. Nhập mật khẩu (ẩn ký tự bằng `*` khi nhập).
4. Nếu đăng nhập thành công:
   - Nếu đây là lần đầu đăng nhập, hệ thống sẽ yêu cầu thay đổi mật khẩu.
   - Nếu không, bạn sẽ được chuyển đến menu người dùng.

### 3. Thay đổi mật khẩu
**Mô tả:** Người dùng có thể thay đổi mật khẩu sau khi nhập đúng OTP.  
**Hướng dẫn:**
1. Chọn tùy chọn **Change Password** từ menu người dùng.
2. Hệ thống sẽ gửi OTP đến email/phone (hiển thị trên màn hình).
3. Nhập OTP để xác thực.
4. Nhập mật khẩu mới và xác nhận lại mật khẩu.
5. Nếu thành công, hệ thống sẽ thông báo **"Password changed successfully!"**.

### 4. Chuyển điểm
**Mô tả:** Người dùng có thể gửi điểm cho tài khoản khác.  
**Hướng dẫn:**
1. Chọn tùy chọn **Transfer Points** từ menu người dùng.
2. Nhập tên tài khoản người nhận.
3. Nhập số điểm cần chuyển.
4. Hệ thống sẽ yêu cầu xác thực bằng OTP.
5. Nếu OTP hợp lệ, giao dịch sẽ được thực hiện và ghi lại trong lịch sử giao dịch.

### 5. Quản lý người dùng (Admin)
**Mô tả:** Chỉ dành cho tài khoản administrator.  
**Hướng dẫn:**
1. Chọn tùy chọn **Manage Users** từ menu admin.
2. Các chức năng quản lý bao gồm:
   - **Xem danh sách người dùng**: Hiển thị tất cả tài khoản và vai trò.
   - **Cấp quyền**: Thay đổi quyền của tài khoản giữa user và administrator.
   - **Xóa tài khoản**: Xóa tài khoản khỏi hệ thống.
   - **Xóa tất cả tài khoản**: Xóa toàn bộ tài khoản (trừ admin hiện tại).

### 6. Ví tổng (Root)
**Mô tả:** Chỉ dành cho tài khoản root.  
**Hướng dẫn:**
1. Chọn tùy chọn **Total Wallet** từ menu admin.
2. Các chức năng quản lý ví tổng bao gồm:
   - **Xem số dư ví tổng**.
   - **Xem lịch sử giao dịch ví tổng**.
  
### 7. Xóa tất cả tài khoản (Admin)
**Mô tả:** Xóa toàn bộ tài khoản người dùng (trừ tài khoản admin hiện tại).  
**Hướng dẫn:**
1. Chọn tùy chọn **Delete All Users** từ menu admin.
2. Hệ thống sẽ yêu cầu xác nhận bằng cách nhập `y` hoặc `n`.
3. Nếu xác nhận, tất cả tài khoản (trừ admin hiện tại) sẽ bị xóa khỏi hệ thống.

### 8. Chỉnh sửa quyền người dùng (Admin)
**Mô tả:** Thay đổi quyền của một tài khoản giữa `user` và `administrator`.  
**Hướng dẫn:**
1. Chọn tùy chọn **Edit Permission** từ menu admin.
2. Nhập tên tài khoản cần chỉnh sửa quyền.
3. Nhập vai trò mới (`user` hoặc `administrator`).
4. Hệ thống sẽ yêu cầu xác thực bằng OTP.
5. Nếu OTP hợp lệ, quyền của tài khoản sẽ được cập nhật.

### 9. Xóa tài khoản (Admin)
**Mô tả:** Xóa một tài khoản người dùng khỏi hệ thống.  
**Hướng dẫn:**
1. Chọn tùy chọn **Delete User** từ menu admin.
2. Nhập tên tài khoản cần xóa.
3. Hệ thống sẽ yêu cầu xác thực bằng OTP.
4. Nếu OTP hợp lệ, tài khoản sẽ bị xóa khỏi hệ thống.

### 10. Lưu thông tin ví tổng
**Mô tả:** Lưu số điểm hiện tại của ví tổng vào file `total_wallet.csv`.  
**Hướng dẫn:**
- Hệ thống tự động gọi hàm này sau khi cập nhật số điểm trong ví tổng.
- File `total_wallet.csv` sẽ được cập nhật với số điểm mới nhất.

### 11. Sắp xếp lịch sử giao dịch ví tổng
**Mô tả:** Sắp xếp dữ liệu trong file `total_wallet_transactions.csv` theo ngày và thời gian.  
**Hướng dẫn:**
- Hệ thống tự động gọi hàm này sau khi ghi giao dịch mới vào file.
- Dữ liệu được sắp xếp theo thứ tự tăng dần của cột `Date` và `Time`.
---

## 📂 Cấu trúc dữ liệu

### 1. `accounts.csv`
Lưu thông tin tài khoản:
- **username**: Tên đăng nhập.
- **password**: Mật khẩu.
- **role**: Vai trò (user hoặc administrator).
- **points**: Số điểm hiện có.
- **isFirstLogin**: Đánh dấu lần đầu đăng nhập.
- **createdAt**: Thời gian tạo tài khoản.
- **firstLoginAt**: Thời gian đăng nhập đầu tiên.

### 2. `total_wallet.csv`
Lưu số điểm hiện có trong ví tổng.

### 3. `total_wallet_transactions.csv`
Lưu lịch sử giao dịch của ví tổng:
- **Date**: Ngày giao dịch.
- **Time**: Thời gian giao dịch.
- **Transaction Details**: Chi tiết giao dịch.
- **Debit**: Số điểm trừ.
- **Credit**: Số điểm cộng.
- **Balance**: Số dư sau giao dịch.

**Lưu ý:** Dữ liệu trong file này được sắp xếp theo cột `Date` và `Time`.

### 4. `log.txt`
Ghi lại các hoạt động quan trọng:
- Đăng nhập, đăng ký, thay đổi mật khẩu, chuyển điểm, v.v.

---

## 🛠 Xử lý lỗi

### 1. Không thể đăng nhập:
- Kiểm tra lại **username** và **password**.
- Đảm bảo tài khoản đã được đăng ký.

### 2. OTP không hợp lệ:
- Kiểm tra lại OTP được cung cấp.
- Nếu nhập sai quá 3 lần, bạn sẽ bị đăng xuất.

### 3. Không đủ điểm để chuyển:
- Kiểm tra số dư tài khoản trước khi thực hiện giao dịch.

### 4. Không thể mở file CSV:
- Đảm bảo file CSV tồn tại trong thư mục `output/`.
- Kiểm tra quyền ghi/đọc của file.

### 5. Lỗi khi thay đổi quyền người dùng:
- Đảm bảo tài khoản cần chỉnh sửa quyền tồn tại.
- Không thể thay đổi quyền của tài khoản admin hiện tại.

## 🖥 Yêu cầu hệ thống
- Hệ điều hành: Windows, Linux, hoặc macOS.
- Trình biên dịch C++: GCC (MinGW), Clang, hoặc MSVC.
- Visual Studio Code (khuyến nghị) với tiện ích mở rộng C/C++.

## 🤝 Đóng góp
Nếu bạn muốn đóng góp cho dự án, vui lòng:
1. Fork repository này.
2. Tạo branch mới: `git checkout -b feature/ten-tinh-nang`.
3. Commit thay đổi: `git commit -m "Thêm tính năng mới"`.
4. Push branch: `git push origin feature/ten-tinh-nang`.
5. Tạo Pull Request.
---

### 📞 Hỗ trợ
Nếu gặp vấn đề hoặc cần hỗ trợ, vui lòng liên hệ:

- **Email:** [phungnh.k24dtcn268@stu.ptit.edu.vn](mailto:phungnh.k24dtcn268@stu.ptit.edu.vn)

---
