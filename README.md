# P4FS

Perforce as UserMode FileSystem


## Requirements

- macOS (10.5 or higher)
- [Fuse for macOS](https://osxfuse.github.io/)
- A C compiler (e.g Xcode)
- GNU binutils

## Building

```
$ cd src
$ make
```

The resulting binary is called `p4fs`.

## Running

Make sure you have P4V open, and configured the perforce command line.
You can verify the command line working by executing `p4 info`, which should
show you the information about your current perforce connection.

To mount all perforce depots under the path `./p4`, execute the following:

```
$ ./p4fs -f p4
```

This will mount all depots under `./p4` and run it in the foreground. To unmount,
just interrupt it using Ctrl-C.

## License

Copyright (C) 2018 Arvid Gerstmann

meta is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

meta is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with meta. If not, see http://www.gnu.org/licenses/.

