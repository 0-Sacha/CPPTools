# CPPTools

Formatter and profiler function for C++17.

The String Formatter looks like [&lt;fmt&gt;](https://github.com/fmtlib/fmt)
Using it there is some others tools provided:
- Logger like [spdlog](https://github.com/gabime/spdlog)
- Profiler
- Test Suite

Tested only on Windows and Visual Studio. However it should, at least, compile on gcc/clang, see below.

## Not update anymore

Not update anymore, the new version: [ProjectCore](https://github.com/0-Sacha/ProjectCore) which is based on `CPPTools`.

## Docs / Examples

If you want ot see an example, there is the included [Sandbox](Sandbox/src/Sandbox.cpp) project if you want to try this lib.
But sorry, I did this project when learning programming, by the time I wasn't into code usability, there is either no docs nor comments.
Maybe some examples will come later, if I have the time...

Instead you can checkout the new version [ProjectCore](https://github.com/0-Sacha/ProjectCore) which is mostly the exact same version as this one but updated to use C++20.
Therefore, the docs and comments should be usable for this project.

## Using it

Create the Visual Studio solutions files
```
./VSProject.bat
```
Then you can launch the generated Visual Studio solution and hit Start.

To use gcc or clang use the `MakeProject.bat`, it will generate some Makefiles using `Premake`:
```
make -j8 all
```
Will compile everything. To be able to batter config your build, you can checkout the [Premake](https://premake.github.io/docs/using-premake) documentation.
