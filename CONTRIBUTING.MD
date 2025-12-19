# Contributing to HolyC++

## 🎯 Welcome!

Thank you for your interest in contributing to HolyC++! This project is a modern reimplementation of Terry A. Davis's HolyC programming language, aiming to preserve its spirit while adding modern safety features and usability improvements.

## 🏛️ Project Philosophy

Before contributing, please understand our core principles:

1. **HolyC Compatibility First**: We follow the [HolyC Language Specification](https://holyc-lang.com/docs/language-spec/learn-variables) as our reference
2. **No Bootstrapping**: This implementation will **never** be self-hosted in HolyC++ (author's philosophical choice)
3. **Modern Safety**: Add optional safety features without breaking HolyC compatibility
4. **Educational Value**: Clean, readable code for studying language and compiler design
5. **Community-Driven**: Open development welcoming all contributors
6. **Freedom to Fork**: Others may create bootstrapped versions if they wish

**Important Note from the Author (@DebadityaMalakar):**  
*"I am not bootstrapping but you may fork and make a bootstrapped version for yourself."*  
This means while the main project will never self-host, forks that implement bootstrapping are welcome and permitted under GPLv3.

## 📜 License: GNU GPL v3.0

### Why This License Matters

**This project uses the GNU General Public License version 3.0 (GPLv3).** This is not just a legal formality - it's a philosophical statement about software freedom that aligns with Terry Davis's belief in open software.

### 🆓 What GPLv3 **ALLOWS** You To Do:

1. **Use Freely**: Run the software for any purpose
2. **Study & Modify**: Access and modify the source code
3. **Redistribute**: Share copies with others
4. **Improve & Share**: Distribute your modified versions
5. **Charge Money**: Sell copies or support services
6. **Patent Protection**: Contributors grant patent licenses
7. **Fork & Bootstrap**: Create versions that self-host (author explicitly allows this!)

### 🚫 What GPLv3 **DOES NOT ALLOW**:

1. **Proprietary Lock-in**: You cannot make HolyC++ proprietary
2. **Removing Freedom**: Derivatives must also be GPLv3
3. **Tivoization**: Cannot restrict users from modifying software on their own devices
4. **Patent Attacks**: Cannot use patents to make the software non-free
5. **Sublicensing**: Cannot relicense under different terms

### 🤝 The "Copyleft" Promise

GPLv3 is a **copyleft** license. This means:
- If you modify HolyC++, your changes must also be GPLv3
- If you distribute HolyC++ (modified or not), you must provide the source code
- Users of your version get the same freedoms you received
- You can create bootstrapped forks (explicitly permitted by author)

This ensures HolyC++ remains **free software** forever, honoring Terry Davis's belief in open, accessible software.

## 🛠️ How to Contribute

### 1. Development Setup

```bash
# Clone the repository
git clone https://github.com/DebadityaMalakar/holyc-plus-plus.git
cd holyc-plus-plus

# Make build script executable
chmod +x run.sh

# Build and test
./run.sh
```

### 2. Contribution Workflow

1. **Fork the repository** on GitHub
2. **Create a branch** for your feature/fix:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Make your changes** following our coding standards
4. **Write tests** for your changes
5. **Run the test suite**:
   ```bash
   ./run.sh
   ```
6. **Commit your changes** with descriptive commit messages
7. **Push to your fork** and submit a Pull Request

### 3. Project Maintainer

**Lead Developer:** [@DebadityaMalakar](https://github.com/DebadityaMalakar)  
**Philosophy:** No bootstrapping in main project, but forks may bootstrap

### 4. Coding Standards

- **C++17**: Use modern C++ features appropriately
- **HolyC Compatibility**: When in doubt, refer to the [HolyC spec](https://holyc-lang.com/docs/language-spec/learn-variables)
- **Type Safety**: Add bounds checking where it makes sense
- **Documentation**: Comment complex algorithms and HolyC-specific behavior
- **Testing**: Write comprehensive tests for new features
- **No Bootstrapping Code**: Main project will not include self-hosting features

### 5. Areas Needing Help

**Priority Tasks (Phase 2):**
- Implementing `F64` floating-point type (IEEE 754)
- Building the HolyC parser
- Adding control structures (if, while, for) with HolyC syntax
- Implementing functions and scope per HolyC spec

**Advanced Tasks (For Forkers):**
- Bootstrapped compiler implementations (in separate forks)
- Alternative backends (LLVM, WASM)
- Extended language features

**Documentation:**
- Writing tutorials and examples
- Improving API documentation
- Creating language specification

**Testing:**
- Writing test cases
- Improving test coverage
- Performance benchmarking

### 6. Pull Request Guidelines

1. **One PR per feature**: Keep changes focused
2. **Reference issues**: Link to related issues
3. **Update documentation**: Include relevant doc changes
4. **Pass all tests**: Ensure `./run.sh` succeeds
5. **Follow style**: Match existing code style
6. **No bootstrapping**: Main PRs should not add self-hosting code

## 📚 Resources

### Essential Reading:
1. **[HolyC Language Specification](https://holyc-lang.com/docs/language-spec/learn-variables)** - Our reference
2. **[TempleOS Source](https://github.com/cia-foundation/TempleOS)** - Original implementation
3. **[GNU GPL v3.0](LICENSE)** - Full license text
4. **[DebadityaMalakar's GitHub](https://github.com/DebadityaMalakar)** - Project maintainer

### Development Resources:
- `./run.sh` - Build and test script
- `tests/test_int.cpp` - Example test structure
- `src/types/` - Current type implementation

## 🔄 Forking Policy

**Explicit Permission from @DebadityaMalakar:**
> "I am not bootstrapping but you may fork and make a bootstrapped version for yourself."

This means:
- Main project stays non-bootstrapped (author's choice)
- Forks may implement bootstrapping
- Forks may add features not in main project
- All forks must remain GPLv3

**Creating a Bootstrapped Fork:**
```bash
# Create your fork on GitHub
# Add bootstrapping features in a separate branch
# Keep GPLv3 license
# Credit original project
```

## 🐛 Reporting Issues

When reporting bugs:
1. Check if the issue exists in original HolyC
2. Include reproducible code examples
3. Specify your platform and compiler
4. Check existing issues first

## 🤔 Questions?

- **License Questions**: Review the [GPLv3 FAQ](https://www.gnu.org/licenses/gpl-faq.html)
- **HolyC Questions**: Refer to the [HolyC spec](https://holyc-lang.com/docs/language-spec/learn-variables)
- **Bootstrapping Questions**: Create bootstrapped forks, don't submit to main project
- **Development Questions**: Open an issue for discussion

## 🙏 Acknowledgments

By contributing to HolyC++, you agree that:
1. Your contributions are licensed under GPLv3
2. You have the right to contribute the code
3. You respect the project's philosophy (no bootstrapping in main)
4. You understand forks may differ in philosophy

**Special Thanks:**
- **Terry A. Davis** - For creating HolyC
- **@DebadityaMalakar** - Project founder and maintainer
- **All Contributors** - Helping build modern HolyC

---

## 📝 Contributor Agreement

By submitting a contribution to HolyC++, you:
1. Agree to license your contribution under GPLv3
2. Confirm you have the right to submit the work
3. Acknowledge that contributions become part of the project
4. Understand main project won't bootstrap (but forks may)

**Thank you for helping build a modern, safe HolyC for everyone!**

*"The compiler is the Temple and the language is the Word." - Terry A. Davis*  
*"No bootstrapping, but fork freely." - @DebadityaMalakar*