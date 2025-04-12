app-name := "DataLaVista"
src-dir := "./src"
bin-dir := "./bin"
bin-linux-dir := bin-dir / "linux"
bin-linux-output := bin-linux-dir / app-name
bin-win-dir := bin-dir / "win"  # 🔹 Thêm dòng này
bin-win-output := bin-win-dir / app-name  # 🔹 Thêm dòng này
libs := "raylib"
include-dirs := "extern/include src"
src-files := "main_app/main.cpp main_app/main_window.cpp main_app/pages/main_menu.cpp main_app/pages/GraphVisualizer.cpp main_app/pages/InputBox.cpp"

linux-dbg_build:
    mkdir -p {{bin-linux-dir}}
    clang++ {{prepend(src-dir, prepend('/', src-files))}} \
            -o {{bin-linux-output}} \
            -g -fsanitize=address \
            {{prepend('$(pkg-config --libs --cflags ', append(')', libs))}} \
            {{prepend('-I', include-dirs)}}

linux-dbg_build-run: linux-dbg_build
    {{bin-linux-output}}

win-msys2-dbg_build:
    mkdir -p {{bin-win-dir}}
    g++ {{prepend(src-dir, prepend('/', src-files))}} \
            -o {{bin-win-output}} \
            -g -static-libgcc -static-libstdc++ -mwindows\
            {{prepend('$(pkg-config --libs --cflags --static ', append(')', libs))}} \
            {{prepend('-I', include-dirs)}}

clean:
    rm -rf {{bin-dir}}
