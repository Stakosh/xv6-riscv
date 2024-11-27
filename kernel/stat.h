#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device
#define PERMISSION_NONE       0
#define PERMISSION_READ       1
#define PERMISSION_WRITE      2
#define PERMISSION_RW         3
#define PERMISSION_IMMUTABLE  5


struct stat {
  int dev;     // File system's disk device
  uint ino;    // Inode number
  short type;  // Type of file
  short nlink; // Number of links to file
  uint64 size; // Size of file in bytes
};
