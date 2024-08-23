# Instalación de xv6

## Pasos Seguidos para la Instalación

1. **Clonar el repositorio de Qemu:**
   ```bash
   git checkout -b mi-rama-personalizada
   git clone https://gitlab.com/qemu-project/qemu.git
   cd qemu
   git submodule init
   git submodule update --recursive
   ./configure
   make
   sudo make install

2. **RISC-V GNU Compiler Toolchain:**
   ```bash
   $ sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev libslirp-dev

   $ git clone https://github.com/riscv/riscv-gnu-toolchain

   To build the Newlib cross-compiler, pick an install path (that is writeable). If you choose, say, /opt/riscv, then add /opt/riscv/bin to your PATH. Then

   ./configure --prefix=/opt/riscv
    sudo make

3. **Clonar el repositorio de xv6:**
   ```bash
   git clone https://github.com/mit-pdos/xv6-riscv.git
   cd xv6-riscv
   make qemu



Problemas y Resoluciones
Al instalar y hacer los pasos anteriores en  otro orden, desencadenó en que solamente qemu se ejecutaba pero no xv6
asi que se cambio el makefile y se reemplazo el -nograpichs por otro comando que generaba una nueva ventana con qemu

eso no soluciono todo el problema, asi que se comenzó desde
cero, se eliminaron todas las carpetas y todas las bibliotecas descargas, y se comenzó nuevamente con el paso a paso descrito arriba, esto permitio que al momento de hacer ingreso a la carpeta de xv6 y ejecutar "make qemu" la aplicacion funcionara sin problemas

pruebas de que funcionan los comandos dados en la tarea

`"imagenes\tarea-0-foto1.png"`
`"imagenes\tarea-0-foto2.png"`
`"imagenes\tarea-0-foto3.png"`

