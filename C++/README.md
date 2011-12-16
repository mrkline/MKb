## Directory Contents

- The networking directory contains some basic wrappers for TCP and UDP
  communication.
- CRC32Generator generates a 32-bit cyclic redundancy check (CRC) checksum
  for a given buffer of data.
- Exceptions provides a base exception class which can report an error message
  and the function it originated from, as well as a set of derived exception
  classes which provide information about the type of error that occurred.
  The exception hierarchy is similar to that of .NET.
- MKMath provides a set of math constants as well as some utilities like
  clamping and floating point comparisons using tolerances.
- Transform provides a 4x4 transform matrix class (such as those
  used by 3D graphics for vertex transformation).
- Vector2 and Vector3 are 2D and 3D vector classes, respectively.

## Todo

- Make any needed changes now that exceptions are in a namespace

## Comment Format

Most, if not all of the C++ files are commented in a format to be consumed by
Doxygen, a documentation system which can be found at
http://www.stack.nl/~dimitri/doxygen/
