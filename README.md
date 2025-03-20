# 🚀 Point Management System (PMS) v1.1

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Last Updated](https://img.shields.io/badge/Last%20Updated-March%2019%2C%202025-orange.svg)

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
.
├── accounts.csv          # Lưu trữ thông tin tài khoản
├── log.txt               # Ghi log các hoạt động hệ thống
├── main.cpp              # Code chính của hệ thống
├── README.md             # Hướng dẫn sử dụng
└── Makefile              # Hỗ trợ biên dịch chương trình (nếu có)

###
###🔧 Hướng dẫn tải chương trình & cài đặt
