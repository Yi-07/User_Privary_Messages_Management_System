# 用户隐私信息管理系统

本地命令行密码管理工具，用 C++ 编写。支持账户的增删改查、二进制加密存储和多维度排序。

> ⚠️ **安全声明**：本项目的密码加密算法为 Caesar cipher（ASCII 偏移 +3 mod 128），属于教学级实现，**不适用于生产环境或存储真实敏感数据**。

---

## 功能

- 添加、浏览、搜索、修改、删除账户
- 密码输入时隐藏字符（Windows `_getch` / Linux `stty`）
- 浏览时可选是否显示明文密码
- 账户按 ID 或创建时间排序
- 增量保存：内存数据与文件数据合并写入，避免覆盖
- 文件清理：支持删除单条记录、清空内容、删除文件
- 管理员密码验证（默认 `123456`），敏感操作二次确认
- 数据持久化到本地二进制文件 `accounts.dat`

## 编译与运行

**依赖**：C++11，标准库（无第三方依赖）

```bash
# GCC
g++ -std=c++11 User_Privary_messages_manage.cpp -o account_manager

# Windows（MSVC，在 VS2022 中直接打开 .sln 文件构建即可）
```

运行后，程序会在**当前工作目录**生成 `accounts.dat` 用于持久化存储。

```bash
./account_manager
# 默认管理员密码：123456
```

## 项目结构

```
├── User_Privary_messages_manage.cpp   # 全部源码（单文件）
├── accounts.dat                       # 运行时自动生成，二进制数据文件
├── Project1.sln                       # Visual Studio 解决方案
└── Project1.vcxproj                   # VS 项目文件
```

## 数据文件格式

`accounts.dat` 为二进制格式，每条记录结构如下：

```
[id: int]
[accountLocation: size_t 长度 + 字符串数据]
[description:     size_t 长度 + 字符串数据]
[username:        size_t 长度 + 字符串数据]
[password:        size_t 长度 + 加密字符串数据]
[createTime:      size_t 长度 + 字符串数据]
```

密码存储前经过 Caesar cipher 处理（每字节 `(c + 3) % 128`），解密为逆操作。

## 已知限制

- 加密强度弱，不适合存储真实账号密码
- 数据文件无完整性校验，手动修改二进制文件可能导致加载失败
- 目前仅支持单用户本地使用，无多用户隔离
- 跨平台时 `accounts.dat` 的字节序依赖平台（`int`/`size_t` 大小），不保证可移植

## 后续计划

- [ ] 替换为 AES 等标准加密算法
- [ ] 增加数据文件完整性校验（如 checksum）
- [ ] 支持导出为 CSV / JSON
- [ ] 添加密码强度检测

## License

[MIT](LICENSE)
