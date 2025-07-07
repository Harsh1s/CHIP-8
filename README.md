# CHIP-8

A CHIP-8 emulator written in C++.

> [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) was created by RCA engineer Joe Weisbecker in 1977 for the COSMAC VIP microcomputer. It was intended as a simpler way to make small programs and games for the computer.

### Demo Images:
![Screenshot_c8_20230302101435](https://user-images.githubusercontent.com/53043454/222313656-08436520-a643-45a8-8e78-84aa1e70a86d.png)
![Screenshot_c8_20230302101536](https://user-images.githubusercontent.com/53043454/222313676-599b8611-8e7e-4ff4-8bf3-1a1d78982b58.png)
![Screenshot_c8_20230302101610](https://user-images.githubusercontent.com/53043454/222313777-5aa4c488-843d-426e-b015-ecbafacccfb9.png)


## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
  - [Terminal](#terminal)
- [Acknowlegdements](#acknowledgements)
- [License](#license)

## Installation

> This guide assumes you have a debian based linux distro installed.

Prior to installing CHIP-8, you must have CMake installed.

```bash
sudo apt-get install cmake
```
This project uses SDL2 library for hardware access. So, install that.

```bash
sudo apt-get install libsdl2-dev
```

Clone the repository and install.

```bash
git clone https://github.com/Harsh1s/CHIP-8.git
cd CHIP-8
cmake -B bld
cmake --build bld
```

## Usage

Arguements look something like this.

```bash
<CHIP-8 executable> <Scale> <Delay> <ROM>
```

>CHIP-8 executable: The executable that cmake throws in bld folder, c8.
>
>Scale: The CHIP-8 video buffer is only 64x32, so we’ll need an integer scale factor to be able to play on our big modern monitors.
>
>Delay: The CHIP-8 had no specified clock speed, so we’ll use a delay to determine the time in milliseconds between cycles. Different games run best at different speeds, so we can control it here.
>
>ROM: The ROM file to load.

### Terminal

Run CHIP-8 through the terminal by selecting a ROM.

```bash
./bld/c8 20 5 roms/invaders.ch8
```

## Acknowledgements

- Used [Austin Morlan's guide](https://austinmorlan.com/posts/chip8_emulator/)

## Author

- Harsh Raj
- Contact: harsh.raj.cd.ece21@itbhu.ac.in

## License

This project is open source and available under the [MIT License](LICENSE).
