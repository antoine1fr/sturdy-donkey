# Sturdy Donkey's change log

## Version 0.2.0

Some advances but still, you can't do anything useful with this engine yet.
Lighting is still completely broken. While trying to debug and profile the
0.1.0, I started feeling the impact of OpenGL's end of life on macOS. OpenGL
isn't supported as well as Metal in Apple's instruments, and macOS lacks great
free third-party tools. NVIDIA doesn't ship Nsight on macOS and Intel's Frame
Analyzer began to prove quite limited. So the main goal of this release was to
start porting my small code base to Metal. Instead of playing a little and
implement some first cool rendering technique, I decided to directly start to
migrate the renderer to a command-bucket-oriented design. I was planning to play
with these kind of techniques at some point anyway, so I thought I'd better do
this while the code base is still small. Once the renderer is completely free of
calls to OpenGL, implementing support for Metal should be fairly
straightforward.

Here are the few notable things I did since the last release:

* Add support for wavefront mesh files, thanks to tinobjloader.
* Add a new example called boulders.
* Sort render objects by material before drawing.
* Start decoupling calls to OpenGL from the renderer, through the introduction
  of the notion of command buckets and drivers. This is the first step to
  implementing support for multiple graphics APIs.

For an exhaustive change log, please run the following command:

```
$ git log --oneline 0.1.0..0.2.0
```

## Version 0.1.0

Very first version of a pipelined game engine, with a deferred shading
renderer. There is a little demo in the examples/quad folder.
