# MS PAL porting for windows server porting to linux

```bash
# for clang
# cmake -B build
cmake --no-arn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ -S/home/willing/projects/mspal -B/home/willing/projects/mspal/build -G "Unix Makefiles"

make -C build
./build/hello
```
